# web1.0

Welcome to the web1.0.

## Public Challenge Description.

Welcome to the interweb!

We heared of this new thing called "the interweb" (or web for short) and promptly created a State-of-the-Art web server that allows you to store data _ON_ our server!
Say whaaaaaat!
Also, it leverages most modern web technologies to make this thing bullet proof.
Even I, the admin, use it to store my secrets.

## Secret Design Specification.

web1.0 is a web challenge that breaks the HTTP protocol a bit.
It's written in C and players get the (obscure gcc89) source code.
With C preprocessor as templating language, etc.
As the service uses ASAN and FORTIFY_SOURCE, it does not provide an RCE, _even though_ it has a range of buffer overflows and even uses `gets` to read network input.

The hope here is that players learn about old-skool C, CGI, and side channels, even though their first instinct wants to PWN.
Oh and it supports modern web security mechanisms, such as CRSF tokens and CSP nonces, plus there is a range of fun red herings and memes in this.
