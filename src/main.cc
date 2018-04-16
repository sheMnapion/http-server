#include "fileEdit.h"
#include "dealer.h"
#include "account.h"
#include "httpServer.h"
#include <time.h>
#include <pthread.h>
#define READ_LENGTH 100
int servfd = -1;
httpInfo tempInfo;
bool tryGzip;
char *tempo,*resulta,*finalResult,*bufPointer;
static char buf[20480];
pthread_t id1,id2,id3,id4;
pthread_mutex_t lock;
int type;
pthread_cond_t answerRequest,receiveRequest;
static const char unacceptedInfo[]="HTTP/1.1 200 OK\r\nContent-Length: 11\r\nContent-Type: text/html\r\n\r\nPlease wait";
int resultSize;
void sigint_handler(int signum) {
	printf("keyboard interrupt detected with number %d, close server\n",signum);
	close(servfd);
	exit(0);
}
const int queueSize=10;
int volatile fdQueue[queueSize];
int volatile queueNumber=0;
int volatile queueFrontPointer,queueEndPointer;
bool isFull()
{
	return queueFrontPointer==0?(queueEndPointer==9):(queueEndPointer==queueFrontPointer-1);
}
bool isEmpty(){return queueFrontPointer==queueEndPointer;}
void enQueue(int fd)
{
	fdQueue[queueFrontPointer++]=fd;
	queueFrontPointer%=queueSize;
	queueNumber++;
}
int deQueue()
{
	int ret=fdQueue[queueEndPointer];
	queueEndPointer=(queueEndPointer+1)%queueSize;
	queueNumber--;
	return ret;
}

void initServer(int argc,char *argv[])
{
	servfd = socket(AF_INET, SOCK_STREAM, 0);

	signal(SIGINT, sigint_handler);

	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8080);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	tryGzip=true;
	int bindTimes=1;
	int bindResult=bind(servfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	if(argv[1]==NULL){
		tryGzip=false;
	}
	else if(strcmp(argv[1],"GZIP_ON")==0){
		tryGzip=true;
	}
	else if(strcmp(argv[1],"GZIP_OFF")==0){
		tryGzip=false;
	}
	else{
		printf("Unknown command %s! Server exited\n",argv[1]);
		exit(EXIT_FAILURE);
	}
	while(bindResult<0){
		bindResult=bind(servfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
		bindTimes++;
	}
	printf("Argc:%d\n",argc);
	for(int i=0;i<3;i++){
		printf("Arg #%d:%s\n",i+1,argv[i]);
	}
	printf("Bind done after %d times!\n",bindTimes);
}

void *ht_client(void *args)
{
    while(true){
        pthread_mutex_lock(&lock);
		printf("QUEUE NUMBER in client:%d %u\n",queueNumber,pthread_self());
        while(queueNumber==10)
            pthread_cond_wait(&answerRequest,&lock);
		printf("AFER CHECK ANS IN CLIENT\n");
        int lis=listen(servfd, 50);
		printf("AFER CHECK ANS IN CLIENT\n");
		int conn;
		if(lis>=0){
			struct sockaddr_in client_addr;
			socklen_t length = sizeof(client_addr);
			memset(buf,0,sizeof(buf));
			conn = accept(servfd, (struct sockaddr*)&client_addr, &length);
			printf("TO HERE\n");
			if(conn>=0){
				printf("Accepted with conn:%d\n",conn);
				enQueue(conn);
				bufPointer=buf;
				ssize_t readLength=read(conn,(void *)bufPointer,READ_LENGTH);
				while(readLength==READ_LENGTH){
					bufPointer+=readLength;
					readLength=read(conn,(void *)bufPointer,READ_LENGTH);
				}
				pthread_cond_signal(&receiveRequest);
			}
		}
		printf("DONE RECEIVING %d\n",conn);
		pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void *ht_server(void *args)
{
	printf("IN SERVER\n");
    while(true){
        pthread_mutex_lock(&lock);
		printf("QUEUE NUMBER in server:%d %u\n",queueNumber,pthread_self());
		while(queueNumber==0)
            pthread_cond_wait(&receiveRequest,&lock);

        int fd=deQueue();
		printf("FD in server:%d\n",fd);
		bufPointer=buf;
		ssize_t readLength=read(fd,(void *)bufPointer,READ_LENGTH);
		while(readLength==READ_LENGTH){
			bufPointer+=readLength;
			readLength=read(fd,(void *)bufPointer,READ_LENGTH);
		}
        tempInfo=analyzeExplorer(buf);
		show(tempInfo);
		tempo=analyzeInput(buf);
		printf("Temp:%s\n",tempo);
		if(strlen(buf)==0){
			write(fd,unacceptedInfo,strlen(unacceptedInfo ));
			close(fd);
			memset(buf,0,sizeof(buf ));
			continue;
		}
		memset(buf,0,sizeof(buf ));
		prepare(tempInfo);
		resulta=editMessage(tempo,&type);
		if(type==LOADFILE)
			finalResult=writeFileResponse(resulta,tryGzip);
		else
			finalResult=writeSimpleResponse(resulta);
		printf("Result:\n%s afer writ respons with length %d\n",finalResult,resultSize);
		[[maybe_unused]]
		int len=write(fd, finalResult, resultSize);
		close(fd);
		pthread_cond_signal(&answerRequest);
		pthread_mutex_unlock(&lock);
    }
    return NULL;
}
void initThreads()
{
	pthread_mutex_init(&lock,NULL);
	pthread_cond_init(&answerRequest,NULL);
	pthread_cond_init(&receiveRequest,NULL);
	int ret;
	int arg=5;
	ret=pthread_create(&id1,NULL,ht_server,NULL);
	assert(ret==0);
	ret=pthread_create(&id2,NULL,ht_server,NULL);
	assert(ret==0);
	ret=pthread_create(&id3,NULL,ht_client,NULL);
	assert(ret==0);
	ret=pthread_create(&id4,NULL,ht_client,NULL);
	assert(ret==0);
	queueFrontPointer=queueEndPointer=0;
	queueNumber=0;
	assert(isEmpty());
}
int main(int argc,char *argv[]) {
	printf("Present process id:%d\n",getpid());
	init();
    initServer(argc,argv);
	initThreads();
	printf("ALL INITIALIZATION DONE\n");
	pthread_join(id1,NULL);
	pthread_join(id2,NULL);
	pthread_join(id3,NULL);
	pthread_join(id4,NULL);
	return 0;
}
