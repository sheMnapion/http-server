#include "fileEdit.h"
#include "account.h"
#include <string.h>
#include <vector>
#define MAXN 0x400000
using namespace std;

static char guarantee[]="HTTP/1.1 200 OK\r\n";
static char contentLength[]="Content-Length: ";
static char length[200];
static char changeLine[]="\r\n";
static char argument[300];
static char Response[MAXN];
static char imageBuffer[MAXN];
static char errorMessage[]="Unexpected Error Occured!Please Wait";
static char usrLogin[]="Welcome!";
static char usrWrongCode[]="Wrong Code!";
static char usrMismatch[]="No such user!";
static char usrSameName[]="The account already exists!";
static char signupDone[]="Done signing up!";
static char name[20];
static char usrName[30];
static char usrPassword[30];

static char gzipType[]="Content-Encoding: gzip\r\n";

static char jpgType[]="Content-Type: image/jpg\r\n\r\n";
static char jpegType[]="Content-Type: image/jpeg\r\n\r\n";
static char pngType[]="Content-Type: image/png\r\n\r\n";
static char icoType[]="Content-Type: image/ico\r\n\r\n";
static char gifType[]="Content-Type: image/gif\r\n\r\n";
static char htmlType[]="Content-Type: text/html\r\n\r\n";
static char cssType[]="Content-Type: text/css\r\n\r\n";
static char jsType[]="Content-Type: application/js\r\n\r\n";
static char jsonType[]="Content-Type: application/json\r\n\r\n";
static char pdfType[]="Content-Type: application/pdf\r\n\r\n";
static char pptType[]="Content-Type: application/x-ppt\r\n\r\n";
//static char htmlType[]="\r\n";

static char accountName[]="account";
static char passwordName[]="passWord";
static char loginName[]="login=";
extern int resultSize;
char *parse(char *pathName)
{
	if(pathName==NULL)
		return NULL;
	int poz=-1;
	for(size_t i=0;i<strlen(pathName );i++){
		if(pathName[i]=='.')
			poz=i;
	}
	memset(name,0,sizeof(name ));
	for(size_t i=poz;i<strlen(pathName);i++)
		name[i-poz]=pathName[i+1];
	//printf("Name:%s\n",name);
	if(strcmp(name,"jpg")==0)
		return jpgType;
	else if(strcmp(name,"jpeg")==0)
		return jpegType;	
	else if(strcmp(name,"png")==0)
		return pngType;	
	else if(strcmp(name,"ico")==0)
		return icoType;
	else if(strcmp(name,"gif")==0)
		return gifType;
	else if(strcmp(name,"css")==0)
		return cssType;
	else if(strcmp(name,"js")==0)
		return jsType;
	else if(strcmp(name,"json")==0)
		return jsonType;
	else if(strcmp(name,"pdf")==0)
		return pdfType;
	else if(strcmp(name,"pptx")==0)
		return pptType;
	else
		return htmlType;
}
void writeString(char *dst,char *src)
{
	for(size_t i=0;i<strlen(src);i++)
		dst[i]=src[i];
}
void writeFile(char *dst,int size)
{
	for(int i=0;i<size;i++){
		dst[i]=imageBuffer[i];
		//if(dst[i]==0)
		//	dst[i]=1;//avoiding ending
	}
}
char* analyzeInput(char *fullInput)
{//for getting out the required thing
	//assert(fullInput[0]=='G');
	if(fullInput==NULL)
		return NULL;
	static char ret[1024];
	memset(ret,0,sizeof(ret ));
	if(strncmp(fullInput,"GET",3)==0){
		for(size_t i=0;i<strlen(fullInput );i++){
			if(fullInput[4+i]==' '||fullInput[4+i]=='\n')
				break;
			ret[i]=fullInput[4+i];
		}
		return ret;
	}
	else if(strncmp(fullInput,"POST",4)==0){
		//printf("In posting\n");
		char *usrNameStart,*passwordStart,*loginStart;
		static char site[100];
		memset(site,0,sizeof(site ));
		int loginState;
		for(int i=0;i<100;i++){
			if(fullInput[6+i]!=' ')
				site[i]=fullInput[6+i];
			else
				break;
		}//this is unsafe
		//printf("Site:%s\n",site);
		usrNameStart=strstr(fullInput,accountName);
		passwordStart=strstr(fullInput,passwordName);
		loginStart=strstr(fullInput,loginName);
		if(usrNameStart==NULL||passwordStart==NULL||loginStart==NULL)
			return NULL;//no usrname or other necessary things!!
		memset(usrName,0,sizeof(usrName ));
		memset(usrPassword,0,sizeof(usrPassword ));
		for(int i=strlen(accountName)+1;usrNameStart+i<passwordStart-1;i++)
		usrName[i-strlen(accountName)-1]=usrNameStart[i];
		for(int i=strlen(passwordName)+1;passwordStart+i<loginStart-1;i++)
		usrPassword[i-strlen(passwordName)-1]=passwordStart[i];
		//printf("Username:%s\nPassword:%s\n",usrName,usrPassword);
		if(strcmp(site,"login")==0){
			loginState=checkLogin(usrName,usrPassword);
			//printf("Login state:%d\n",loginState);
			accountLogin(usrName,usrPassword);
			if(loginState==0)
				return usrLogin;
			else if(loginState==-1)
				return usrWrongCode;
			else if(loginState==1)
				return usrMismatch;
			else
				return NULL;
		}
		else if(strcmp(site,"signup")==0){
			loginState=checkSignup(usrName,usrPassword);
			//printf("Login state:%d\n",loginState);
			if(loginState==-2)
				return NULL;
			else if(loginState==-1)
				return usrSameName;
			else{
				addAccount(usrName,usrPassword);
				return signupDone;
			}
		}
		else
			return NULL;
	}
	else{
		perror("Unexpected Request!");	
		return NULL;
	}
}

