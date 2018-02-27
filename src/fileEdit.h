#ifndef FILEEDIT_H
#define FILEEDIT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <iostream>

void writeString(char *,char *);
char* analyzeInput(char *fullInput);
void simplifiedItoa(size_t length,char *dst);
char* writeSimpleResponse(char *);
char* writeFileResponse(char *);
int judge(char *);
char* getArgument(size_t k,char *fullInput);
char* getFileName(char *fileName);
char* sparse(char *pathName);
int loadPicture(char *pathName);

enum {ADD,HELLO,POST_INFO,SIGNUP,BLOG,BETTERHELLO,ERROR,LOGIN,LOGOUT,LOGINRETURN,DEFAULT,LOADFILE};
#endif
