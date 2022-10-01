import pygtk
pygtk.require("2.0")
import gtk
import numpy
import statisticsDialog
import algorithms
import string
import PIL.Image
import StringIO
gtk.threads_init()

class ColorsCountDialog(object):
	def __init__(self,caller,quantType):
		self.window=gtk.Window(gtk.WINDOW_TOPLEVEL)
		self.caller=caller
		self.quantType=quantType
		ok_button=gtk.Button("OK")
		cancel_button=gtk.Button("Cancel")
		label=gtk.Label("Quantisation to colors:")
		self.theValueEntry=gtk.Entry()
		self.theValueEntry.set_text("90")
		self.theValueEntry.set_editable(True)
		vbox=gtk.VBox()
		hbox1=gtk.HBox()
		hbox2=gtk.HBox()
		vbox.pack_start(hbox1)
		vbox.pack_start(hbox2)
		hbox1.pack_start(label)	
		hbox1.pack_start(self.theValueEntry)
		hbox2.pack_start(ok_button)
		hbox2.pack_start(cancel_button)
		self.window.add(vbox)
		self.window.set_position(gtk.WIN_POS_CENTER_ALWAYS)
		self.window.connect("destroy",self.destructor)
		cancel_button.connect("clicked",self.destructor)
		ok_button.connect("clicked",self.callback_return)
		label.show()
		self.theValueEntry.show()
		ok_button.show()
		cancel_button.show()
		vbox.show()
		hbox1.show()
		hbox2.show()
		self.window.show()
	def callback_return(self,widget):
		self.theValue=self.theValueEntry.get_text()
		functions={"Popularity":self.caller.popularityQuant_callback,
					"Averaged":self.caller.averagedQuant_callback}
		
		functions[self.quantType](self.theValue)
		self.destructor(None,None)
	def destructor(self,widget,data=None):
		self.window.destroy()
		return True

