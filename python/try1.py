from random import randint as rand
from http.server import HTTPServer, BaseHTTPRequestHandler
from multiprocessing import Process


def data_update():
	global data
	data = rand(100, 500)
	print(data)


class SimpleHTTPRequestHandler(BaseHTTPRequestHandler):

	def do_GET(self):
		global data
		self.send_response(200)
		self.end_headers()
		self.wfile.write(str(data).encode())


def runserver():
	port = 8080
	global data
	print(data)
	server = HTTPServer(('localhost', port), SimpleHTTPRequestHandler)
	server.serve_forever()


if __name__=="__main__":
	global data
	data = 0
	p1 = Process(target = data_update)
	p2 = Process(target = runserver)
	p1.start()
	p1.join()
	p2.start()
	p2.join()