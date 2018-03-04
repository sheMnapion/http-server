#include "httpServer.h"
static httpInfo innerHttpInfo;
void clear(httpInfo *information)
{
	for(int i=0;i<100;i++){
		information->getInfo[i]=0;
		information->hostInfo[i]=0;
	}
	for(int i=0;i<1000;i++)
		information->userAgentInfo[i]=0;
}
httpInfo analyzeExplorer(char *buf)
{
	strPointer httpEditionPointer,hostPointer,userAgentPointer,acceptPointer;
	char *bufCopy;
	bufCopy=buf;
	//printf("Buf:\n%s\n",buf);
	clear(&innerHttpInfo);
	innerHttpInfo.isValid=true;
	if(strncmp(bufCopy,"POST",4)==0){
		innerHttpInfo.requestType=POST;
		httpEditionPointer=strstr(bufCopy,"HTTP/1.1");
		if(httpEditionPointer==NULL){
			innerHttpInfo.isValid=false;
			return innerHttpInfo;
		}
		for(int i=0;bufCopy+i+5<httpEditionPointer;i++){
			innerHttpInfo.postInfo[i]=bufCopy[5+i];
		}
	}
	else if(strncmp(bufCopy,"GET",3)==0){
		innerHttpInfo.requestType=GET;
		httpEditionPointer=strstr(bufCopy,"HTTP/1.1");
		if(httpEditionPointer==NULL){
			innerHttpInfo.isValid=false;
			return innerHttpInfo;
		}
		for(int i=0;bufCopy+i+4<httpEditionPointer;i++){
			innerHttpInfo.getInfo[i]=bufCopy[4+i];
		}
	}
	else{
		innerHttpInfo.isValid=false;
		return innerHttpInfo;
	}
	//printf("INNER SHOW:\n");
	//show(innerHttpInfo);
	hostPointer=strstr(bufCopy,"Host: ");
	userAgentPointer=strstr(bufCopy,"User-Agent: ");
	acceptPointer=strstr(bufCopy,"Accept: ");
	//printf("Bufcopy:%p hostPointer:%p userAgent:%p acceptPointer:%p",bufCopy,hostPointer,userAgentPointer,acceptPointer);
	if(hostPointer==NULL||userAgentPointer==NULL||acceptPointer==NULL){
		innerHttpInfo.isValid=false;
		return innerHttpInfo;
	}
	for(int i=0;;i++){
		if(hostPointer[i+6]!='\r')
			innerHttpInfo.hostInfo[i]=hostPointer[i+6];
		else
			break;
	}
	for(int i=0;;i++){
		if(userAgentPointer[i+12]!='\n')
			innerHttpInfo.userAgentInfo[i]=userAgentPointer[i+12];
		else
			break;
	}
	return innerHttpInfo;
}

void show(httpInfo information)
{
	if(information.isValid==false)
		printf("Invalid Connection info collected!\n");
	else{
		printf("%s:",information.requestType==POST?"post":"get");
		printf("%s\n",information.getInfo);
		printf("%s\n",information.postInfo);
		printf("HostInfo:%s\n",information.hostInfo);
		printf("UserAgent:%s\n",information.userAgentInfo);
	}
}
