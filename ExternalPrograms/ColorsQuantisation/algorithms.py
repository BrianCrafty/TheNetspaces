from decimal import *
import threading
import pygtk
pygtk.require("2.0")
import gtk
import os
import calculationsThread
import operator
gtk.threads_init()

class AlgorithmXCP(Exception):
	def __init__(self,value):
		self.value=value
	def __str__(self):
		return repr(self.value)

class ColorStructure:
	def __init__(self,r,g,b):
		self.r=r
		self.g=g
		self.b=b
		self.count=1
	def __cmp__(self,other):
		if self.count<other.count:
			return -1
		elif self.count==other.count:
			return 0
		else: return 1
	def __repr__(self):
		return "(%d , %d, %d | %d)" %(self.r,self.g,self.b,self.count)

class AveragedThread(calculationsThread.CalculationsThread):
	class Box:
		def __init__(self,rstart,rend,gstart,gend,bstart,bend):
			self.rstart=rstart
			self.rend=rend
			self.gstart=gstart
			self.gend=gend
			self.bstart=bstart
			self.bend=bend
			self.myColors=[]
			self.pixelsAmount=0
			self.averageColor=ColorStructure(0,0,0)
		def __repr__(self):
			theString="[rstart: %d, rend: %d, gstart: %d, gend: %d, bstart: %d, bend: %d | pixelsAmount: %d]" %(self.rstart,self.rend,self.gstart,self.gend,self.bstart,self.bend,self.pixelsAmount)
			return theString
	def __init__(self,lstBuffer,width, height, caller, Kcolors):
		calculationsThread.CalculationsThread.__init__(self, lstBuffer, width, height)
		self.caller=caller
		self.Kcolors=Kcolors
		#the whole RGB cube as beggining of the division
		self.boxes=[AveragedThread.Box(0,255,0,255,0,255)]
	
	def make_statistics(self,usedColors):
		colorlist=[0,]*(256**3)
		counter=0
		for y in range(0,self.height):
			for x in range(0,self.width):
				counter+=1
				red=ord(self.lstBuffer[3*self.height*x+3*y])
				green=ord(self.lstBuffer[3*self.height*x+3*y+1])
				blue=ord(self.lstBuffer[3*self.height*x+3*y+2])

				#not definig a function, using inline in order to speed up
				colorvalue=red+256*green+256*256*blue
				if colorlist[colorvalue]==0:
					usedColors.append(ColorStructure(red,green,blue))
				colorlist[colorvalue]+=1
			self.progresswindow.inc()
			#return counter
			#avoiding return because of the multithreading issues
			#don't want to implement reference scheme (pixelsAmount=[value])
			#in one object
			self.boxes[0].pixelsAmount=counter
		for theCS in usedColors:
			theCS.count=colorlist[theCS.r+256*theCS.g+256*256*theCS.b]
		usedColors.sort()
	def select_box_to_divide(self):
		lastMax=0
		for theBox in self.boxes:
			if theBox.pixelsAmount>lastMax:
				lastMax=theBox.pixelsAmount
				lastBox=theBox
		return lastBox
	def getDivisionEdge(self,theBox):
		#selecting the longest edge
		theLongestValue=0
		value=theBox.rend-theBox.rstart
		if value>theLongestValue:
			theLongestValue=value
			chosen="red"
		value=theBox.gend-theBox.gstart
		if value>theLongestValue:
			theLongestValue=value
			chosen="green"
		value=theBox.bend-theBox.bstart
		if value>theLongestValue:
			theLongestValue=value
			chosen="blue"
		if value==0:
			raise AlgorithmXCP
		return chosen
	def divideRed(self,theBox):
		print "divideRed: ",theBox
		i=1
		first=self.Box(theBox.rstart,theBox.rstart+1,theBox.gstart,theBox.gend,theBox.bstart,theBox.bend)
		while first.pixelsAmount<(theBox.pixelsAmount/2):
			i+=1
			if (theBox.rend-theBox.rstart)<2:
				print "red xcp"
				raise AlgorithmXCP("(theBox.rend-theBox.rstart)<2");
			if i>(theBox.rend-theBox.rstart):
				print "red xcp"
				raise AlgoritmXCP("i>(theBox.rend-theBox.rstart)");
			first=self.Box(theBox.rstart,theBox.rstart+i,theBox.gstart, theBox.gend, theBox.bstart, theBox.bend)
			for colorstructure in theBox.myColors:
				if colorstructure.r<=first.rend:
					first.myColors.append(colorstructure)
					first.pixelsAmount+=colorstructure.count
		second = self.Box(theBox.rstart+i+1,theBox.rend,theBox.gstart,theBox.gend,theBox.bstart,theBox.bend)
		for colorstructure in theBox.myColors:
			if colorstructure.r>=second.rstart:
				second.myColors.append(colorstructure)
				second.pixelsAmount+=colorstructure.count
		if second.pixelsAmount==0:
			print "red: second.pixelsAmount==0"
			raise AlgorithmXCP("red: second.pixelAmount==0")
		self.replace(theBox, first, second)

	def divideGreen(self,theBox):
		print "divideGreen: ", theBox
		i=1
		first=self.Box(theBox.rstart,theBox.rend,theBox.gstart,theBox.gstart+1,theBox.bstart,theBox.bend)
		while first.pixelsAmount<(theBox.pixelsAmount/2):
			i+=1
			if (theBox.gend-theBox.gstart)<2:
				print "green xcp"
				raise AlgorithmXCP("(theBox.rend-theBox.rstart)<2");
			if i>(theBox.gend-theBox.gstart):
				print "green xcp"
				raise AlgoritmXCP("i>(theBox.rend-theBox.rstart)");
			first=self.Box(theBox.rstart,theBox.rend,theBox.gstart,theBox.gstart+i,theBox.bstart,theBox.bend)
			for colorstructure in theBox.myColors:
				if colorstructure.r<=first.gend:
					first.myColors.append(colorstructure)
					first.pixelsAmount+=colorstructure.count
		second =self.Box(theBox.rstart,theBox.rend,theBox.gstart+i+1,theBox.gend,theBox.bstart,theBox.bend)
		for colorstructure in theBox.myColors:
			if colorstructure.r>=second.gstart:
				second.myColors.append(colorstructure)
				second.pixelsAmount+=colorstructure.count
		if second.pixelsAmount==0:
			print "green: second.pixelsAmount==0"
			raise AlgorithmXCP("green: second.pixelAmount==0")
		self.replace(theBox, first, second)

	def divideBlue(self,theBox):
		print "divideBlue: ", theBox
		i=1
		first=self.Box(theBox.rstart,theBox.rend,theBox.gstart,theBox.gend,theBox.bstart,theBox.bstart+i)
		while first.pixelsAmount<(theBox.pixelsAmount/2):
			i+=1
			if (theBox.bend-theBox.bstart)<2:
				print "blue xcp"
				raise AlgorithmXCP("(theBox.rend-theBox.rstart)<2");
			if i>(theBox.bend-theBox.bstart):
				print "blue xcp"
				raise AlgoritmXCP("i>(theBox.rend-theBox.rstart)");
			first=self.Box(theBox.rstart,theBox.rend,theBox.gstart,theBox.gend,theBox.bstart,theBox.bstart+i)
			for colorstructure in theBox.myColors:
				if colorstructure.r<=first.bend:
					first.myColors.append(colorstructure)
					first.pixelsAmount+=colorstructure.count
		second=self.Box(theBox.rstart,theBox.rend,theBox.gstart,theBox.gend,theBox.bstart+i+1,theBox.bend)
		for colorstructure in theBox.myColors:
			if colorstructure.r>=second.bstart:
				second.myColors.append(colorstructure)
				second.pixelsAmount+=colorstructure.count
		if second.pixelsAmount==0:
			print "blue: second.pixelsAmount==0"
			raise AlgorithmXCP("blue: second.pixelAmount==0")
		self.replace(theBox,first,second)
	def replace(self,toRemove,boxFirst,boxSecond):
		self.boxes.remove(toRemove)
		self.boxes.append(boxFirst)
		self.boxes.append(boxSecond)
	def averageColors(self):
		self.averages=[]
		for theBox in self.boxes:
			theSumR,theSumG,theSumB=0,0,0
			for theColor in theBox.myColors:
				theSumR+=theColor.r
				theSumG+=theColor.g
				theSumB+=theColor.b
			ar=theSumR/theBox.pixelsAmount
			ag=theSumG/theBox.pixelsAmount
			ab=theSumB/theBox.pixelsAmount
			av=[ar,ag,ab]
			if av not in self.averages:
				self.averages.append(av)
	def quantBuff(self):
		counter=0
		self.progresswindow.maxvalue=self.height
		for y in range(0,self.height):
			self.progresswindow.inc()
			for x in range(0,self.width):
				counter+=1
				r=ord(self.lstBuffer[3*self.height*x+3*y])
				g=ord(self.lstBuffer[3*self.height*x+3*y+1])
				b=ord(self.lstBuffer[3*self.height*x+3*y+2])
				minValue=3*(255**2) #the biggest possible 
				for theColor in self.averages:
					#Euklid metric on the RGB cube
					metric=(r-theColor[0])**2+(g-theColor[1])**2+(b-theColor[2])**2
					if metric<minValue:
						minValue=metric
						quantColor=theColor
				#quantColor=self.getBestApprox(red,green,blue)
				self.lstBuffer[3*self.height*x+3*y]=chr(quantColor[0])
				self.lstBuffer[3*self.height*x+3*y+1]=chr(quantColor[1])
				self.lstBuffer[3*self.height*x+3*y+2]=chr(quantColor[2])

	def run(self):
		gtk.threads_enter()
		print "Hello from the AveragedThread, Kcolors: ",self.Kcolors
		self.make_statistics(self.boxes[0].myColors)
		self.progresswindow.setvalue(0)
		print "List of the used colors length: ", len(self.boxes[0].myColors)
		print "self.boxes[0].pixelsAmount: ",self.boxes[0].pixelsAmount
		self.progresswindow.setvalue(0)
		self.progresswindow.maxvalue=self.Kcolors
		intLength=1
		try:
			while intLength<self.Kcolors:
				intLength=len(self.boxes)
				self.progresswindow.setvalue(intLength)
				d=self.select_box_to_divide()
				function={"red":self.divideRed,
					"green":self.divideGreen,
					"blue":self.divideBlue}
				function[self.getDivisionEdge(d)](d)
		except:
			print "Quantisation into more than ",intLength, " is not possible. Results of quantisation into ",intLength, "colors"
		print "Have division, averaging colors in boxes"
		self.averageColors()	
		print "Quanting the buffer"
		self.progresswindow.setvalue(0)
		self.quantBuff()
		self.caller.get_buffer()
		self.caller.imagearea_expose(None, None)
		print "Quantisation done"
		self.progresswindow.hide()
		gtk.threads_leave()


