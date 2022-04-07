#!/usr/bin/python
import http.server
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
		while ".." in path:
			path = path.replace("../", "")
		print(path)
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
	with socketserver.TCPServer(("0.0.0.0", PORT), HANDLER) as httpd:
	    print("serving at port", PORT)
	    httpd.serve_forever()