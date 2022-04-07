## Vulnerability
* It is possible to transfer money to a deleted account
* This opens the possibility to corrupt tcache fd pointers
* Also the loan function allows for very big loans being given by inserting
  a negative number. Negative int will be cast to int64_t resulting in positive
  amount added

## Exploit
* Use the transfer vuln to corrupt the size field of a chunk and free
it into the unsorted bin.
  * house of spirit
  * => libc leak
* Use the fact that one get get a big loan by putting in a negative number
and get a big loan
* Transfer a specific amount to corrupt the fd pointer of a freed cache to 
 free_hook
* win
