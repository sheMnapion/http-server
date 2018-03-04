function save(item)
{
	var playListArray=getStoreArray("playlist");
	var shouldPush=true;
	for(var i=0;i<playListArray.length;i++){
		if(playListArray[i]==item){
			shouldPush=false;
		}
	}
	if(shouldPush)
		playListArray.push(item);
	localStorage.setItem("playlist",JSON.stringify(playListArray));
}

function deleteSong(item)
{
	var playlistArray=getStoreArray("playlist");
	for(var i=0;i<playlistArray.length;i++){
		if(playlistArray[i]==null)
			playlistArray[i]="empty";
		if(playlistArray[i]==item){
			playlistArray.splice(i,1);
			break;
		}
	}
	localStorage.setItem("playlist",JSON.stringify(playlistArray));
}

function loadPlayList()
{
	var playlistArray=getSavedSongs();
	var ul=document.getElementById("playlist");
	if(playlistArray!=null){
		for(var i=0;i<playlistArray.length;i++){
			var li=document.createElement("li");
			li.innerHTML=playlistArray[i];
			ul.appendChild(li);
		}
	}
}

function getSavedSongs()
{
	return getStoreArray("playlist");
}

function getStoreArray(key)
{
	var playlistArray=localStorage.getItem(key);
	if(playlistArray==null||playlistArray=="")
		playlistArray=new Array();
	else
		playlistArray=JSON.parse(playlistArray);
	return playlistArray;
}
