#include "fileEdit.h"
#include "account.h"
#include "dealer.h"
#include <string>
#define MAXSIZE 0x100000
static char message[100000];
static char buffer[MAXSIZE];
static bool toSwitch=false;
extern int resultSize;
extern httpInfo tempInfo;
char* editMessage(char *input,int *type)
{//for figuring out the specific message to answer the request
	char *temp;
	//printf("In editting %s as input in editMessage\n",input);
	if(input==NULL)
		return NULL;
	if(strlen(input)==0)
		return NULL;
	temp=input;
	*type=judge(temp);
	memset(message,0,sizeof(message ));
	memset(buffer,0,sizeof(buffer));
	temp=input;
	//printf("Done here with input %s in editMessage\n",input);
	switch(*type){
		case HELLO:
			strncpy(message,"Hello world!",12);
			break;
		case LOGINRETURN:
			return input;
		case ADD:
			char *arg1,*arg2;
			arg1=getArgument(1,temp);
			arg2=getArgument(2,temp);
			if(arg1==NULL||arg2==NULL)
				strncpy(message,"Inefficient arguments for carrying out adding\n",80);
			else{
				int j=atoi(arg2);
				int i=atoi(getArgument(1,temp));
				sprintf(message,"%d + %d = %d\n",i,j,i+j);
			}
			break;
		case BETTERHELLO:
			printf("A better hello!\n");
			return createSimpleHtmlText("Gulu Hola");
			break;
		case BLOG:
			//printf("Enter Blog with switch:%d!\n",toSwitch);
			printf("Present Account:%s\n",presentAccount());
			writeAccountFile(presentAccount());
			//if(toSwitch)
			getHTMLFile("./webpages/index.html",buffer);
			//else
			//	getHTMLFile("./myFile1.html",buffer);
			//toSwitch=!toSwitch;
			return buffer;
		case LOADFILE:
			printf("In Loading file...\n");
			char *fileName;
			fileName=getFileName(temp);
			strncpy(message,fileName,30);
			break;
		case LOGOUT:
			//printf("In loging out\n");
			int state;
			state=logoutAccount(presentAccount());
			if(state==1){
				writeAccountFile(NULL);
				getHTMLFile("./webpages/myFile.html",buffer);
				return buffer;
			}
			break;
		case LOGIN:
			printf("To Enter with login\n");
			getHTMLFile("./webpages/login.html",buffer);
			return buffer;
		case SIGNUP:
			//printf("To Enter with Signup\n");
			getHTMLFile("./webpages/signup.html",buffer);
			return buffer;
		case POST_INFO:
			if(tempInfo.requestType==POST){
				printf("BAM!\n");
			}
			return buffer;
		default:
			strncpy(message,"Unfinished!",sizeof(message ));
			break;
	}
	return &message[0];
}

static char *dumbInfo[8];
static char senten1[]="<html>\n";
static char senten2[]="<head>\n";
static char senten3[]="<title>Better Hello World--Under HTML</title>\n";
static char senten4[]="</head>\n";
static char senten5[]="<body>\n";
static char senten6[]="<h2>Hello World! <i>Better One!!!</i></h2>\n";
static char senten7[]="</body>\n";
static char senten8[]="</html>\n";

/*static char senten1[]="<!DOCTYPE html>\n";
static char senten2[]="<html>\n";
static char senten3[]="<head>\n";
static char senten4[]="<meta charset=\"utf-8\">\n";
static char senten5[]="<title>(runoob.com)</title>\n</head>\n";
static char senten6[]="<body> <h1>我的第一个标题</h1> ";
static char senten7[]="<font size=\"5\" color=\"ff0000\"><p>我的第一个段落。</p></font></body>\n";
static char senten8[]="</html>\n";
*/
void assign()
{
	dumbInfo[0]=senten1;
	dumbInfo[1]=senten2;
	dumbInfo[2]=senten3;
	dumbInfo[3]=senten4;
	dumbInfo[4]=senten5;
	dumbInfo[5]=senten6;
	dumbInfo[6]=senten7;
	dumbInfo[7]=senten8;
	//printf("Here in assign\n");
	for(int i=0;i<7;i++){
		strcat(dumbInfo[0],dumbInfo[i+1]);
		//printf("Here in assign\n");
		//printf("for %dth turn:%s with %p\n",i,dumbInfo[0],dumbInfo[0]);
	}
	printf("%s\n",dumbInfo[0]);
}

char* createSimpleHtmlText(char *input)
{//return a html text written with input
	//sprintf(buffer,"Content-type:text/html\r\n");
	memset(buffer,0,sizeof(buffer ));
	//printf("Here\n");
	assign();
	//printf("Here\n");
	strcpy(buffer,dumbInfo[0]);
	//printf("Here\n");
	//printf("Buffer:%s\n",buffer);
	return buffer;
}

void followSPrintf(char *src,char *str)
{
	sprintf(src,str);
	src+=strlen(str);
}

void getHTMLFile(const char *pathName,char *buf)
{//for reading a html text into buf
	memset(buf,0,strlen(buf ));
	FILE *input=fopen(pathName,"r");
	if(input==NULL){
		sprintf(buf,"Can't find file %s!\n",pathName);
		return;
	}
	char c;
	int i=0;
	while((c=fgetc(input ))!=EOF){
		//printf("%c",c);
		buf[i++]=c;
		//sprintf(buf,"%c",c);
	}
	
	fclose(input);
}

void writeAccountFile(char *accountName)
{
	FILE *output=fopen("a.txt","w");
	//printf("Here\n");
	if(output==NULL){
		perror("Can't open a.txt!");
		return;
	}
	if(accountName==NULL){
		fclose(output);
		return;
	}
	else{
		fprintf(output,"<h>%s<h>",accountName);
	}
	fclose(output);
}
