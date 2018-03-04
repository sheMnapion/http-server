window.onload=init;
function init()
{
	var button=document.getElementById("addButton");
	button.onclick=handleButtonClick;
	var buttona=document.getElementById("deleteButton");
	buttona.onclick=handleDeleteButtonClick;
	loadPlayList();
}

function handleButtonClick()
{
	//alert("Button was clicked");
	var textInput=document.getElementById("songTextInput");
	var songName=textInput.value;
	if(songName=="")
		alert("Please enter a song!");
	else{
		//alert("Adding "+songName);
		var li=document.createElement("li");
		li.innerHTML=songName;
		var ul=document.getElementById("playlist");
		ul.appendChild(li);
		save(songName);
	}
}

function handleDeleteButtonClick()
{
	alert("In deleting!\n");
	var textInput=document.getElementById("songTextInput");
	var songName=textInput.value;
	if(songName=="")
		alert("Please enter a song!");
	else
		deleteSong(songName);
}