class PopularityThread(calculationsThread.CalculationsThread):
	def __init__(self,lstBuffer,width,height,caller):
		calculationsThread.CalculationsThread.__init__(self,lstBuffer,width,height)
		self.caller=caller

	def run(self):
		gtk.threads_enter()
		colorlist=[0,]*(256**3)
		usedColors=[]
		counter=0
		for y in range(0,self.height):
			for x in range(0,self.width):
				counter+=1
				red=ord(self.lstBuffer[3*self.height*x+3*y])
				green=ord(self.lstBuffer[3*self.height*x+3*y+1])
				blue=ord(self.lstBuffer[3*self.height*x+3*y+2])

				#not definig a function, using inline in order to speed up
				colorvalue=red+256*green+256*256*blue
				if colorlist[colorvalue]==0:
					usedColors.append(ColorStructure(red,green,blue))
				colorlist[colorvalue]+=1
			self.progresswindow.inc()
		for theCS in usedColors:
			theCS.count=colorlist[theCS.r+256*theCS.g+256*256*theCS.b]
		usedColors.sort()
		self.newColors=usedColors[len(usedColors)-self.caller.Kcolors:]
				

		print "Kcolors: ",self.caller.Kcolors
		print self.newColors
		print "len(self.newColors): ", len(self.newColors)	
		print "Computed the most apriopriate colors, transforming into the image"
		self.progresswindow.setvalue(0)
		for y in range(0,self.height):
			for x in range(0,self.width):
				self.progresswindow.inc()
				red=ord(self.lstBuffer[3*self.height*x+3*y])
				green=ord(self.lstBuffer[3*self.height*x+3*y+1])
				blue=ord(self.lstBuffer[3*self.height*x+3*y+2])
				theNewColor=self.getBestApprox(red,green,blue,self.newColors)

				self.lstBuffer[3*self.height*x+3*y]=chr(theNewColor.r)
				self.lstBuffer[3*self.height*x+3*y+1]=chr(theNewColor.g)
				self.lstBuffer[3*self.height*x+3*y+2]=chr(theNewColor.b)
		self.caller.get_buffer()
		self.caller.imagearea_expose(None,None)
		print "Quantisation done"
		self.progresswindow.hide()
		gtk.threads_leave()
	def getBestApprox(self,r,g,b,newColors):
		minValue=256**3 #the biggest possible
		for theColor in newColors:
			#Euklid metric on the RGB cube
			metric=(r-theColor.r)**2+(g-theColor.g)**2+(b-theColor.b)**2
			if metric<minValue:
				minValue=metric
				returnColor=theColor
		return returnColor


