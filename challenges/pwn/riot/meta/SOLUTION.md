## Vuln
* rounding error in base64 decoding func

## Exploit
* craft a base64 payload that results in an overflow into the out_sz field
* next base64 decode (when using amount 2) will cause an buffer overflow
* simply print flag based on the static flash address
