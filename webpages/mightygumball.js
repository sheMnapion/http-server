window.onload=function(){
	/*noted for future updates
	var url="sales.json";
	//var url="http://gumball.wickedlysmart.com/gumball/gumball.html";
	var request=new XMLHttpRequest();
	request.open("GET",url);
	alert(request);
	request.onload=function(){
		//alert(request.status);
		//alert(request.responseText);
		if(request.status==200)
			updateSales(request.responseText);
	};
	request.send(null);*/
	setInterval(handleRefresh,3000);
}

function handleRefresh(){
//	alert("Estoy viviendo!");
	var url="http://gumball.wickedlysmart.com/?callback=updateSales";
	var newScriptElement=document.createElement("script");
	newScriptElement.setAttribute("src",url);
	newScriptElement.setAttribute("id","jsonp");

	var oldScriptElement=document.getElementById("jsonp");
	var head=document.getElementsByTagName("head")[0];
	if(oldScriptElement==null){
		head.appendChild(newScriptElement);
	}
	else{
		head.replaceChild(newScriptElement,oldScriptElement);
	}
	//alert("Done handling!\n");
}

function updateSales(sales)
{
	//alert(sales);
	var salesDiv=document.getElementById("sales");
	//var sales=JSON.parse(responseText);
	for(var i=0;i<sales.length;i++){
		var sale=sales[i];
		var div=document.createElement("div");
		div.setAttribute("class","saleItem");
		div.innerHTML=sale.name+" sold "+sale.sales+" gumballs";
		salesDiv.appendChild(div);
	}
}
function displayLuck(luck){
	var p=document.getElementById("sales");
	alert(p);
	p.innerHTML="Today you are "+luck;
}
