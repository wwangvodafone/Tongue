//============================================================================
// Name        : TongueCtrl.cpp
// Author      : ubky
// Version     :
// Copyright   : Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

//#include <TongueCtrlCamThd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "TongueCtrl.h"
#include "TongueCtrlInitial.h"
#include "TongueCtrlCmdRunThd.h"
#include "TongueCtrlNetCommThd.h"
#include "TongueCtrlRsltResponseThd.h"
#include "TongueCtrlCamThd.h"
#include "TongueCtrlLogThd.h"
#include "TongueCtrlSysMonitorThd.h"
#include "TongueCtrlMsgThd.h"
#include "TongueCtrlBlueToothThd.h"
#include "TongueCtrlUdpCommThd.h"
#include "TongueCtrlPlaySound.h"

static int CheckArg(char **argv, TongueCtrlParam_En& TongueCtrlParam);
static void CreateDeamon(void);

/**
 * @brief Main函数
 *
 * Main函灵敏
 *
 * @param
 * @return
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int main(int argc, char* argv[]) {
	pthread_t	camThread;		//
	pthread_t	netCommThread;
	pthread_t	cmdRunThread;
	pthread_t 	rsltResponseThread;
	pthread_t 	LogFileSaveThread;
	pthread_t   sysMonitorThread;
	pthread_t 	msgQueueThread;
	pthread_t	videoThread;
	pthread_t   blueToothThread;
	pthread_t   udpThread;
	pthread_t   playSoundThread;
	TongueCtrlParam_En tongueCtrlParam;

//	CreateDeamon();

	InitParameter(&tongueCtrlParam);

	pthread_create(&camThread, NULL, CamProcFunc,  (void*)&tongueCtrlParam);
	pthread_create(&netCommThread, NULL, NetCommFunc,  NULL);
	pthread_create(&cmdRunThread, NULL, CmdRunFunc,  NULL);
	pthread_create(&rsltResponseThread, NULL, RsltResponseFunc,  NULL);
	pthread_create(&LogFileSaveThread, NULL, LogFileSaveFunc,  NULL);
	pthread_create(&sysMonitorThread, NULL, SysMonitorFunc, NULL);
	pthread_create(&msgQueueThread, NULL, CtrlMsgFunc, NULL);
	pthread_create(&udpThread, NULL, UdpCommFxn, NULL);
//	pthread_create(&blueToothThread, NULL, BlueToothFunc, NULL);
//	pthread_create(&playSoundThread, NULL, CtrlSoundFunc, NULL);

	pthread_join(netCommThread, NULL);
	fprintf(stderr, "netCommThread finished.\n");
	g_TongueCtrlLog.I("netCommThread finished.");

	pthread_join(camThread, NULL);
	fprintf(stderr, "camThread finished.\n");
	g_TongueCtrlLog.I("camThread finished.");

	pthread_join(cmdRunThread, NULL);
	fprintf(stderr, "cmdRunThread finished.\n");
	g_TongueCtrlLog.I("cmdRunThread finished.");

	pthread_join(rsltResponseThread, NULL);
	fprintf(stderr, "rsltResponseThread finished.\n");
	g_TongueCtrlLog.I("rsltResponseThread finished.");

	pthread_join(LogFileSaveThread, NULL);
	fprintf(stderr, "LogFileSaveThread finished.\n");
	g_TongueCtrlLog.I("LogFileSaveThread finished.");

	fprintf(stderr, "TonguementCtrl finished.\n");
	g_TongueCtrlLog.I("TonguementCtrl finished.");



	return EXIT_SUCCESS;
}

int CheckArg(char **argv, TongueCtrlParam_En& TongueCtrlParam)
{
	int iResult = -1;

	if (strcmp("-n", argv[0])) { // no preview;
		TongueCtrlParam.preview = 1;
		iResult = 0;
	}
	if (!strcmp("-w", argv[0])) {
		sscanf(argv[1], "%d", &TongueCtrlParam.iResolutionW);
		iResult = 0;
	}
	else if (!strcmp("-h", argv[0])) {
		sscanf(argv[1], "%d", &TongueCtrlParam.iResolutionH);
		iResult = 0;
	}
	else if (!strcmp("-r", argv[0])) {
		sscanf(argv[1], "%f", &TongueCtrlParam.fRoiStartX);
		sscanf(argv[2], "%f", &TongueCtrlParam.fRoiStartY);
		sscanf(argv[3], "%f", &TongueCtrlParam.fRoiLenX);
		sscanf(argv[4], "%f", &TongueCtrlParam.fRoiLenY);
		iResult = 0;
	}
	else if (!strcmp("-c", argv[0])) {
		sscanf(argv[1], "%d", &TongueCtrlParam.iCropX);
		sscanf(argv[2], "%d", &TongueCtrlParam.iCropY);
		sscanf(argv[3], "%d", &TongueCtrlParam.iCropWidth);
		sscanf(argv[4], "%d", &TongueCtrlParam.iCropHeight);
		iResult = 0;
	}
	else if (!strcmp("-m", argv[0])) {
		sscanf(argv[1], "%d", &TongueCtrlParam.iSensorMode);
		iResult = 0;
	}

	return iResult;
}

/**
 * @brief 守护进程创建
 *
 * 守护进程创建
 *
 * @param
 * @return
 * @author wangwei
 * @date 2016/06/08 wangwei 初始版本
 */
void CreateDeamon(void)
{
    pid_t pid;
    pid = fork();
    if( pid == -1)
        exit(EXIT_FAILURE);
    if(pid > 0 )
        exit(EXIT_SUCCESS);
    if(setsid() == -1)
        exit(EXIT_FAILURE);
    chdir("/");
    int i;
    for( i = 0; i < _SC_OPEN_MAX; ++i)
    {
        close(i);
        open("/dev/null", O_RDWR);
        dup(0);
        dup(0);
    }
    umask(0);
    return;
}
