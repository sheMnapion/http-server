#include "account.h"
User users[MAXUSERSIZE];
static int usrNumbers;
static char accessedHosts[10][100];
static int accessedUsers[10];
//static httpInfo presentHTTP;
static char presentHostInfo[100];
static char presentAgentInfo[1000];
int presentHostNumber;
int accountNumber;
void init()
{
	memset(users,0,sizeof(users));
	strcpy(users[0].usrName,"lac");
	strcpy(users[0].usrPassword,"west01010101");
	memset(accessedHosts,0,sizeof(accessedHosts ));
	usrNumbers=1;//only one admin
	presentHostNumber=0;
}
void prepare(httpInfo serverInfo)
{
	strcpy(presentHostInfo,serverInfo.hostInfo);
	strcpy(presentAgentInfo,serverInfo.userAgentInfo);
}

int checkLogin(char *loginName,char *loginPassword)
{
	if(loginName==NULL||loginPassword==NULL)
		return -2;//wrong input
	for(int i=0;i<usrNumbers;i++){
		if(strcmp(users[i].usrName,loginName)==0){
			if(strcmp(users[i].usrPassword,loginPassword)==0){
				accountNumber=i;
				//printf("Match!\n");
				return 0;
			}
			else{
				//printf("Wrong password!\n");
				return -1;
			}
		}
	}
	return 1;//no such account
}

int checkSignup(char *signupName,char *signupPassword)
{
	if(signupName==NULL||signupPassword==NULL)
		return -2;//wrong input
	for(int i=0;i<usrNumbers;i++)
		if(strcmp(users[i].usrName,signupName)==0)
			return -1;//already same account exists
	return 1;//able to sign up
}
void addAccount(char *loginName,char *loginPassword)
{
	if(loginName==NULL||loginPassword==NULL)
		return;
	if(usrNumbers>=MAXUSERSIZE){
		return;//can't add more accounts
	}
	for(int i=0;i<30;i++){
		users[usrNumbers].usrName[i]='\0';
		users[usrNumbers].usrPassword[i]='\0';
	}
	strcpy(users[usrNumbers].usrName,loginName);
	strcpy(users[usrNumbers].usrPassword,loginPassword);
	usrNumbers+=1;
}
int logoutAccount(char *accountName)
{
	if(accountName==NULL)
		return -1;
	for(int i=0;i<usrNumbers;i++){
		if(strcmp(accountName,users[i].usrName)==0){
			for(int j=0;j<10;j++){
				if(strcmp(accessedHosts[j],presentHostInfo)==0){
					memset(accessedHosts[j],0,sizeof(accessedHosts[j]));
					accessedUsers[j]=-1;
					break;
				}
			}
			
			return 1;//could log out
		}
	}
	return 0;// no match
}
void accountLogin(char *loginName,char *loginPassword)
{
	int result=checkLogin(loginName,loginPassword);
	if(result==0){
		for(int i=0;i<10;i++){
			if(strcmp(accessedHosts[i],presentHostInfo)==0){
				accessedUsers[i]=accountNumber;
			}
		}
		strcpy(accessedHosts[presentHostNumber],presentHostInfo);
		accessedUsers[presentHostNumber++]=accountNumber;	
	}
}

char *presentAccount()
{
	for(int i=0;i<10;i++){//only allowing 10 users to login aud p
		if(strcmp(accessedHosts[i],presentHostInfo)==0){
			if(accessedUsers[i]<0)
				return NULL;
			return users[accessedUsers[i]].usrName;
		}
	}
	return NULL;
}

void showPresentAccessedAccounts()
{
	for(int i=0;i<10;i++){
		if(strlen(accessedHosts[i])==0){
			printf("Host #%d: Unreached.\n",i);
		}
		else{
			printf("Host #%d: %s\t",i,accessedHosts[i]);
			printf("Usr Name:%s\n",users[accessedUsers[i]].usrName);
		}
	}
}

void showPresentAccounts()
{
	for(int i=0;i<usrNumbers;i++){
		printf("User #%d:",i);
		printf("%s\twith password %s\n",users[i].usrName,users[i].usrPassword);
	}
	printf("Host this time:%s\n",presentHostInfo);
	printf("Agent this time:%s\n",presentAgentInfo);
}
