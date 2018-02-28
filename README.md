<h1>This is my http-server of LAC(161220062).</h1>
CRITICAL NOTE: Sometimes the http server just can't answer the request of the explorer. If so, please restart the http-server by make clean and make run for several times and it will finally get connected.

what it can do:
	127.0.0.1:8080/hello show a hello world
	127.0.0.1:8080/bhello show a better hello world written in html5
	127.0.0.1:8080/add/X/Y to carry out adding X and Y
	127.0.0.1:8080/blog enter the main blog where you have link to
		127.0.0.1:8080/login login(though nothing new would appear)
		127.0.0.1:8080/signup sign up a new account
		//and a small change would appear

By Lac Jolime Lee 2018.2.10

added on 2018.2.19
	I mended the bug for restarting. Now when the server starts, you need to wait until the terminal outputs "Bind done after ### times!" to establish reliable connection. Besides you can visit my homepage by visiting 127.0.0.1:8080 directly.
