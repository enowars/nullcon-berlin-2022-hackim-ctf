## Vuln
* UAF when opening two file descriptors and freeing one

## Exploit
* UAF seems useless since the data is stored in a dedicated slab cache
and doesn't contain anything useable for RCE
  * notice that slab gets merged with seq_file (`ls -la /sys/kernel/slab/ | grep " :0000112"`
* Use UAF on seq_file to gain rce
  * use `seq_file.from` and `seq_file.count` to leak stuff
  * use `seq_file.op` to get rce and pivot to ROPchain in user space
