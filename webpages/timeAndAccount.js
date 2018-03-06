	function disptime(){
		var today=new Date();
		var hh=today.getHours();
		var mm=today.getMinutes();
		var ss=today.getSeconds();
		var inerHtml;
		inerHtml="<h3>Present Time: ";
		if(hh<10)
			inerHtml+="0";
		inerHtml+=hh;
		inerHtml+=":";
		if(mm<10)
			inerHtml+="0";
		inerHtml+=mm;
		inerHtml+=":";
		if(ss<10)
			inerHtml+="0";
		inerHtml+=ss;
		inerHtml+="</h3>";
		document.getElementById("myclock").innerHTML=inerHtml;//"<h4>Present Time: "+hh+":"+mm+":"+ss+"</h4>";
	}
	//readFile();
	function loginControl(){
		var loginInfo=document.getElementById("LoginColumn");
		var ifram=document.getElementById("iframe1");
		var iframContent=document.getElementById("iframe1").contentDocument.documentElement;
		var txtValue=iframContent.innerText;
		if(txtValue.length==0){
			loginInfo.innerHTML="";
			loginInfo.innerHTML+="<p>";
			loginInfo.innerHTML+="<a href=/login>login</a> ";
			loginInfo.innerHTML+="<a href=/signup>sign up</a></p>";
		}
		else{
			loginInfo.innerHTML+="<p>You login as <em>"+txtValue+"</em> <a href=\"/logout\">log out</a></p>";
		}
		setInterval(disptime,300);
		var canvas=document.getElementById("lookWhatIDrew");
		if(canvas==null)
			return;
		if(canvas.getContext){
			var context=canvas.getContext("2d");
			context.fillRect(10,10,100,100);
		}
		else{
			alert("Oops, your explorer doesn't support canvas element!");
		}
	}
	
	disptime();
	var findPic=document.getElementById("starNight");
	window.onload=loginControl;
