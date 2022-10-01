import pygtk
pygtk.require("2.0")
import gtk
import algorithms
import numpy
import random
import math

class IntervalChooser(gtk.Table):
	def __init__(self,startReds,startGreens,startBlues,intervalsarea,statisticsarea):
		gtk.Table.__init__(self,2,4,False)
		self.reds=startReds
		self.greens=startGreens
		self.blues=startBlues
		self.intervalsarea=intervalsarea
		self.statisticsarea=statisticsarea
		labelReds=gtk.Label("Reds:")
		labelGreens=gtk.Label("Greens:")
		labelBlues=gtk.Label("Blues:")
		buttonUpdate=gtk.Button("Update")
		buttonUpdate.set_size_request(60,20)
		self.entryReds=gtk.Entry()
		self.entryReds.set_size_request(50,20)
		self.entryGreens=gtk.Entry()
		self.entryGreens.set_size_request(50,20)
		self.entryBlues=gtk.Entry()
		self.entryBlues.set_size_request(50,20)
		self.entryReds.set_text(str(self.reds))
		self.entryGreens.set_text(str(self.greens))
		self.entryBlues.set_text(str(self.blues))

		self.attach(labelReds,0,1,0,1,xoptions=gtk.FILL,yoptions=gtk.FILL)	
		self.attach(labelGreens,1,2,0,1,xoptions=gtk.FILL,yoptions=gtk.FILL)	
		self.attach(labelBlues,2,3,0,1,xoptions=gtk.FILL,yoptions=gtk.FILL)	
		self.attach(self.entryReds,0,1,1,2,xoptions=gtk.FILL,yoptions=gtk.FILL)
		self.attach(self.entryGreens,1,2,1,2,xoptions=gtk.FILL,yoptions=gtk.FILL)
		self.attach(self.entryBlues,2,3,1,2,xoptions=gtk.FILL,yoptions=gtk.FILL)
		self.attach(buttonUpdate,3,4,1,2,xoptions=gtk.FILL,yoptions=gtk.FILL)	

		buttonUpdate.connect("clicked",self.updateIntervals)

		labelReds.show()
		labelGreens.show()
		labelBlues.show()
		self.entryReds.show()
		self.entryGreens.show()
		self.entryBlues.show()
		buttonUpdate.show()
		self.set_size_request(100,50)
		self.show()
	def updateIntervals(self,widget,data=None):
		self.reds=int(self.entryReds.get_text())
		self.greens=int(self.entryGreens.get_text())
		self.blues=int(self.entryBlues.get_text())
		self.intervalsarea.change_intervals(self.reds,self.greens,self.blues)
		self.statisticsarea.change_intervals(self.reds,self.greens,self.blues)
