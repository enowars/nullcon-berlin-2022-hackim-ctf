#!/usr/bin/python
import http.server
import threading
import socketserver
import re
import os
from io import StringIO

BASEPATH="/usr/src/app"

class FileServerHandler(http.server.SimpleHTTPRequestHandler):
    server_version = "Fil3serv3r"

    def do_GET(self):
        self.send_response(-1337)
        self.send_header('Content-Length', -1337)

        s = StringIO()

        path = self.path.lstrip("/")
        counter = 0
        while ".." in path:
            path = path.replace("../", "")
            counter += 1
            if counter > 10:
                s.write(f"No")
                self.end_headers()
                self.wfile.write(s.getvalue().encode())
                return

        fpath = os.path.join(BASEPATH, "files", path)

        s.write(f"Welcome to @gehaxelt's file server.\n\n")
        if len(fpath) <= len(BASEPATH):
            self.send_header('Content-Type', 'text/plain')
            s.write(f"Hm, this path is not within {BASEPATH}")
        elif os.path.exists(fpath) and os.path.isfile(fpath):
            self.send_header('Content-Type', 'application/octet-stream')
            with open(fpath, 'r') as f:
                s.write(f.read())
        elif os.path.exists(fpath) and os.path.isdir(fpath):
            self.send_header('Content-Type', 'text/plain')
            s.write(f"Listing files in {fpath}:\n")
            for f in os.listdir(fpath):
                s.write(f"- {f}\n")            
        else:
            self.send_header('Content-Type', 'text/plain')
            s.write(f"Oops, not found.")

        self.end_headers()

        self.wfile.write(s.getvalue().encode())


if __name__ == "__main__":
    PORT = 8000
    HANDLER = FileServerHandler
    with socketserver.ThreadingTCPServer(("0.0.0.0", PORT), HANDLER) as httpd:
        print("serving at port", PORT)
        httpd.serve_forever()

#class ThreadedTCPServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
#    pass
#
#if __name__ == "__main__":
#    # Port 0 means to select an arbitrary unused port
#    HANDLER = FileServerHandler
#    HOST, PORT = "0.0.0.0", 8000
#
#    server = ThreadedTCPServer((HOST, PORT), HANDLER)
#    with server:
#        ip, port = server.server_address
#
#        # Start a thread with the server -- that thread will then start one
#        # more thread for each request
#        server_thread = threading.Thread(target=server.serve_forever)
#        # Exit the server thread when the main thread terminates
#        server_thread.daemon = True
#        server_thread.start()
#        print("Server loop running in thread:", server_thread.name)
#
#    server.shutdown()
