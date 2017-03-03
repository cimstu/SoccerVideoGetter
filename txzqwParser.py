import HTMLParser
import codec

class Status:
	initStatus = 0
	inTd = 1
	inImg = 2
	inH3 = 3
	inA = 4
	inB = 5
	inFont = 6
class TxzqwParser(HTMLParser):
	def __init_(self):
		self.currStatus = Status.initStatus
		self.currUrl = ""
		self.currName = ""
		
	def handle_starttag(self, tag, attrs):
		if tag == "td": currStatus = Status.inTd 
		else: currStatus = Status.initStatus
		
		if tag == "img" and currStatus == Status.inTd: currStatus = Status.inImg
		else: currStatus = Status.initStatus
				
		if tag == "h3" and currStatus == Status.inImg: currStatus = Status.inH3
		else: currStatus = Status.initStatus
		
		if tag == "a" and currStatus == Status.inH3: currStatus = Status.inA
		else: currStatus = Status.initStatus
		
		if tag == "b" and currStatus == Status.inA: currStatus = Status.inB
		else: currStatus = Status.initStatus
		
		if tag == "font" and currStatus == Status.inB: currStatus = Status.inFont
		else: currStatus = Status.initStatus
		
	def handle_data(self, data):
		if currStatus == Status.inA:
			currUrl = data
		if currStatus == inFont
			currName = data
		
fileobj = codec.open("txzqw-home.html", "r", "utf-8")
filestream = fileobj.read()
self.feed(filestream)