class StatisticsThread(calculationsThread.CalculationsThread):
	def __init__(self,lstBuffer,width,height,reds,greens,blues,caller):
		calculationsThread.CalculationsThread.__init__(self,lstBuffer,width,height)
		self.reds=reds
		self.greens=greens
		self.blues=blues
		self.caller=caller
		#self.stopthread=threading.Event()

	def run(self):	
		pixels_total=0
		returnlist=[0,]*(self.reds*self.greens*self.blues)
		retindex=0
		gtk.threads_enter()
		for y in range(1,self.height):
			for x in range(1,self.width):
				pixels_total+=1
				redval=ord(self.lstBuffer[3*self.height*x+3*y])
				greenval=ord(self.lstBuffer[3*self.height*x+3*y+1])
				blueval=ord(self.lstBuffer[3*self.height*x+3*y+2])
				#rounding to the interval
				
				red_interval=int(self.reds*(redval/255.))
				blue_interval=int(self.blues*(blueval/255.))
				green_interval=int(self.greens*(greenval/255.))
				#problem only for colours ==255	
				if red_interval>2: red_interval=2
				if blue_interval>2: blue_interval=2
				if green_interval>2: green_interval=2
				retindex=red_interval+self.reds*green_interval+self.reds*self.greens*blue_interval
				returnlist[retindex]+=1
			self.progresswindow.inc()
			#self.stopthread.wait(0.01)
		self.caller.maxcolor=pixels_total
		self.caller.list=returnlist
		self.caller.drawingarea_expose(None,None)
		self.progresswindow.hide()
		self.caller.generate_list_response()
		gtk.threads_leave()
