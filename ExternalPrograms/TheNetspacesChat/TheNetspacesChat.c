/*
The code in that version could be unsecure - better to use
in "demilitarized zone" only like local or tunneled
network with the security on the network level
and server on the separate or sandboxed computer
when online in hazardous networks.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#define ADDRESS "127.0.0.1"
#define PORT 38120
//#define PORT 38121
#define LISTENINGQUEUELIMIT 512
#define NICKNAMELIMIT 39 
#define NUMBERSTRINGLIMIT 15
#define BUFFERSIZE 512
#define MESSAGELIMIT 1024*1000*1
//#define BUFFERSIZE MESSAGELIMIT
#define QUERYSIZE MESSAGELIMIT
#define CONNECTEDCLIENTSLIMIT 512
#define LOGLINELIMIT 1024*1000*1
#define BLNREPEATMESSAGEINACKNOWLEDGMENT 0
int intServerSocket;
struct cctArgs{
	int intClientSocket;
	char* strIP;
}; 
struct clientID{
	int intID;
	char* strIP;
	char* strNickname;
	int intReceiving;
	struct cctArgs* receivingCctArgs;
};
struct clientID** connectedClients;
int intPendingConnections;
struct cctArgs** pendingConnectionsCctArgs;
int intConnectedClientsLength;
int* intConnectedClientSockets;
int intConnectedClientSocketsLength;
FILE* fileLog0,*fileLog1;
int logprintf(char* strText,int intLogLevel){
	if(0==intLogLevel){
		printf("%s\n",strText);
		fprintf(fileLog0,"%s\n",strText);
		fflush(fileLog0);
	}
	if(intLogLevel>=2){
		printf("%s\n",strText);
		fprintf(fileLog1,"%s\n",strText);
		fflush(fileLog1);
	}
}
struct clientID* newClientID(int intID,char* strIP,char* strNickname){
	struct clientID* theClientID=(struct clientID*)malloc(sizeof(struct clientID));
	theClientID->strIP=(char*)malloc(16*sizeof(char));
	theClientID->strNickname=(char*)malloc(NICKNAMELIMIT*sizeof(char));
	theClientID->intID=intID;	
	theClientID->intReceiving=0;
	theClientID->receivingCctArgs=NULL;
	strncpy(theClientID->strIP,strIP,16);
	strncpy(theClientID->strNickname,strNickname,NICKNAMELIMIT);
	return theClientID;
}
void deleteCctArgs(struct cctArgs* theCctArgs){
	//logprintf("deleteCctArgs",0);
	if(NULL!=theCctArgs){
		if(NULL!=theCctArgs->strIP){
			//logprintf("free(theCctArgs->strIP);",0);
			free(theCctArgs->strIP);
		}
		if(NULL!=theCctArgs)
			free(theCctArgs);
	}
}
void removeClientIDreceivingCctArgs(struct clientID* theClientID){
	//logprintf("==================>removeClientIDreceivingCctArgs",0);
	deleteCctArgs(theClientID->receivingCctArgs);
	theClientID->receivingCctArgs=NULL;
	theClientID->intReceiving=0;
}
void removePendingConnectionsClientIDreceivingCctArgs(){
	struct cctArgs* theCctArgs;
	for(int ii=0;ii<intPendingConnections;ii++){
		theCctArgs=pendingConnectionsCctArgs[ii];
		deleteCctArgs(theCctArgs);
		pendingConnectionsCctArgs[ii]=NULL;
		//logprintf("pendingConnection removed",0);
	}
}
void moveClientIDreceivingCctArgsToPendingConnections(struct clientID* theClientID){
	struct cctArgs* theCctArgs=theClientID->receivingCctArgs;
	//logprintf("==================>moveClientIDreceivingCctArgsToPendingConnections",0);
	pendingConnectionsCctArgs[intPendingConnections]=(struct cctArgs*)malloc(sizeof(struct cctArgs));
	pendingConnectionsCctArgs[intPendingConnections]->intClientSocket=theCctArgs->intClientSocket;
	pendingConnectionsCctArgs[intPendingConnections]->strIP=(char*)malloc(16*sizeof(char));
	strncpy(pendingConnectionsCctArgs[intPendingConnections]->strIP,theCctArgs->strIP,16);
	deleteCctArgs(theClientID->receivingCctArgs);
	theClientID->receivingCctArgs=NULL;
	theClientID->intReceiving=0;
	intPendingConnections++;
}
void deleteClientID(struct clientID* theClientID){
	removeClientIDreceivingCctArgs(theClientID);
	if(NULL!=theClientID){
		if(NULL!=theClientID->strIP){
			free(theClientID->strIP);
			//logprintf("free(theClientID->strIP);",0);
		}
		if(NULL!=theClientID->strNickname){
			free(theClientID->strNickname);
			//logprintf("free(theClientID->strNickname);",0);
		}
		if(NULL!=theClientID){
			free(theClientID);
			//logprintf("free(theClientID);",0);
		}
	}
}
void addClientIDreceivingCctArgs(int ii,struct cctArgs* theCctArgs){
	if(NULL==connectedClients)return; 
	struct clientID* theClientID=connectedClients[ii];
	if(NULL!=theClientID->receivingCctArgs)
		moveClientIDreceivingCctArgsToPendingConnections(theClientID);
	theClientID->receivingCctArgs=(struct cctArgs*)malloc(sizeof(struct cctArgs));
	theClientID->receivingCctArgs->intClientSocket=theCctArgs->intClientSocket;
	theClientID->receivingCctArgs->strIP=(char*)malloc(16*sizeof(char));
	theClientID->intReceiving=1;
	strncpy(theClientID->receivingCctArgs->strIP,theCctArgs->strIP,16);
	//char strLog[LOGLINELIMIT];
	//snprintf(strLog,LOGLINELIMIT,"-------------------->addClientIDreceivingCctArgs IP=%s",theCctArgs->strIP);
	//logprintf(strLog,0);
}
int freeResources(){
	//char strLog[LOGLINELIMIT];
	//snprintf(strLog,LOGLINELIMIT,"freeResources(): intConnectedClientsLength=%d",intConnectedClientsLength);
	//logprintf(strLog,0);
	for(int ii=0;ii<intConnectedClientsLength;ii++){
		if(NULL!=connectedClients&&NULL!=connectedClients[ii]){
			//snprintf(strLog,LOGLINELIMIT,"deleteClientID(connectedClients[%d]);",ii);
			//logprintf(strLog,0);
			deleteClientID(connectedClients[ii]);
			connectedClients[ii]=NULL;
		}
	}
	intConnectedClientsLength=0;
	//logprintf("free(connectedClients);",0);
	if(NULL!=connectedClients){
		free(connectedClients);
		connectedClients=NULL;
	}
	//logprintf("free(connectedClients); done",0);

	removePendingConnectionsClientIDreceivingCctArgs();
	if(NULL!=pendingConnectionsCctArgs){
		free(pendingConnectionsCctArgs);
		pendingConnectionsCctArgs=NULL;
	}
	for(int ii=0;ii<intConnectedClientSocketsLength;ii++){
		if(NULL!=intConnectedClientSockets){
			close(intConnectedClientSockets[ii]);
		}
	}
	intConnectedClientSocketsLength=0;
	if(NULL!=intConnectedClientSockets){
		free(intConnectedClientSockets);
		intConnectedClientSockets=NULL;
	}
	close(intServerSocket);
	fflush(fileLog0);	
	fclose(fileLog0);
	fflush(fileLog1);	
	fclose(fileLog1);
}
int addClientSocket(int intClientSocket){
	intConnectedClientSockets[intConnectedClientSocketsLength++]=intClientSocket;
}
int removeClientSocket(int intClientSocket){
	int ii,jj;
	for(ii=0;ii<intConnectedClientSocketsLength;ii++){
		if(intClientSocket==intConnectedClientSockets[ii]){
			for(jj=ii;jj<intConnectedClientSocketsLength;jj++){
				intConnectedClientSockets[jj]=intConnectedClientSockets[jj+1];
			}
			intConnectedClientSockets[jj]=-1;
			intConnectedClientSocketsLength--;
			break;
		}
	}
}
int createServerSocket(int intServerPort){
	struct sockaddr_in serveraddr;
	memset(&serveraddr,0,sizeof(serveraddr));
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(intServerPort);
	serveraddr.sin_addr.s_addr=inet_addr(ADDRESS);
	int intServerSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(-1==intServerSocket) {
    		perror("Socket creation error");
    		logprintf("Socket creation error",0);
    		return -1;
	}
	if(-1==bind(intServerSocket,(struct sockaddr*)&serveraddr,sizeof(serveraddr))){
    		close(intServerSocket);
		if(-1==bind(intServerSocket,(struct sockaddr*)&serveraddr,sizeof(serveraddr))){
    			perror("I tried, bind error");
    			logprintf("I tried, bind error",0);
    			close(intServerSocket);
    			return -1;
		}
	}
	if (-1==listen(intServerSocket,LISTENINGQUEUELIMIT)){
    		perror("Listen error");
    		logprintf("Listen error",0);
    		close(intServerSocket);
    		return -1;
	}
	return intServerSocket;
}
struct cctArgs* acceptClientConnection(int intServerSocket){
	struct sockaddr_in clientaddr;
	memset(&clientaddr,0,sizeof(clientaddr));
	socklen_t socklen=sizeof(clientaddr);
	int intClientSocket=accept(intServerSocket,(struct sockaddr*)&clientaddr,&socklen);
	if (-1==intClientSocket) {
    		perror("Accept error");
    		logprintf("Accept error",0);
    		close(intServerSocket);
    		return NULL;
	}

	addClientSocket(intClientSocket);
	struct cctArgs* theCctArgs;
	if(NULL==(theCctArgs=(struct cctArgs*)malloc(sizeof(struct cctArgs)))){
		perror("cctArgs malloc error");
		logprintf("cctArgs malloc error",0);
    		close(intServerSocket);
		return NULL;
	}
	theCctArgs->intClientSocket=intClientSocket;
	if(NULL==(theCctArgs->strIP=(char*)malloc(16*sizeof(char)))){
		perror("cctArgs->strIP malloc error");
		logprintf("cctArgs->strIP malloc error",0);
    		close(intServerSocket);
		return NULL;
	}

	//char strLog[LOGLINELIMIT];
	//snprintf(strLog,LOGLINELIMIT,"Client connected. IP=%s",inet_ntoa(clientaddr.sin_addr));
	//logprintf(strLog,0);
	snprintf(theCctArgs->strIP,16,"%s",inet_ntoa(clientaddr.sin_addr));
	return theCctArgs;
}
int helloResponse(int intClientSocket){
	char strHello[]="HTTP/1.1 200 OK\nContent-Type: text/html\nAccess-Control-Allow-Origin: *\n\n<html><head></head><body>It works!</body></html>";
	//char strLog[LOGLINELIMIT];
	//snprintf(strLog,LOGLINELIMIT,"(int)strHello[strlen(strHello)]=%d \n",(int)strHello[strlen(strHello)]);
	//logprintf(strLog,0);
	//snprintf(strLog,LOGLINELIMIT,"(int)strHello[strlen(strHello)-1]=%d \n",(int)strHello[strlen(strHello)-1]);
	//logprintf(strLog,0);
	if (-1==send(intClientSocket,strHello,strlen(strHello),0)){
    		perror("Send error");
    		logprintf("Send error",0);
    		close(intClientSocket);
		removeClientSocket(intClientSocket);
    		return -1;
	}
	close(intClientSocket);
	removeClientSocket(intClientSocket);
	//snprintf(strLog,LOGLINELIMIT,"%s",strHello);
	//logprintf(strLog,0);
	//logprintf("hello sent \n",0);
}
int sendMessage(struct cctArgs* theCctArgs,char* strMessage){
	//logprintf("sendMessage( \n",2);
	//logprintf("strMessage=\n",2);
	//logprintf(strMessage,2);
	//char strLogMessage[BUFFERSIZE];
	char strHeader[]="HTTP/1.1 200 OK\nContent-Type: text/html\nAccess-Control-Allow-Origin: *\n\n";
	int intHeader=strlen(strHeader);
	int intMessage=strlen(strMessage);
	char strWholeMessage[intHeader+intMessage];
	strncpy(strWholeMessage,strHeader,intHeader);
	strncpy(strWholeMessage+intHeader,strMessage,intMessage);
	strWholeMessage[intHeader+intMessage]='\0';
	//strncpy(strLogMessage,theCctArgs->strIP,16);
	//strncpy(strLogMessage+17,":",1);
	//strncpy(strLogMessage+18,strWholeMessage,strlen(strWholeMessage));
	//logprintf(strLogMessage,2);
	//logprintf("strWholeMessage=\n",2);
	//logprintf(strWholeMessage,2);
	ssize_t sentBytes=0;
	ssize_t intOffset=0;
	ssize_t intToSend=strlen(strWholeMessage);
	do{
		sentBytes=send(theCctArgs->intClientSocket,strWholeMessage+intOffset,(intToSend<BUFFERSIZE)?intToSend:BUFFERSIZE,0);
		intOffset+=sentBytes;
		intToSend-=sentBytes;

	}while(sentBytes==BUFFERSIZE&&intToSend>0);
	if(-1==sentBytes){
    		perror("Send error");
    		logprintf("Send error",0);
    		close(theCctArgs->intClientSocket);
		removeClientSocket(theCctArgs->intClientSocket);
    		return -1;
	}
	removeClientSocket(theCctArgs->intClientSocket);
	close(theCctArgs->intClientSocket);
	//logprintf("sendMessage done \n",2);
}
int getIntIdAndStrCommand(int* intID, char* strCommand,char* strBuffer, int intClientSocket){
	char strGET[]="GET";
	char strHTTP[]="HTTP";
	char* chrGet=strstr(strBuffer,strGET);
	char* chrHttp=strstr(strBuffer,strHTTP);
	int intQuerySize=chrHttp-chrGet-1;
	if(intQuerySize>QUERYSIZE)intQuerySize=QUERYSIZE;
	char strQuery[QUERYSIZE];
	strncpy(strQuery,chrGet,(size_t)intQuerySize);
	strQuery[intQuerySize]='\0';
	//char strLog[LOGLINELIMIT];
	//snprintf(strLog,LOGLINELIMIT,"intQuerySize=%d, strQuery=|%s| \n",intQuerySize,strQuery);	
	//logprintf(strLog,0);
	char strIntId[]="intID=";
	char strStrCommand[]="strCommand=";
	char strAnd[]="&";
	char* chrIntId=strstr(strQuery,strIntId);
	char* chrStrCommand=strstr(strQuery,strStrCommand);
	char* chrAnd=strstr(strQuery,strAnd);
	if(NULL==chrIntId||NULL==chrStrCommand||NULL==chrAnd){
		perror("Incorrect query");
		logprintf("Incorrect query",0);
		removeClientSocket(intClientSocket);
		close(intClientSocket);
		return -1;
	}
	char strId[100];
	int intStrIdLength=chrAnd-chrIntId-6;
	int intStrCommandLength=strlen(strQuery)-(chrStrCommand-strQuery)-2;
	strncpy(strId,chrIntId+6,(size_t)intStrIdLength);
	strId[intStrIdLength]='\0';
	strncpy(strCommand,chrStrCommand+11,(size_t)intStrCommandLength);
	strCommand[intStrCommandLength]='\0';
	//snprintf(strLog,LOGLINELIMIT,"strId=|%s|, strCommand=|%s| \n",strId,strCommand);
	//logprintf(strLog,0);
	*intID=atoi(strId);
}
int findConnectedClient(int intID,char* strIP,char* strNickname){
	if(NULL==connectedClients)return -1;
	for(int ii=0;ii<intConnectedClientsLength;ii++){
		if(NULL!=connectedClients[ii]&&intID==connectedClients[ii]->intID&&0==strcmp(strIP,connectedClients[ii]->strIP)&&0==strcmp(strNickname,connectedClients[ii]->strNickname)){
			return ii;
		}
	}
	return -1;
}
int findConnectedClient_(int intID){
	if(NULL==connectedClients)return -1;
	for(int ii=0;ii<intConnectedClientsLength;ii++){
		if(NULL!=connectedClients[ii]&&intID==connectedClients[ii]->intID){
			return ii;
		}
	}
	return -1;
}
int connectCommand(int intID, char* strCommand, struct cctArgs* theCctArgs){
	if(NULL==connectedClients)return -1;
	char strNickname[NICKNAMELIMIT];
	char* chr40=strstr(strCommand,"(");
	char* chr41=strstr(strCommand,")");
	if(NULL==chr40||NULL==chr41){
		return -1;
	}
	int intNicknameLength=chr41-chr40-1;
	strncpy(strNickname,chr40+1,intNicknameLength);
	strNickname[intNicknameLength]='\0';
	int ii=findConnectedClient(intID,theCctArgs->strIP,strNickname);
	char strMessage[BUFFERSIZE];
	if(-1==ii){
		connectedClients[intConnectedClientsLength++]=newClientID(intID,theCctArgs->strIP,strNickname);
		snprintf(strMessage,BUFFERSIZE,"connectedSucessfully?intID=%d&strNickname=%s;",intID,strNickname);
		sendMessage(theCctArgs,strMessage);
	}
	else{
		snprintf(strMessage,BUFFERSIZE,"userAlreadyConnected?intID=%d&strNickname=%s;",intID,strNickname);
		sendMessage(theCctArgs,strMessage);
	}
}
void* receivingFunction(void* strContent){
	char* theContent=(char*)strContent;
	//char strLog[LOGLINELIMIT];
	//snprintf(strLog,LOGLINELIMIT,"receivingFunction(%s);\n",theContent);
	//logprintf(strLog,0);
}
int receivingCommand(int intID, char* strCommand, struct cctArgs* theCctArgs){
	if(NULL==connectedClients)return -1;
	//logprintf("receivingCommand \n",0);
	int ii=findConnectedClient_(intID);
	char strMessage[BUFFERSIZE];
	//char strLog[LOGLINELIMIT];
	if(-1==ii){
		snprintf(strMessage,BUFFERSIZE,"userNotConnected?receiving?intID=%d;",intID);
		sendMessage(theCctArgs,strMessage);
	}
	else{
		addClientIDreceivingCctArgs(ii,theCctArgs);
		//snprintf(strLog,LOGLINELIMIT,"---------------------------->connectedClients[ii]->receivingCctArgs=theCctArgs; ii=%d",ii);
		//logprintf(strLog,0);
	}
	for(int ii=0;ii<intConnectedClientsLength;ii++){
		//snprintf(strLog,LOGLINELIMIT,"After receving command: connectedClients[%d]->intID=%d, connectedClients[%d]->intReceiving=%d \n",ii,connectedClients[ii]->intID,ii,connectedClients[ii]->intReceiving);
		//logprintf(strLog,0);
	}
}
int sendToCommand(int intID, char* strCommand, struct cctArgs* theCctArgs){
	//logprintf("sendToCommand( \n",2);
	//logprintf(strCommand,2);
	//logprintf("\n",2);
	if(NULL==connectedClients)return -1;
	char strSendTo[NUMBERSTRINGLIMIT];
	char strContent[MESSAGELIMIT];
	char* chr40=strstr(strCommand,"(");
	char* chr44=strstr(strCommand,",");
	char* chr41=strstr(strCommand,")");
	//logprintf(chr40,2);
	//logprintf(chr41,2);
	if(NULL==chr40||NULL==chr41){
		//if(NULL==chr40)logprintf("NULL==chr40 \n",2);
		//if(NULL==chr41)logprintf("NULL==chr41 \n",2);
		return -1;
	}
	/*
	else{
		logprintf("both chr40 and chr41 not NULL \n",2);
	}
	*/
	int intSendToLength=chr44-chr40-1;
	int intContentLength=chr41-chr44-1;
	strncpy(strSendTo,chr40+1,intSendToLength);
	strncpy(strContent,chr44+1,intContentLength);
	strSendTo[intSendToLength]='\0';
	strContent[intContentLength]='\0';
	int intSendTo=atoi(strSendTo);
	int ii=findConnectedClient_(intID);
	int jj=findConnectedClient_(intSendTo);

	//char strLog[LOGLINELIMIT];
	//snprintf(strLog,LOGLINELIMIT,"intSendTo=%d jj=%d \n",intSendTo,jj);
	//logprintf(strLog,0);
	//logprintf("strContent: \n",2);
	//logprintf(strContent,2);
	
	char strMessage[MESSAGELIMIT];
	if(-1==ii){
		snprintf(strMessage,MESSAGELIMIT,"youAreNotConnected?sendTo?intID=%d&intSendTo=%d&strMessage=%s;",intID,intSendTo,strContent);
		sendMessage(theCctArgs,strMessage);
	}
	else{
		if(-1==jj){
			snprintf(strMessage,MESSAGELIMIT,"userNotConnected?sendTo?intID=%d&intSendTo=%d&strMessage=%s;",intID,intSendTo,strContent);
			sendMessage(theCctArgs,strMessage);
		}else{
			//snprintf(strLog,LOGLINELIMIT,"ii found=%d \n",ii);
			//logprintf(strLog,0);
			for(int ii=0;ii<intConnectedClientsLength;ii++)
				//snprintf(strLog,LOGLINELIMIT,"Before sendTo command: connectedClients[%d]->intID=%d, connectedClients[%d]->intReceiving=%d \n",ii,connectedClients[ii]->intID,ii,connectedClients[ii]->intReceiving);
				//logprintf(strLog,0);
			if(1==connectedClients[jj]->intReceiving){
				snprintf(strMessage,MESSAGELIMIT,"receiveMessage?intID=%d&intRecvFrom=%d&strMessage=%s;",connectedClients[jj]->intID,intID,strContent);
				//snprintf(strLog,LOGLINELIMIT,"strMessage=%s\n",strMessage);
				//logprintf(strLog,0);
				sendMessage(connectedClients[jj]->receivingCctArgs,strMessage);
				removeClientIDreceivingCctArgs(connectedClients[jj]);
				//snprintf(strLog,LOGLINELIMIT,"=============>removeClientIDreceivingCctArgs ii=%d",ii);
				//logprintf(strLog,0);

				if(BLNREPEATMESSAGEINACKNOWLEDGMENT==0)
					//bandwidth optimization
					snprintf(strMessage,MESSAGELIMIT,"messageSent?intID=%d&intSendTo=%d&strMessage=%s;",intID,intSendTo,"...");
				else
					snprintf(strMessage,MESSAGELIMIT,"messageSent?intID=%d&intSendTo=%d&strMessage=%s;",intID,intSendTo,strContent);
				//snprintf(strLog,LOGLINELIMIT,"acknowledgment strMessage=%s\n",strMessage);
				//logprintf(strLog,0);
				sendMessage(theCctArgs,strMessage);
			}else{
				snprintf(strMessage,MESSAGELIMIT,"userConnectedButNotListening?sendTo?intID=%d&intSendTo=%d&strMessage=%s;",intID,intSendTo,strContent);
				//snprintf(strLog,LOGLINELIMIT,"strMessage=%s\n",strMessage);
				//logprintf(strLog,0);
				sendMessage(theCctArgs,strMessage);
			}
		}
	}
};
int changeNickname(int intID,char* strCommand,struct cctArgs* theCctArgs){
	if(NULL==connectedClients)return -1;
	//logprintf("---------->changeNickname",0);
	int ii=findConnectedClient_(intID);
	//char strLog[LOGLINELIMIT];
	//snprintf(strLog,LOGLINELIMIT,"findConnectedClient intID=%d,ii=%d",intID,ii);
	//logprintf(strLog,0);
	if(-1!=ii){
		char strNickname[NICKNAMELIMIT];
		char* chr40=strstr(strCommand,"(");
		char* chr41=strstr(strCommand,")");
		int intContentLength=chr41-chr40-1;
		strncpy(strNickname,chr40+1,intContentLength);
		strNickname[intContentLength]='\0';
		struct clientID* theClientID=connectedClients[ii];
		strncpy(theClientID->strNickname,strNickname,NICKNAMELIMIT);
		char strMessage[BUFFERSIZE];
		snprintf(strMessage,BUFFERSIZE,"changedNickname?intID=%d&strNickname=%s;",intID,strNickname);
		//logprintf("Will send message:",0);
		//logprintf(strMessage,0);
		sendMessage(theCctArgs,strMessage);
	}
}
int changedNickname(int intID,char* strCommand,struct cctArgs* theCctArgs){
	if(NULL==connectedClients)return -1;
	//logprintf("==========>changedNickname",0);
	char strSendTo[NUMBERSTRINGLIMIT];
	char strContent[MESSAGELIMIT];
	char* chr40=strstr(strCommand,"(");
	char* chr44=strstr(strCommand,",");
	char* chr41=strstr(strCommand,")");
	if(NULL==chr40||NULL==chr41){
		return -1;
	}
	int intSendToLength=chr44-chr40-1;
	int intContentLength=chr41-chr44-1;
	strncpy(strSendTo,chr40+1,intSendToLength);
	strncpy(strContent,chr44+1,intContentLength);
	strSendTo[intSendToLength]='\0';
	strContent[intContentLength]='\0';
	int intSendTo=atoi(strSendTo);
	int ii=findConnectedClient_(intID);
	int jj=findConnectedClient_(intSendTo);
	if(-1!=jj){
		//char strLog[LOGLINELIMIT];
		//snprintf(strLog,LOGLINELIMIT,"intSendTo=%d jj=%d \n",intSendTo,jj);
		//logprintf(strLog,0);
		char strMessage[BUFFERSIZE];
		//char strNickname[NICKNAMELIMIT];
		//intContentLength=chr41-chr40-1;
		//strncpy(strNickname,chr40+1,intContentLength);
		//strNickname[intContentLength]='\0';
		//strContent[intContentLength]='\0';
		//snprintf(strMessage,BUFFERSIZE,"interlocutorChangedNickname?intID=%d&strNickname=%s;",intID,strNickname);
		snprintf(strMessage,BUFFERSIZE,"interlocutorChangedNickname?intID=%d&strNickname=%s;",intID,strContent);
		struct cctArgs* cctArgsSendTo=connectedClients[jj]->receivingCctArgs;
		sendMessage(cctArgsSendTo,strMessage);
	}
}
int interpretCommand(char* strBuffer,struct cctArgs* theCctArgs){
	int intID;
	//char strLogMessage[LOGLINELIMIT];
	char strCommand[QUERYSIZE];
	if(-1==getIntIdAndStrCommand(&intID,strCommand,strBuffer,theCctArgs->intClientSocket)){
		return -1;
	}
	//snprintf(strLogMessage,LOGLINELIMIT,"intNumber=|%d|, strCommand=|%s| \n",intID,strCommand);
	//logprintf(strLogMessage,0);
	if(NULL!=strstr(strCommand,"connect(")){
		if(-1==connectCommand(intID,strCommand,theCctArgs)){
			return -1;
		}
	}
	if(NULL!=strstr(strCommand,"receiving(")){
		if(-1==receivingCommand(intID,strCommand,theCctArgs)){
			return -1;
		}
	}
	if(NULL!=strstr(strCommand,"sendTo(")){
		if(-1==sendToCommand(intID,strCommand,theCctArgs)){
			return -1;
		}
	}
	if(NULL!=strstr(strCommand,"nickname(")){
		if(-1==changeNickname(intID,strCommand,theCctArgs)){
			return -1;
		}
	}
	if(NULL!=strstr(strCommand,"changedNickname(")){
		if(-1==changedNickname(intID,strCommand,theCctArgs)){
			return -1;
		}
	}
	return 0;
}
int handleClientConnection(struct cctArgs* theCctArgs){
	char chrBuffer[BUFFERSIZE];
	char strMessage[MESSAGELIMIT];
	//char strLogMessage[LOGLINELIMIT];
	//snprintf(strLogMessage,LOGLINELIMIT,"Connection from %s",theCctArgs->strIP);
	//logprintf(strLogMessage,2);
	ssize_t readBytes=0;
	ssize_t intOffset=0;
	int bln41AlreadyRead=0;
	do{
		if(readBytes==BUFFERSIZE&&1==bln41AlreadyRead)break;
		readBytes=recv(theCctArgs->intClientSocket,chrBuffer,BUFFERSIZE,0);
		if(intOffset+readBytes>MESSAGELIMIT)break;
		strncpy(strMessage+intOffset,chrBuffer,readBytes);
		intOffset+=readBytes;
		strMessage[intOffset]='\0';
		if(NULL!=strstr(chrBuffer,")")){
			bln41AlreadyRead=1;
			//logprintf("handleClientConnection bln41AlreadyRead=1;",2);
		}
		//else logprintf("handleClientConnection bln41AlreadyRead=0;",2);
		//logprintf(chrBuffer,2);
	}while(readBytes==BUFFERSIZE);
	if(readBytes<0){
    		perror("Receive error");
    		logprintf("Receive error",0);
		removeClientSocket(theCctArgs->intClientSocket);
    		close(theCctArgs->intClientSocket);
    		return -1;
	}
    	/*write(STDOUT_FILENO,chrBuffer,readBytes);*/
	//snprintf(strLogMessage,readBytes,"%s",chrBuffer);
	//logprintf("handleClientConnection \n",2);
	//snprintf(strLogMessage,intOffset,"%s",strMessage);
	//logprintf(strLogMessage,2);
	//return interpretCommand(chrBuffer,theCctArgs);
	return interpretCommand(strMessage,theCctArgs);
}
void* ClientConnectionThread(void* theCctArgs){
	pthread_detach(pthread_self());
	handleClientConnection(theCctArgs);
	if(NULL!=theCctArgs)
		deleteCctArgs(theCctArgs);
		theCctArgs=NULL;
	return NULL;
}
void signalHandler(int signalType){
	//char strLogMessage[LOGLINELIMIT];
	//snprintf(strLogMessage,LOGLINELIMIT,"signalType=%d \n",signalType);
	//logprintf(strLogMessage,0);
	freeResources();
}
int main(int argc,char** argv){
	struct sigaction sahandler;
	sahandler.sa_handler=signalHandler;
	if(sigfillset(&sahandler.sa_mask)<0){
		perror("sigfillset error");
		logprintf("sigfillset error",0);
		return EXIT_FAILURE;
	}
	sahandler.sa_flags=0;
	if(sigaction(SIGINT,&sahandler,0)<0){
		perror("sigaction error");
		logprintf("sigaction error",0);
		return EXIT_FAILURE;
	}
	fileLog0=fopen("./TheNetspacesChat0.log","a");
	fileLog1=fopen("./TheNetspacesChat1.log","a");
	int intServerPort=PORT;
	if(2==argc){
		intServerPort=atoi(argv[1]);
	}
	intServerSocket=createServerSocket(intServerPort);
	if(-1==intServerSocket){
		return EXIT_FAILURE;
	}
	connectedClients=(struct clientID**)malloc(CONNECTEDCLIENTSLIMIT*sizeof(struct clientID*));
	pendingConnectionsCctArgs=(struct cctArgs**)malloc(CONNECTEDCLIENTSLIMIT*sizeof(struct cctArgs*));
	intPendingConnections=0;
	for(int ii=0;ii<CONNECTEDCLIENTSLIMIT;ii++){
		connectedClients[ii]=NULL;
	}
	intConnectedClientsLength=0;
	intConnectedClientSockets=(int*)malloc(CONNECTEDCLIENTSLIMIT*sizeof(int));
	intConnectedClientSocketsLength=0;
	pthread_t pThread;
	struct cctArgs* theCctArgs;
	//logprintf("Server started",1);
	//char strLogMessage[LOGLINELIMIT];
	//snprintf(strLogMessage,LOGLINELIMIT,"Server started on %s",ADDRESS);
	//logprintf(strLogMessage,1);
	while(0==0){
		theCctArgs=acceptClientConnection(intServerSocket);	
		if(NULL==theCctArgs){
			close(intServerSocket);
			freeResources();
			return EXIT_FAILURE;

		}
		if(0!=pthread_create(&pThread,NULL,ClientConnectionThread,(void*)theCctArgs)){
			perror("pthread_create error");
			logprintf("pthread_create error",0);
			close(intServerSocket);
			freeResources();
			return EXIT_FAILURE;
		}
		if(intConnectedClientsLength>CONNECTEDCLIENTSLIMIT){
			perror("intConnectedClientsLength>CONNECTEDCLIENTSLIMIT");
			logprintf("intConnectedClientsLength>CONNECTEDCLIENTSLIMIT",0);
			close(intServerSocket);
			freeResources();
			return EXIT_FAILURE;

		}
	}
}
