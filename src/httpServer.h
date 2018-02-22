#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
using namespace std;
enum serverInfoType {GET,POST};
struct Info{
	serverInfoType requestType;
	bool isValid;
	union {
		char getInfo[100];
		char postInfo[100];
	};
	char hostInfo[100];
	char userAgentInfo[1000];
};
typedef struct Info httpInfo;
typedef char* strPointer;

httpInfo analyzeExplorer(char *buf);
void show(httpInfo );
void clear(httpInfo *);
#endif
