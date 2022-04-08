#!/usr/bin/env python3
"""
ENOJAIL
"""
import re, sys, signal, datetime
from subprocess import Popen, PIPE
from functools import partial
from socketserver import ForkingTCPServer, BaseRequestHandler

PORT = 5656
REGEX = r"p*[^ &\--=@-P'_-m]*,*"


class RequestHandler(BaseRequestHandler):
    def handle(self):
        print(
            "{}: session for {} started".format(
                datetime.datetime.now(), self.client_address[0]
            )
        )
        fd = self.request.makefile("rwb", buffering=0)
        main(fd, fd, bytes=True)


def main(f_in=sys.stdin, f_out=sys.stdout, bytes=False):
    def enc(str):
        if bytes:
            return str.encode()
        return str

    def decode(b):
        if bytes:
            return b.decode()
        return b

    def alarm_handler(signum, frame):
        f_out.write(enc("\nThank you for your visit.\nPlease come back soon. :)\n"))
        print("{}: Another timeout reached.".format(datetime.datetime.now()))
        sys.exit(15)

    if "debug" not in sys.argv:
        signal.signal(signal.SIGALRM, alarm_handler)
        signal.alarm(15)

    f_out_no = f_out.fileno()

    r = REGEX
    cat_food = partial(re.compile(r).sub, "")
    proc = Popen(
        ["python3", "-u", "-m", "IPython", "--HistoryManager.enabled=False"],
        stdin=PIPE,
        stdout=f_out_no,
        stderr=f_out_no,
    )
    si = proc.stdin

    f_out.write(
        f"""Welcome to the ENOJAIL™ interactive IPython experience!
You can IPython all you want.
Just please don't look at ./flag.txt, thanks!

Oh, actually, we will filter out this regex:
```
{REGEX}
```
just to be on the safe side.

Enjoy your stay! :)\n\n""".encode()
    )

    while True:
        userinput = cat_food(decode(f_in.readline())).strip()
        # forward "sanitized" input to IPython
        si.write("{}\n".format(userinput).encode())
        si.flush()


if __name__ == "__main__":
    print("Listening on port {}".format(PORT))
    ForkingTCPServer(("0.0.0.0", PORT), RequestHandler).serve_forever()
