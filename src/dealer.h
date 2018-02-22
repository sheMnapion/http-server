#ifndef DEALER_H
#define DEALER_H

#include "fileEdit.h"
//for dealing with website arguments
char* editMessage(char *,int *);
char* createSimpleHtmlText(char *);
void followSPrintf(char *,char *);
void getHTMLFile(const char *,char *);
void writeAccountFile(char *);

#endif