class IntervalsArea(gtk.DrawingArea):
	def __init__(self,reds,greens,blues):
		gtk.DrawingArea.__init__(self)
		self.reds=reds
		self.greens=greens
		self.blues=blues
		self.set_size_request(200,200)	
		self.pangolayout=self.create_pango_layout("")
		self.connect("expose-event",self.drawingarea_expose)
	def drawingarea_expose(self,widget,event):
		self.theStyle=self.get_style()
		self.fg_gc=self.theStyle.fg_gc[gtk.STATE_NORMAL]
		self.bg_gc=self.theStyle.bg_gc[gtk.STATE_NORMAL]
		myRectangle=self.get_allocation()
		self.myWidth=myRectangle.width
		self.myHeight=myRectangle.height
		self.center_point=[2*(self.myWidth/5.),3*(self.myHeight/5.)]
		self.generate_labels()
		self.update_intervals()
	def print_label(self,x,y,colour,index):
		if colour=="Red":
			self.print_text(self.intervals_red[index],x,y)
		if colour=="Green":
			self.print_text(self.intervals_green[index],x,y)
		if colour=="Blue":
			self.print_text(self.intervals_blue[index],x,y)
			
	def print_text(self,txt,x,y):
		self.pangolayout.set_text(txt)
		self.window.draw_layout(self.fg_gc,int(x),int(y),self.pangolayout)
	def generate_labels(self):
		self.intervals_red=["",]*self.reds
		self.intervals_green=["",]*self.greens
		self.intervals_blue=["",]*self.blues
	def change_intervals(self,newReds,newGreens,newBlues):
		self.reds=newReds
		self.greens=newGreens
		self.blues=newBlues
		self.generate_labels()
		self.update_intervals()
	def active_intervals(self,interval):
		blue_val=int(interval/(self.reds*self.greens))
		green_val=int((interval-blue_val*self.reds*self.greens)/self.reds)
		red_val=interval-green_val*self.reds-blue_val*self.reds*self.greens
		self.generate_labels()
		try:
			self.intervals_red[red_val]="x"
			self.intervals_green[green_val]="x"
			self.intervals_blue[blue_val]="x"
		except IndexError:
			pass
		self.update_intervals()
	def clear_screen(self):
		self.window.draw_rectangle(self.bg_gc,True,0,0,self.myWidth,self.myHeight)
	def update_intervals(self):
		self.clear_screen()
		self.drawOX(self.reds)
		self.drawOY(self.greens)
		self.drawOZ(self.blues)
	def drawline(self,x1,y1,x2,y2):
		self.window.draw_line(self.fg_gc,int(x1),int(y1),int(x2),int(y2))
	def drawOX(self,reds):
		self.print_text("R",self.myWidth-15,self.center_point[1]-15)
		self.drawline(self.center_point[0],self.center_point[1],self.myWidth,self.center_point[1])
		linesize=(self.myWidth-self.center_point[0])
		interval=(1.*linesize)/reds
		x=self.center_point[0]+int(interval/2.)
		y=self.center_point[1]
		self.print_label(x,y,"Red",0)
		x+=int(interval/2.)
		for counter in range(0,reds-1):
			self.drawline(x,y-5,x,y+5)
			x+=int(interval/2.)
			self.print_label(x,y,"Red",counter+1)
			x+=int(interval/2.)
	def drawOY(self,greens):
		self.print_text("G",self.center_point[0]+5,5)
		self.drawline(self.center_point[0],self.center_point[1],self.center_point[0],0)
		linesize=(self.center_point[1])
		interval=(1.*linesize)/greens
		x=self.center_point[0]
		y=self.center_point[1]-int(interval/2.)
		self.print_label(x+5,y,"Green",0)
		y-=int(interval/2.)
		for counter in range(0,greens-1):
			self.drawline(x-5,y,x+5,y)
			y-=int(interval/2.)
			self.print_label(x+5,y,"Green",counter+1)
			y-=int(interval/2.)
	def drawOZ(self,blues):
		self.print_text("B",5,self.myHeight-30)
		self.drawline(self.center_point[0],self.center_point[1],0,self.myHeight)
		linesize=math.sqrt((1.*self.center_point[0])**2+(self.myHeight-self.center_point[1])**2)
		interval=(1.*linesize)/blues
		x=self.center_point[0]
		y=self.center_point[1]
		sin_alpha=(self.center_point[0])/(1.*linesize)
		cos_alpha=(self.myHeight-self.center_point[1])/(1.*linesize)
		dx,dy=int(5.*cos_alpha),int(5.*sin_alpha)
		lx,ly=interval*sin_alpha,interval*cos_alpha
		x-=int(lx/2.)
		y+=int(ly/2.)

		self.print_label(x+5,y,"Blue",0)
		x-=int(lx/2.)
		y+=int(ly/2.)
		for counter in range(0,blues-1):
			self.drawline(x-dx,y-dy,x+dx,y+dy)
			x-=int(lx/2.)
			y+=int(ly/2.)
			self.print_label(x+5,y,"Blue",counter+1)
			x-=int(lx/2.)
			y+=int(ly/2.)

