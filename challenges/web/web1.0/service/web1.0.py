#!/usr/bin/env python3
import os
from http.server import ThreadingHTTPServer, CGIHTTPRequestHandler

PORT = 9001


def setup():
    os.system(
        "mkdir -p ../data/cookies ../data/priority_msgs ../data/msgs ../data/users"
    )
    if (
        "ADMIN_PASS_HASH" not in os.environ
        or "RESET_PASS" not in os.environ
        or "FLAG" not in os.environ
    ):
        raise ValueError("Necessary env variables not set")

    with open("../data/users/admin", "w") as f:
        f.write(
            f"""name=admin
pass_hash={os.environ["ADMIN_PASS_HASH"]}
msgs=/admin"""
        )
    with open("../data/msgs/admin", "w") as f:
        f.write(os.environ["FLAG"])


def serve():
    print(f"serving on http://127.0.0.1:{PORT}")
    ThreadingHTTPServer(("0.0.0.0", PORT), CGIHTTPRequestHandler).serve_forever()


if __name__ == "__main__":
    setup()
    serve()
