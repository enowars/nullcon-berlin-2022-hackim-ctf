#include <string.h>

#include "periph/flashpage.h"
#include "shell.h"

#define ENABLE_DEBUG    (1)
#include "debug.h"

#include <stdlib.h>

#include "base64.h"

#define MAX_SZ 0x20

typedef struct
{
    char* in;
    uint8_t in_sz;
    char out[MAX_SZ / 2];
    uint8_t out_sz;
} req_t;

static int _enc(req_t *req)
{ 
    int ret = 0;
    size_t sz = req->out_sz;

    ret = base64_encode(req->in, req->in_sz, req->out, &sz);

    if (ret != BASE64_SUCCESS) {
        DEBUG("Encoding failed \n");
        return -1;
    }
    
    req->out[sz] = 0x00;

    req->in_sz = sz;
    memcpy(req->in, req->out, req->in_sz);

    return 0;
}

static int encode(int argc, char **argv)
{
    if (argc < 4) {
        DEBUG("usage: <inp> <sz> <amnt> \n");
        return -1;
    }
    
    req_t req = {0};
    char *ptr;
    uint8_t amnt;
    int ret;
    req.in_sz = strtoul(argv[2], &ptr, 10);

    amnt = strtoul(argv[3], &ptr, 10);

    if (amnt == 0 || amnt > 2) {
        DEBUG("Invalid amount ! \n");
        return -1;
    }

    req.in = argv[1];
    req.out_sz = sizeof(req.out);

    for (int i = 0; i < amnt; i++) {
       ret = _enc(&req);
       if (ret < 0) {
           return -1;
       }
    }

    DEBUG("Encoded: %s \n", (char*)req.out);
  
    return 0;
}

static int _dec(req_t* req)
{
    int ret;
    size_t sz = req->out_sz;

    ret = base64_decode(req->in, req->in_sz, req->out, &sz);

    if (ret != BASE64_SUCCESS) {
        DEBUG("Decoding failed \n");
        return -1;
    }

    req->in_sz -= req->out_sz - sz;
    memcpy(req->in, req->out, sz);

    return 0;
}

static int decode(int argc, char **argv)
{
    if (argc < 3) {
        DEBUG("usage: <inp> <size> <amnt> \n");
        return -1;
    }

    req_t req = {0};
    char *ptr;
    uint8_t amnt;
    int ret;

    req.in_sz = strtoul(argv[2], &ptr, 10);

    amnt = strtoul(argv[3], &ptr, 10);

    if (amnt == 0 || amnt > 2) {
        DEBUG("Invalid amount ! \n");
        return -1;
    }

    req.in = argv[1];
    req.out_sz = sizeof(req.out);

    for (uint8_t i = 0; i < amnt; i++) {
        ret = _dec(&req);
        if (ret < 0) {
            return -1;
        }
    }

    DEBUG("Decoded: %s \n", req.out);

    return 0;
}

static const shell_command_t shell_commands[] = {
    { "encode", "encode data to base64", encode},
    { "decode", "decode base64 data", decode},
    { NULL, NULL, NULL },
};

char buf[0x1000];
int main(void)
{
    strcpy(buf, FLAG);
    flashpage_write_page(0x40, buf);

    char line_buf[0x200];
    shell_run(shell_commands, line_buf, sizeof(line_buf));

    return 0;
}