//vector<int > pausePlace;//used to store break places
static int pausePlace[100];//used to store break places
static int pauseSize;

int judge(char *fullInput)
{//for judging the func of input website from the first argument
	//printf("Full input:%s in judge\n",fullInput);
	if(fullInput==NULL)
		return LOADFILE;
	if(strcmp(fullInput,"/webpages/index.html")==0)
		return LOGIN;
	static char tempInput[0x40];
	memset(tempInput,0,sizeof(tempInput ));
	strcpy(tempInput,fullInput);
	//pausePlace.clear();
	pauseSize=0;
	int i;
	//printf("strlen of tempInput:%d\n",strlen(tempInput));
	for(i=0;i<strlen(tempInput);i++){
		if(tempInput[i]=='/'){
			pausePlace[pauseSize++]=i;
		}
	}
	pausePlace[pauseSize++]=i;
	//pausePlace.push_back(i);
	//printf("Pauseplace.size:%d\n",pauseSize);
	if(pauseSize==1)
		return LOGINRETURN;
		//printf("%d ",pausePlace[i]);
	//printf("\n");

	char *temp,*second;
	temp=getArgument(0,fullInput);
	second=getArgument(1,fullInput);
	if(pauseSize==2&&(pausePlace[1]==1)){
		return BLOG;
	}
	else if(pauseSize==2||strlen(second)==0){
		temp=getArgument(0,fullInput);
		//printf("To judge with temp:%s and strlen:%lu\n",temp,strlen(temp));
		//if(strlen(temp)==0)
			//return ERROR;//unknown error occured
		if(strcmp(temp,"hello")==0)
			return HELLO;
		else if(strcmp(temp,"bhello")==0)
			return BETTERHELLO;
		else if(strcmp(temp,"blog")==0)
			return BLOG;
		else if(strcmp(temp,"login")==0)
			return LOGIN;
		else if(strcmp(temp,"signup")==0)
			return SIGNUP;
		else if(strcmp(temp,"logout")==0)
			return LOGOUT;
		else{	//only one argument: load file
			//printf("Here\n");
			return LOADFILE;
		}
	}
	else{
		temp=getArgument(0,fullInput);
		if(strcmp(temp,"add")==0){
			return ADD;
		}
		else if(strcmp(temp,"hello")==0){
			return HELLO;
		}
		else
			return LOADFILE;
	}

	return 0;
}
char *getArgument(size_t k,char *fullInput)
{//return the kth argument of the website
 //should call after judge
	//printf("Fullinput:%s with k:%d\n",fullInput,k);
	if(fullInput==NULL)
		return NULL;
	if(k>=pauseSize-1)
		return NULL;
	memset(argument,0,sizeof(argument ));
	for(int i=0;i<pausePlace[k+1]-pausePlace[k]-1;i++){
		argument[i]=fullInput[pausePlace[k]+i+1];
	}
	//printf("Argument:%s\n",argument);
	return &argument[0];
}
char *getFileName(char *pathName)
{
	static char fileName[100];
	if(pathName==NULL)
		return NULL;
	int breakPlaces=0;
	for(int i=0;i<strlen(pathName);i++){
		if(pathName[i]=='/')
			breakPlaces++;
	}
	if(breakPlaces==1)
		strcpy(fileName,pathName+1);
	else
		strcpy(fileName,pathName+1);
	return fileName;
}
int loadPicture(char *pathName,bool *tryCompress)
{//load the picture info into imagebuf and return its size
	//printf("in loadPicture with %s\n",pathName);
	FILE *input,*output,*faultInput;
	char c;
	int i=0;
	if(*tryCompress){
		static char after[]=".gz";
		static char compressedPath[100];
		memset(compressedPath,0,sizeof(compressedPath ));
		strcpy(compressedPath,pathName);
		strcat(compressedPath,after);
		//printf("Expanded path:%s\n",compressedPath);
		FILE *compressedInput;
		compressedInput=fopen(compressedPath,"rb");
		memset(imageBuffer,0,sizeof(imageBuffer ));
		if(compressedInput!=NULL){
			//printf("compressedInput:%p\n",compressedInput);
			while(fscanf(compressedInput,"%c",&c)!=EOF){
				imageBuffer[i++]=c;
			}
			fclose(compressedInput);
			return i;
		}
		*tryCompress=false;
	}
	input=fopen(pathName,"rb");
	output=fopen("trial.ico","w");
	if(input==NULL){
		//printf("NULL here\n");
		faultInput=fopen("webpages/noFile.html","rb");
		//printf("Faultinput:%p\n",faultInput);
		while(fscanf(faultInput,"%c",&c)!=EOF){
			imageBuffer[i++]=c;
		}
		fclose(output);
		fclose(faultInput);
		return i;
		//sprintf(imageBuffer,"Can't find file %s\n",pathName);
		//printf("Can't find file %s\n",pathName);
		//return strlen(imageBuffer);
	}
	while(fscanf(input,"%c",&c)!=EOF){
		imageBuffer[i++]=c;
		fprintf(output,"%c",c);
	}
	//printf("I:%d\n",i);
	fclose(input);
	fclose(output);
	return i;//strlen(imageBuffer);
}
void simplifiedItoa(size_t length,char *dst)
{
	size_t temp=length;
	memset(dst,0,strlen(dst ));
	if(temp==0){
		temp=strlen(errorMessage);
	}
	size_t i=0;
	while(temp>0){
		int tempBit=temp%10;
		temp/=10;
		dst[i++]='0'+tempBit;
	}
	for(i=0;i<strlen(dst )/2;i++){
		char temp=dst[i];
		dst[i]=dst[strlen(dst )-1-i];
		dst[strlen(dst )-1-i]=temp;
	}
}
char* writeSimpleResponse(char *msg)
{
	//if(msg!=NULL)
	//	printf("Msg in writing response:%s with strlen %d\n",msg,strlen(msg ));
	//else
	//	printf("Null msg\n");
	size_t lengtha;
	if(msg==NULL)
		lengtha=0;
	else
		lengtha=strlen(msg);
	char *stringStart=NULL;
	memset(Response,0,sizeof(Response ));
	stringStart=&Response[0];
	writeString(stringStart,guarantee);
	stringStart+=strlen(guarantee);
	writeString(stringStart,contentLength);
	stringStart+=strlen(contentLength);
	
	
	simplifiedItoa(lengtha,length);
	writeString(stringStart,length);
	stringStart+=strlen(length);

	writeString(stringStart,changeLine);
	stringStart+=2;
	//writeString(stringStart,changeLine);
	//stringStart+=2;

	writeString(stringStart,htmlType);
	stringStart+=strlen(htmlType);
	if(msg!=NULL)
		writeString(stringStart,msg);
	else
		writeString(stringStart,errorMessage);

	resultSize=strlen(Response);
	//printf("Response:\n%s\n",Response);
	return Response;
}
char* writeFileResponse(char *pathName)
{//for writing response from the path
	//printf("Path:%s\n",pathName);
	int size;
	bool tryCompress=true;
	if(strcmp(pathName,"a.txt")==0)
		tryCompress=false;	
	size=loadPicture(pathName,&tryCompress);
	//printf("Size:%d\n",size);
	char *stringStart=NULL,*contentType=NULL;
	memset(Response,0,sizeof(Response ));
	stringStart=&Response[0];
	
	writeString(stringStart,guarantee);
	stringStart+=strlen(guarantee);

	if(tryCompress){
		writeString(stringStart,gzipType);
		stringStart+=strlen(gzipType);
	}

	writeString(stringStart,contentLength);
	stringStart+=strlen(contentLength);
	simplifiedItoa(size,length);
	writeString(stringStart,length);
	stringStart+=strlen(length);
	writeString(stringStart,changeLine);
	stringStart+=2;

	//printf("Done here\n");
	contentType=parse(pathName);
	if(contentType==NULL)
		contentType=htmlType;
	writeString(stringStart,contentType);
	stringStart+=strlen(contentType);
	resultSize=strlen(Response);	

	writeFile(stringStart,size);
	resultSize+=size;
	return Response;
}
