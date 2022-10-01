import pygtk
pygtk.require("2.0")
import gtk
import threading
import algorithms
import os

class ProgressArea(gtk.Window):
	def __init__(self,maxvalue=100,theCalculatingThread=None):
		gtk.Window.__init__(self,gtk.WINDOW_TOPLEVEL)
		self.maxvalue=maxvalue
		self.theCalculatingThread=theCalculatingThread
		self.set_title("Computations in progress...")
		self.set_size_request(350,100)
		vbox=gtk.VBox()
		cancelButton=gtk.Button("Too long - terminate whole app")
		cancelButton.connect("clicked",self.cancelation)	
		self.progressbar=gtk.ProgressBar()
		self.progressbar.set_fraction(0)
		vbox.pack_start(self.progressbar)
		vbox.pack_start(cancelButton)
		self.add(vbox)
		self.step=1./maxvalue
		self.connect("destroy_event",self.destroy_window)
		self.progressbar.show()
		cancelButton.show()
		vbox.show()
		self.show()
		self.set_opacity(0.7)
	def inc(self):
		theValue=self.progressbar.get_fraction()
		self.progressbar.set_fraction(theValue+self.step)
	def destroy_window(self,widget,event):
		gtk.main_quit()
		return True
	def cancelation(self,widget):
		self.theCalculatingThread.stop()
		self.hide()
	def setvalue(self,thevalue):
		self.progressbar.set_fraction((1.*thevalue)/(1.*self.maxvalue))


class CalculationsThread(threading.Thread):
	def __init__(self,lstBuffer,width,height):
		threading.Thread.__init__(self)
		maxvalue=height
		self.width=width
		self.height=height
		self.lstBuffer=lstBuffer
		self.progresswindow=ProgressArea(maxvalue,self)
		self.mypid=os.getpid()


	def stop(self):
		#self.stopthread.set()
		#self.join()
		#simple thread closing
		#standard operating system terminology
		os.popen("kill -9 "+str(self.mypid))
