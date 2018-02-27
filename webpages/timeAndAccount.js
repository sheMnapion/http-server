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
	var loginInfo=document.getElementById("LoginColumn");
	disptime();
	//readFile();
	var ifram=document.getElementById("iframe1");
	alert(ifram.align);
	var iframContent=ifram.contentDocument.documentElement;
	//alert(iframContent);
	var txtValue=iframContent.innerText;
	//alert(txtValue);
	if(txtValue.length==0){
		loginInfo.innerHTML="";
		loginInfo.innerHTML+="<p>";
		loginInfo.innerHTML+="<a href=/login>login</a> ";
		loginInfo.innerHTML+="<a href=/signup>sign up</a></p>";
	}
	else{
		loginInfo.innerHTML+="<p>You login as <em>"+txtValue+"</em> <a href=\"/logout\">log out</a></p>";
	}
	var findPic=document.getElementById("starNight");
	//alert("Done here with "+findPic);
