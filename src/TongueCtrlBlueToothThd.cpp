/*****************************************************************************/
/**
* @file TongueCtrlBlueToothThd.cpp
* @brief 
*
*
* @author wangyp
* @date 2016/07/06 wangyp
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/

#include <stdio.h>
#include <stdlib.h> //system
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h> //inet_ntoa
#include "TongueCtrl.h"

enum  BT_STATE{
	BT_NONE,
	BT_DISCOVERABLE,
	BT_PAIRED,
	BT_DISCONNECTED,
	BT_CONNECTED,
	// the followings are not state but for debug
	BT_INFO,
	BT_STATE_MAX	
};

static BT_STATE g_state = BT_NONE;
const int discoveryDefaultTimeout = 180; // default value, 3 minutes
BT_STATE noneAction(void) {
	printf("make it discoverable\n");
	system("bt-adapter -s Discoverable 1");
	return BT_DISCOVERABLE;
}
BT_STATE discoverableAction(void) {
	static int discoverTimeout = discoveryDefaultTimeout; 
	if (--discoverTimeout > 0) {
		return BT_DISCOVERABLE;
	} else {
		printf("back to none\n");
		discoverTimeout = discoveryDefaultTimeout;
		return BT_NONE;
	}
}
BT_STATE pairedAction() {
	return BT_PAIRED;
}
BT_STATE disconnectedAction() {
	return BT_DISCONNECTED;
}
BT_STATE connectedAction() {
	return BT_CONNECTED;
}
BT_STATE infoAction() {
	system("bt-adapter -i");	
	return BT_INFO;
}
void btStateChange(BT_STATE old, BT_STATE news) {
	printf("BT state change: %d -> %d\n", old, news);
}

typedef BT_STATE (*btAction)(void);

btAction actionArrays[BT_STATE_MAX] = {
	&noneAction,
	&discoverableAction,
	&pairedAction,
	&disconnectedAction,
	&connectedAction,
	&infoAction
};
void btRunStateMachine(BT_STATE curState)
{
	BT_STATE newState = actionArrays[curState]();
	if (newState != curState) {
		btStateChange(curState, newState);
		g_state = newState;
	}
}

// There is socket to RFCOMMSRV to get the request and send response to the Mobile Device
// Currently we are using localhost:10008 
enum IPC_STATE {
	IPC_NONE,
	IPC_CONNECTED,
	IPC_FAILED,    // that is fatal, we may need to kill and restart RFCOMMSRV, even send a request for system reboot.
	IPC_STATE_MAX
};
static IPC_STATE g_ipc = IPC_NONE;
typedef IPC_STATE (*ipcAction)(void);

IPC_STATE ipcNoneAction(void) {
	return IPC_NONE;
}
IPC_STATE ipcConnectedAction(void) {
	return IPC_CONNECTED;
}
IPC_STATE ipcFailedAction(void) {
	return IPC_FAILED;
}
ipcAction ipcActionArray[IPC_STATE_MAX] = {
	&ipcNoneAction,
	&ipcConnectedAction,
	&ipcFailedAction
};
void ipcStateChange(IPC_STATE old, IPC_STATE news) {
	printf("IPC state change: %d -> %d\n", old, news);
}
void ipcRunStateMachine(IPC_STATE curState)
{
	IPC_STATE newState = ipcActionArray[curState]();
	if (newState != curState) {
		ipcStateChange(curState, newState);
		g_ipc = newState;
	}
}

typedef void (*timerCallBackFunc)(int signal);

void setTimeCallBack(timerCallBackFunc cb, int delay, int repeat)
{
	int res = -1;
	// Register the
	signal(SIGALRM, cb);
	struct itimerval tick;

	// Initialize struct
	memset(&tick, 0, sizeof(tick));
	tick.it_value.tv_sec = delay;
	tick.it_value.tv_usec = 0;

	// Interval time to run function
	tick.it_interval.tv_sec = repeat;
	tick.it_interval.tv_usec = 0;

	res = setitimer(ITIMER_REAL, &tick, NULL);
	if (res) {
		printf("Set timer failed!!/n");
	}	
}

// called by other thread and when done, call sem_post to wake up Bluetooth thread if it sleep
void sendMsg(void* msg)
{
	(void)msg;
	sem_post(&g_semBlueTooth);
}

const int hb_delay = 1; // delay 1 second to start (only once)
const int hb_repeat = 1;  // repeat every 1 second..
int g_clientBTIpcCommfd = 0;  // used by RsltResponse thread to send the response back
void heartBeat(int signal)
{
	btRunStateMachine(g_state);
	ipcRunStateMachine(g_ipc);
	if (g_clientBTIpcCommfd < 0) {
		//retry
		sem_post(&g_semBlueTooth);
	}
}


int connectBTIPC(const char* host, const char* port)
{
	struct addrinfo hints, *res, *ressave;
	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_family=AF_INET; //AF_UNSPEC;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_protocol=IPPROTO_TCP;
	int ret = getaddrinfo(host, port, &hints, &res);
	if (ret != 0) {
		printf("BTIPC getaddrinfo for %s:%s failed\n", host, port);
		return -1;
	}
	ressave=res;
	int sock = -1;
	do {
		sock=socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (sock < 0) {
			printf("BTIPC socket error for %d %d %d\n", 
			  res->ai_family, res->ai_socktype, res->ai_protocol);
			return -1;
		}
		ret = connect(sock, res->ai_addr, res->ai_addrlen);
		if (ret != 0) {
			perror("connect");
			struct sockaddr_in* addr = (struct sockaddr_in* )res->ai_addr;
			printf("BTIPC connect to %s:%d error\n", 
			  inet_ntoa(addr->sin_addr),  (int) ntohs(addr->sin_port));
			close(sock);
			return -1;
		}
	} while(0); // (res=res->ai_next)!= NULL  // we only use the first one.
	freeaddrinfo(ressave);
	
	// reading banner... just to make sure server is ok.
	char banner[128];
    int bytes = read(sock, banner, 127);
	if (bytes <= 0) {
		printf("BTIPC failed to reading banner\n");
		close(sock);
		return -1;
	}
	banner[bytes] = 0;
	printf("BTIPC read banner: %s\n", banner);

	// save to global
	g_clientBTIpcCommfd = sock;
	return sock;
}
int readBTCommandFromIPC(int sock)
{
	if (sock < 0) return -1;
	if (g_clientFlag != CT_NONE) {
		printf(" WARNING: BTIPC may overwrite the message\n");
	}
    memset(g_sInputCommand, '\0', sizeof(g_sInputCommand));
	int bytes = read(sock, g_sInputCommand, sizeof(g_sInputCommand)-1);
	if (bytes <= 0) {
		printf("BTIPC read error. The connection is down\n");
		return -1;
	}
	g_sInputCommand[bytes] = 0;
	fprintf(stderr, "BTIPC input command: %s\n", g_sInputCommand);
	return 0;	
}
void sendBTCommand(void) 
{
	g_clientFlag = CT_FROM_BTIPC;
	sem_post(&g_semCmdRun);	
}
void* BlueToothFunc(void *ptr)
{

//	setTimeCallBack(&heartBeat, hb_delay, hb_repeat);

	while (1) {
		int sock = connectBTIPC("localhost", "10008");
		while(readBTCommandFromIPC(sock) == 0) {
			sendBTCommand();
			//waiting for response done.
			sem_wait(&g_semBlueTooth); 
		}
		if(sock > 0) close(sock);
		g_clientBTIpcCommfd = -1;
		// wait heartbeat for recovery/reconnect
		sem_wait(&g_semBlueTooth);
		//printf("In BT thread\n");
	}

	return (void*)0;
}



