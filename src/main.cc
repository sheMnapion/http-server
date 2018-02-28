#include "fileEdit.h"
#include "dealer.h"
#include "account.h"
#include "httpServer.h"
#define READ_LENGTH 100
int servfd = -1;
httpInfo tempInfo;
static char buf[20480];
static const char unacceptedInfo[]="HTTP/1.1 200 OK\r\nContent-Length: 11\r\nContent-Type: text/html\r\n\r\nPlease wait";
int resultSize;
void sigint_handler(int signum) {
	printf("keyboard interrupt detected with number %d, close server\n",signum);
	close(servfd);
	exit(0);
}

int main() {
	init();
	servfd = socket(AF_INET, SOCK_STREAM, 0);

	signal(SIGINT, sigint_handler);

	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8080);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int bindTimes=1;
	int bindResult=bind(servfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	while(bindResult<0){
		bindResult=bind(servfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
		bindTimes++;
	}
	printf("Bind done after %d times!\n",bindTimes);
	listen(servfd, 50);
	struct sockaddr_in client_addr;
	socklen_t length = sizeof(client_addr);

	int conn = -1;
	int type;
	int connectTime=0;
	memset(buf,0,sizeof(buf ));
	while((conn = accept(servfd, (struct sockaddr*)&client_addr, &length)) != -1) {
		printf("Accepted\n");
		char *tempo,*resulta,*finalResult,*bufPointer;
		bufPointer=buf;
		ssize_t readLength=read(conn,(void *)bufPointer,READ_LENGTH);
		while(readLength==READ_LENGTH){
			bufPointer+=readLength;
			readLength=read(conn,(void *)bufPointer,READ_LENGTH);
		}
		tempInfo=analyzeExplorer(buf);
		show(tempInfo);
		printf("Buf from connection:%s\nlength:%lu time:%d\n",buf,strlen(buf),connectTime++);
		tempo=analyzeInput(buf);
		//printf("Temp:%s\n",tempo);
		if(strlen(buf)==0){
			write(conn,unacceptedInfo,strlen(unacceptedInfo ));
			close(conn);
			memset(buf,0,sizeof(buf ));
			continue;
		}
		memset(buf,0,sizeof(buf ));
		prepare(tempInfo);
		resulta=editMessage(tempo,&type);
		if(type==LOADFILE)
			finalResult=writeFileResponse(resulta);
		else
			finalResult=writeSimpleResponse(resulta);
		printf("Result:\n%s afer writ respons with length %d\n",finalResult,resultSize);
		[[maybe_unused]]
		int len=write(conn, finalResult, resultSize);
		close(conn);
		showPresentAccounts();
		showPresentAccessedAccounts();
	}

	return 0;
}
