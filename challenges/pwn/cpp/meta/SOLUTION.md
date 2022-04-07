## Vuln
* obv buffer overflow when inserting choice
  * enables corruption of the li vtable
* also there us a UAF in the pop function
  * use this to leak libc

## Exploit
* leak libc
* corrupt vtable pointer and point it to a fake vtable
* win