class StatisticsArea(gtk.Table):
	def __init__(self,lstBuffer,lstBufferWidth,lstBufferHeight,reds,greens,blues,intervalsarea):
		gtk.Table.__init__(self,1,2,False)	
		self.lstBuffer=lstBuffer
		self.lstBufferWidth=lstBufferWidth
		self.lstBufferHeight=lstBufferHeight
		self.intervalsarea=intervalsarea
		self.reds=reds
		self.greens=greens
		self.blues=blues
		self.list=None
		self.txt_space=None
		self.drawingarea=gtk.DrawingArea()
		self.pangolayout=self.drawingarea.create_pango_layout("")
		self.set_size_request(200,800)
		self.my_v_ruler=gtk.VRuler()
		self.my_v_ruler.set_metric(gtk.PIXELS)
		self.set_size_request(700,200)
		self.my_v_ruler.set_range(100,0,100,0)
		self.attach(self.my_v_ruler,0,1,0,1,xoptions=0)
		self.attach(self.drawingarea,1,2,0,1)
		self.my_v_ruler.show()
		self.drawingarea.set_events(gtk.gdk.POINTER_MOTION_MASK)
		self.drawingarea.connect("motion_notify_event",self.mousemove)
		self.drawingarea.connect("expose-event",self.drawingarea_expose)
		self.drawingarea.show()
			
		self.pangolayout=self.drawingarea.create_pango_layout("")
		self.generate_list()
	def mousemove(self,widget,event):
		if (self.txt_space is not None)and(self.list is not None):
			theValue=int((event.x-self.txt_space/2.)/self.txt_space)
			if theValue>len(self.list):
				theValue=len(self.list)
			self.intervalsarea.active_intervals(theValue)
	def generate_list_request(self):
		self.list=None
		
		statisticsThread=algorithms.StatisticsThread(self.lstBuffer,self.lstBufferWidth,self.lstBufferHeight,self.reds,self.greens,self.blues,self)
		statisticsThread.start()

	def clear_screen(self):
		self.drawingarea.window.draw_rectangle(self.bg_gc,True,0,0,self.Xrange,self.Yrange)
	def change_intervals(self,newReds,newGreens,newBlues):
		self.reds=newReds
		self.greens=newGreens
		self.blues=newBlues
		self.generate_label_list([self.reds,self.greens,self.blues])
		self.list=None
		self.clear_screen()
		self.update_statistics()
	def generate_list_response(self):
		self.update_statistics()	

	def generate_list(self,flag=None):
		if flag is not None:
			self.generate_list_request()

	def generate_list_old(self,flag=None):
		if flag is not None:
			statisticsThread=algorithms.StatisticsThread(self.imgarray,self.reds,self.greens,self.blues,self)
			statisticsThread.start()
			statisticsThread.join()
	def update_statistics(self):
		if self.list is None:
			self.draw_0xline(None)
		else:
			self.draw_0xline([self.reds,self.greens,self.blues])


	def line(self,x1,y1,x2,y2):
		self.drawingarea.window.draw_line(self.fg_gc,x1,y1,x2,y2)

	def draw_0xline(self,intervals):
		myRectangle=self.drawingarea.get_allocation()
		self.startX=5
		self.Xrange=myRectangle.width
		self.Yrange=myRectangle.height
		self.startY=myRectangle.height-5
		self.endX=myRectangle.width-5
		self.endY=self.startY
		self.line(self.startX,self.startY,self.endX,self.endY)
		if intervals is None:
			return
		self.label_list=self.generate_label_list(intervals)
		reds,greens,blues=intervals
		self.txt_space=(self.endX-self.startX)/len(self.label_list)
		x=self.startX
		y=self.startY-15
		counter=0	
		for element in self.label_list:
			self.draw_scale_segment(x,y)
			self.draw_statistics_bar(x,y,self.list[counter],self.maxcolor)
			x+=self.txt_space/2
			self.print_text(str(element),x,y)
			x+=self.txt_space/2
			counter+=1

	def generate_label_list(self,intervals):
		returnlist=[]
		counter=0
		for b in range(0,intervals[2]):
			for g in range(0,intervals[1]):
				for r in range(0,intervals[0]):
					counter+=1
					returnlist.append(counter)
		return returnlist

	def print_text(self,txt,x,y):
		self.pangolayout.set_text(txt)
		self.drawingarea.window.draw_layout(self.fg_gc,x,y,self.pangolayout)

	def draw_scale_segment(self,x,y):
		self.drawingarea.window.draw_line(self.fg_gc,x+5,y-10,x+5,y+15)

	def draw_statistics_bar(self,x,y,bar_value,max_value):
		multiplier=(1.*bar_value)/(1.*max_value)
		#-40 the space between the line and bars and windowboarders
		scaled_value=int((self.Yrange-40)*multiplier)
		self.drawingarea.window.draw_rectangle(self.fg_gc,True,x+10,y-15-scaled_value,self.txt_space/2,scaled_value)

	def drawingarea_expose(self,widget,event):
		myRectangle=self.drawingarea.get_allocation()
		self.Xrange=myRectangle.width
		self.Yrange=myRectangle.height
		self.theStyle=self.drawingarea.get_style()
		self.fg_gc=self.theStyle.fg_gc[gtk.STATE_NORMAL]
		self.bg_gc=self.theStyle.bg_gc[gtk.STATE_NORMAL]
		self.update_statistics()

class StatisticsWindow(object):
	def __init__(self,lstBuffer,width,height):
		window=gtk.Window(gtk.WINDOW_TOPLEVEL)
		window.set_title("Image statistics")
		reds=greens=blues=3		
		self.intervalsarea=IntervalsArea(3,3,3)
		self.statisticsarea=StatisticsArea(lstBuffer,width,height,reds,greens,blues,self.intervalsarea)
		self.intervalchooser=IntervalChooser(3,3,3,self.intervalsarea,self.statisticsarea)

		buttonCalculate=gtk.Button("Calculate statistics")
		vbox=gtk.VBox(False)
		vbox.pack_end(buttonCalculate,False,False)
		vbox.pack_end(self.statisticsarea,True,True)

		hbox=gtk.HBox(False)
		
		hbox.pack_start(vbox)	
		vbox2=gtk.VBox(False)
		vbox2.pack_start(self.intervalsarea,False)
		vbox2.pack_start(self.intervalchooser,False)
		hbox.pack_start(vbox2,False)
		window.add(hbox)

		window.connect("destroy_event",self.closeStatistics)
		buttonCalculate.connect("clicked",self.obtain_statistics)

		self.statisticsarea.show()
		buttonCalculate.show()
		vbox.show()
		vbox2.show()
		self.intervalsarea.show()
		hbox.show()
		window.show()
		self.window=window
		self.window.set_size_request(800,100)
	def destroyStatistics(self):
		self.window.destroy()
	def closeStatistics(self,widget,event,data=None):
		gtk.main_quit()
		return True

	def obtain_statistics(self,widget,data=None):
		self.statisticsarea.generate_list(True)
		self.statisticsarea.update_statistics()

	def main(self):

		gtk.main()
		return 0



if __name__=="__main__":
	statistics=StatisticsWindow()
	statistics.main()
