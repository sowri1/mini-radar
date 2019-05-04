print('hello raja')
from tkinter import *
import time
from math import sin, cos
from serial import Serial
import os, json
from datetime import datetime as dt
from http.server import HTTPServer, BaseHTTPRequestHandler
from multiprocessing import Process

class radar(object):
	def __init__(self):
		self.root = Tk()
		self.canvas = Canvas(self.root, width=800, height = 800, background="black")
		self.canvas.pack()
		self.canvas.create_oval(50,50,750,750, width=2, outline='green')
		self.canvas.create_oval(150,150,650,650, width=2, outline='green')
		self.canvas.create_oval(250,250,550,550, width=2, outline='green')
		self.canvas.create_oval(350,350,450,450, width=2, outline='green')
		self.canvas.create_line(400,0,400,800, width=2, fill="green")
		self.canvas.create_line(0,400,800,400, width=2, fill="green")
		self.canvas.create_text(405,410,fill="white",font="arial 10", text="0")
		self.canvas.create_text(455,410,fill="white",font="arial 10", text="50")
		self.canvas.create_text(555,410,fill="white",font="arial 10", text="100")
		self.canvas.create_text(655,410,fill="white",font="arial 10", text="150")
		self.canvas.create_text(755,410,fill="white",font="arial 10", text="200")
		self.canvas.create_text(55,410,fill="white",font="arial 10", text="-200")
		self.canvas.create_text(155,410,fill="white",font="arial 10", text="-150")
		self.canvas.create_text(255,410,fill="white",font="arial 10", text="-100")
		self.canvas.create_text(355,410,fill="white",font="arial 10", text="-50")
		self.canvas.create_text(410,455,fill="white",font="arial 10", text="-50")
		self.canvas.create_text(410,555,fill="white",font="arial 10", text="-100")
		self.canvas.create_text(410,655,fill="white",font="arial 10", text="-150")
		self.canvas.create_text(410,755,fill="white",font="arial 10", text="-200")
		self.canvas.create_text(410,55,fill="white",font="arial 10", text="200")
		self.canvas.create_text(410,155,fill="white",font="arial 10", text="150")
		self.canvas.create_text(410,255,fill="white",font="arial 10", text="100")
		self.canvas.create_text(410,355,fill="white",font="arial 10", text="50")
		self.canvas.create_rectangle(670,10,800,35, fill='white')
		self.canvas.create_text(700,20,fill="black",font="arial 8", text="Distance = ")
		self.distance1 = self.canvas.create_text(70,20,fill="white",font="arial 10", text="50")
		self.distance2 = self.canvas.create_text(750,20,fill="white",font="arial 10", text="50")
#		self.canvas.create_text(405,410,fill="white",font="arial 10", text="0")
		self.tline1 = self.canvas.create_line(400, 400, 400+350*cos(45*22/(7*180)), 400-350*sin(45*22/(7*180)), width=3, fill="white")
		self.tline2 = self.canvas.create_line(400, 400, 400+350*cos(45*22/(7*180)), 400+350*sin(45*22/(7*180)), width=3, fill="white")
		self.obj1 = None
		self.obj2 = None
		self.canvas.pack()
		self.root.after(0, self.animation)
		self.root.mainloop()

	def animation(self):
		ard = Serial('COM4', 9600)
		while True:
			self.canvas.after(0,self.canvas.delete, self.tline1)
			self.canvas.after(0,self.canvas.delete, self.obj1)
			self.canvas.after(0,self.canvas.delete, self.distance1)
			self.canvas.after(0,self.canvas.delete, self.tline2)
			self.canvas.after(0,self.canvas.delete, self.obj2)
			self.canvas.after(0,self.canvas.delete, self.distance2)
			global data
			data = ard.readline().decode().rstrip('\r\n').split('*')
			if len(data) == 4:
				name = data[0]
				angle = int(data[1])*22/(7*180)
				dist1 = abs(int(data[2]))
				dist2 = abs(int(data[3]))
				# with open(str(name)+".json", 'a+') as fle:
				# 	msg = {'datetime' : str(dt.now()), 'angle' : angle, 'dist1' : dist1, 'dist2' : dist2}
				# 	fle.write(json.dumps(msg))
				# 	fle.write('\n')
				x1, y1 = 400+dist1*cos(angle)*350/200, 400-dist1*sin(angle)*350/200
				x2, y2 = 400-dist2*cos(angle)*350/200, 400+dist2*sin(angle)*350/200
				self.tline1 = self.canvas.create_line(400, 400, 400+350*cos(angle), 400-350*sin(angle), width=3, fill="white")
				self.distance1 = self.canvas.create_text(750,20,fill="black",font="arial 10", text=str(dist1))
#				self.canvas.create_oval(x1-5,y1-5,x1+5,y1+5, fill="orange")
				self.obj1 = self.canvas.create_oval(x1-10,y1-10,x1+10,y1+10, fill="red")
				self.tline2 = self.canvas.create_line(400, 400, 400-350*cos(angle), 400+350*sin(angle), width=3, fill="white")
				self.distance2 = self.canvas.create_text(780,20,fill="black",font="arial 10", text=str(dist2))
#				self.canvas.create_oval(x2-5,y2-5,x2+5,y2+5, fill="orange")
				self.obj2 = self.canvas.create_oval(x2-10,y2-10,x2+10,y2+10, fill="red")
			self.canvas.update()

class SimpleHTTPRequestHandler(BaseHTTPRequestHandler):

	def do_GET(self):
		self.send_response(200)
		self.end_headers()
		global data
		self.wfile.write(str(data).encode())


def runserver():
	port = 8080
	server = HTTPServer(('localhost', port), SimpleHTTPRequestHandler)
	server.serve_forever()


if __name__=="__main__":
	print('hello')
	p2 = Process(target = runserver)
	p1 = Process(target = radar)
	p1.start()
	p2.start()
	# radar()
	# runserver()
