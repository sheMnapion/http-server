import sys,socket

def validPath(path):
	"Return the valid path asked by path"
	if path=='/':
		return '/webpages/index.html'
	else:
		return path

def readFile(path):
	"Return the file pointed by path"
	try:
		f=open(path,"r")
	except IOError, e:
		print "[Errno 2]", e
		path="/webpages/noFile.html"
		f=open(path,"r")
	ret=''
	for line in f:
		ret+=line
	return ret
sock=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
sock.bind(('127.0.0.1',8080))
sock.listen(1)
sock.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)

while True:
	conn, _ =sock.accept()
	data=conn.recv(1024)
	reply='<h1>It works!</h1>'
	print 'Received data is ', data
	line=data.split('\r\n')
	print 'Line: ', line
	resource=line[0]
	res=resource.split(' ')
	print res
	print validPath(res[1])
	response='\n'.join([
		'HTTP/1.0 200 OK',
		'Content-Type: text/html; charset: utf-8',
		'Content-Length: {0}'.format(len(reply)),
		'',
		readFile(validPath(res[1]))])
	conn.sendall(response)
	conn.close()
