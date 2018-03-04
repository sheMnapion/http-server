window.onload=getMyLocation;
function getMyLocation(){
	if(navigator.geolocation){
		navigator.geolocation.getCurrentPosition(displayLocation,displayError);
	}
	else{
		alert("Oops, no geolocation support.");
	}
	alert("Done in getMyLocation");
}

function displayLocation(position){
	alert("HERE in displayLocation"+position.coords.latitude);
	var latitude=position.coords.latitude;
	var longitude=position.coords.longitude;

	var div=document.getElementById("location");
	div.innerHTML="You are at Latitude: "+latitude+", Longitude: "+longitude;
	div.innerHTML+=" (with "+position.coords.accuracy+" meters accuracy)";
	var ourCoords={
		latitude: 47.624851,
		longitude: -122.52099
	};

	var dis=document.getElementById("distance");
	var km=computeDistance(position.coords,ourCoords);
	dis.innerHTML="You are "+km+" km from the WickedlySmart HQ";
}

function displayError(error)
{
	alert("Here in displaying error:"+error.code);
	var errorTypes={
		0: "Unknown error",
		1: "Permission denied by user",
		2: "Position is not available",
		3: "Request timed out"
	};
	var errorMessage=errorTypes[error.code];
	if(error.code==0||error.code==2)
		errorMessage=errorMessage+" "+error.message;
	alert(errorMessage);
	var div=document.getElementById("location");
	div.innerHTML=errorMessage;
}

function computeDistance(startCoords,destCoords){
	var startLatRads=degreesToRadians(startCoords.latitude);
	var startLongRads=degreesToRadians(startCoords.longitude);
	var destLatRads=degreesToRadians(destCoords.longitude);
	var destLongRads=degreesToRadians(destCoords.longitude);

	var Radius=6371;
	var distance=Math.acos(Math.sin(startLatRads)*Math.sin(destLatRads)+
							Math.cos(startLatRads)+Math.cos(destLatRads)*Math.cos(startLongRads-destLongRads))*Radius;
	return distance;
}

function degreesToRadians(degrees)
{
	var radians=(degrees*Math.PI)/180;
	return radians;
}