class ColorsQuantisation(object):
	def __init__(self):
		self.lstBuffer=[]
		#buffer is a string version of the lstBuffer
		#this is a gtk requirement to send immutable
		#to x server
		self.buffer=None
		self.statistics_window=None
		window=gtk.Window(gtk.WINDOW_TOPLEVEL)
		#interface creation
		window.set_title("Colors quantisation")
		window.connect("destroy",lambda w:gtk.main_quit())
		window.set_size_request(400,200)
		self.window=window
		buttonClose=gtk.Button("Close")
		buttonStatistics=gtk.Button("Show statistics")
		buttonSave=gtk.Button("Save image");
		buttonLoad=gtk.Button("Load image")
		buttonPopularity=gtk.Button("Popularity quantisation")
		buttonAveraged=gtk.Button("Averaged quantisation")
		vbox=gtk.VBox(False,0)
		self.table=gtk.Table(2,2,False)
		self.imagearea=gtk.DrawingArea()
		self.vruler=gtk.VRuler()
		self.vruler.set_metric(gtk.PIXELS)
		self.vruler.set_range(0,400,0,400)
		self.table.attach(self.vruler,0,1,1,2,xoptions=0)
		self.hruler=gtk.HRuler()
		self.hruler.set_metric(gtk.PIXELS)
		self.hruler.set_range(0,400,0,400)
		self.table.attach(self.hruler,1,2,0,1,yoptions=0)
		self.table.attach(self.imagearea,1,2,1,2,gtk.EXPAND|gtk.FILL,gtk.FILL,0,0)
		self.scroller=gtk.ScrolledWindow()
		vbox.pack_end(buttonClose,False,False,0)
		vbox.pack_end(buttonStatistics,False,False,0)
		vbox.pack_end(buttonLoad,False,False,0)
		vbox.pack_end(buttonSave,False,False,0)
		vbox.pack_end(buttonPopularity,False,False,0)
		vbox.pack_end(buttonAveraged,False,False,0)
		vbox.pack_end(self.scroller,True,True,0)
		window.add(vbox)
		
		self.imagearea.connect("expose-event",self.imagearea_expose)
		buttonAveraged.connect("clicked",self.doQuant,"Averaged")
		buttonPopularity.connect("clicked",self.doQuant,"Popularity")
		buttonClose.connect("clicked",lambda w:gtk.main_quit())
		buttonStatistics.connect("clicked",self.show_statistics)	
		buttonLoad.connect("clicked",self.load_image)
		buttonSave.connect("clicked",self.save_request)
		self.hruler.show()
		self.vruler.show()
		self.table.show()

		self.imagearea.show()
		self.scroller.show()
		buttonSave.show()
		buttonAveraged.show()
		buttonPopularity.show()
		buttonStatistics.show()
		buttonClose.show()
		buttonLoad.show()
		vbox.show()
		window.show()

	def save_response(self,widget,data=None):
		self.savefilename=self.saveselection.get_filename()
		self.saveselection.destroy()
		self.get_buffer()
		try:
			#self.save_image=PIL.Image.fromstring("RGB",(self.width,self.height),self.buffer)
			self.save_image=PIL.Image.frombytes("RGB",(self.width,self.height),self.buffer,"raw")
		except IOError:
			print "Can't save :("
		try:
			lstSavefilename=self.savefilename.split(".")
			strExtension=lstSavefilename[-1].upper()
			if "JPG"==strExtension or "JPEG"==strExtension:
				self.save_image.save(self.savefilename,"JPEG")
			elif "PNG"==strExtension:
				self.save_image.save(self.savefilename,"PNG")
			else:
				self.save_image.save(self.savefilename,"BMP")
		except IOError:
			print "Can't save :("

	def file_taken(self,widget,data=None):
		self.filename=self.fileselection.get_filename()
		self.fileselection.destroy()
		self.pil_image=PIL.Image.open(self.filename)
		self.width,self.height=self.pil_image.size
		#self.buffer=self.pil_image.tostring()
		self.buffer=self.pil_image.tobytes()
		self.vruler.set_range(0,self.width,0,self.width)
		self.hruler.set_range(0,self.height,0,self.height)
		self.scroller.add_with_viewport(self.table)
		#after the expose event
		self.window.set_size_request(500,500)
		self.imagearea_expose(None,None)	
	def get_lstBuffer(self):
		self.lstBuffer=[]
		for letter in self.buffer:
			self.lstBuffer.append(letter)
	def get_buffer(self):
		del self.buffer
		self.buffer=string.join(self.lstBuffer,"")
	def get_buffer_bytes(self):
		buffer_bytes=[]
		for letter in self.buffer:
			#buffer_bytes.append(str(ord(letter)))
			buffer_bytes.append(letter)
		self.buffer_bytes=string.join(buffer_bytes,"")
	def save_request(self,widget,data=None):
		if self.buffer is None:
			print "No image has been loaded, can't save"
			return
		self.saveselection=gtk.FileSelection("Save a file as...")
		self.saveselection.cancel_button.connect("clicked",lambda w:self.fileselection.destroy())
		self.saveselection.ok_button.connect("clicked",self.save_response)
		self.saveselection.show()
	def load_image(self,widget,data=None):
		self.fileselection=gtk.FileSelection("Load a file...")
		self.fileselection.cancel_button.connect("clicked",lambda w:self.fileselection.destroy())
		self.fileselection.ok_button.connect("clicked",self.file_taken)
		self.fileselection.show()

	def show_statistics(self,widget,data=None):
		if self.buffer is None:
			return
		self.get_lstBuffer()
		self.statistics_window=statisticsDialog.StatisticsWindow(self.lstBuffer,self.width,self.height)
		
	def doQuant(self,widget,data=None):
		if self.statistics_window is not None:
			self.statistics_window.destroyStatistics()
			self.statistics_window=None
		if self.buffer is None:
			print "No image has been loaded, nothing to numbercrunch"
			return
		self.get_lstBuffer()
		self.countDialog=ColorsCountDialog(self,data)
	
	def averagedQuant_callback(self,theValue):
		self.Kcolors=int(theValue)
		averagedThread=algorithms.AveragedThread(self.lstBuffer,self.width,self.height,self,self.Kcolors)
		averagedThread.start()

	def popularityQuant_callback(self,theValue):
		self.Kcolors=int(theValue)
		popularityThread=algorithms.PopularityThread(self.lstBuffer,self.width,self.height,self)
		popularityThread.start()

	def imagearea_expose(self,widget,event):
		self.style=self.imagearea.get_style()
		self.fg_gc=self.style.fg_gc[gtk.STATE_NORMAL]
		self.bg_gc=self.style.bg_gc[gtk.STATE_NORMAL]
		if self.buffer is not None:
			self.imagearea.window.draw_rgb_image(self.fg_gc,0,0,self.width,self.height,gtk.gdk.RGB_DITHER_NONE,self.buffer)

	def main(self):
		gtk.main()
		return 0

if __name__=="__main__":
	colorsQuantisation=ColorsQuantisation()
	colorsQuantisation.main()
