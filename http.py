import sys,socket

addr='127.0.0.1'
def validPath(path):
	"Return the valid path asked by path"
	if path=='/':
		return 'webpages/index.html'
	else:
		return path[1:]

def readFile(path):
	"Return the file pointed by path"
	try:
		f=open(path,"r")
	except IOError, e:
		print "[Errno 2]", e
		path="webpages/noFile.html"
		f=open(path,"r")
	ret=''
	for line in f:
		ret+=line
	return ret

def contentType(fileName):
	"Return the standard contentType description here"
	print "Filename: ",fileName
	fileParse=fileName.split('/')
	types={'jpg':'image/jpg',
		'jpeg':'image/jpeg',
		'png':'image/png',
		'ico':'image/ico',
		'gif':'image/gif',
		'html':'text/html',
		'css':'text/css',
		'js':'application/js',
		'json':'application/json',
		'pdf':'application/pdf',
		'pptx':'application/x-ppt'}
	filepos=fileParse[-1].split('.')
	for posa in filepos:
		if posa in types:
			return types.get(posa)
	return 'text/html'
sock=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
sock.bind((addr,8080))
sock.listen(1)
sock.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)

while True:
	conn, _ =sock.accept()
	data=conn.recv(1024)
	print 'Received data has length ',len(data), data
	if(len(data)>0):
		line=data.split('\r\n')
		print 'Line: ', line
		resource=line[0]
		res=resource.split(' ')
		print res
		path=validPath(res[1])
		reply=readFile(path)
		print contentType(path)
		print len(reply)
		response='\n'.join([
			'HTTP/1.0 200 OK',
			'Content-Type: {0}'.format(contentType(path)),
			'Content-Length: {0}'.format(len(reply)),
			'',
			reply])
		conn.sendall(response)
	conn.close()
