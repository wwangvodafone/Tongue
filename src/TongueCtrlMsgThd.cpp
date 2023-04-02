/*****************************************************************************/
/**
* @file TongueCtrlMsg.cpp
* @brief ��̦������Ϣ����
*
* ��̦������Ϣ����
*
* @author wangwei
* @date 2016/07/07 wangwei ��ʼ�汾
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>

#include "TongueCtrl.h"
#include "TongueCtrlCommand.h"
#include "TongueCtrlErrProc.h"
#include "TongueCtrlLogThd.h"
#include "TongueCtrlUtility.h"
#include "TongueCtrlGPIO.h"

#define MSGKEY 1024
#define BUFFER_SIZE 50

struct msgstru
{
   long msgtype;
   char msgtext[2048];
};

char* g_pNameAddr;

/**
 * @brief ��Ϣ���п��ƺ���
 *
 *  ��Ϣ���п��ƺ���
 *
 * @param
 * @return
 * @author wangwei
 * @date 2016/07/07 wangwei ��ʼ�汾
 */
void* CtrlMsgFunc(void* ptr)
{
	struct msgstru msgs;
	int ret_value;
	int msqid;

	msqid=msgget(MSGKEY,IPC_EXCL);  /*�����Ϣ�����Ƿ����*/
	if(msqid < 0) {
		msqid = msgget(MSGKEY,IPC_CREAT|0666);/*������Ϣ����*/
		if(msqid <0){
			g_TongueCtrlLog.E("failed to create msq | errno=%d [%s]\n",errno,strerror(errno));

		}
	}

	while (1) {
		// �ȴ���Ϣ�����ź����ͷ�
		sem_wait(&g_semMsgQueue);
		msgs.msgtype = 1;
		strcpy(msgs.msgtext, "1");
		/* ������Ϣ���� */
		ret_value = msgsnd(msqid,&msgs,sizeof(struct msgstru),IPC_NOWAIT);
		if ( ret_value < 0 ) {
			g_TongueCtrlLog.E("msgsnd() write msg failed,errno=%d[%s]\n",errno,strerror(errno));
			break;
		}
	}

	msgctl(msqid,IPC_RMID,0); //ɾ����Ϣ����

	return (void*)0;
}

/**
 * @brief ��ʱ������
 *
 * ��ʱ������
 *
 * @param
 * @author wangwei
 * @date 2016/09/07 wangwei ��ʼ�汾
 **/
void TimerSet(char* pNameAddr, int sec)
{
//	int res = 0;
//
//	g_pNameAddr = pNameAddr;
//
//	signal(SIGALRM, CamVideoIsr);
//	struct itimerval tick;
//
//	// Initialize struct
//	memset(&tick, 0, sizeof(tick));
//	tick.it_value.tv_sec = sec;
//	tick.it_value.tv_usec = 0;
//
//	// Interval time to run function
//	tick.it_interval.tv_sec = sec;
//	tick.it_interval.tv_usec = 0;
//
//	res = setitimer(ITIMER_REAL, &tick, NULL);
//	if (res) {
//		printf("Set timer failed!!/n");
//	}

	return;
}

/**
 * @brief �����ʱ����
 *
 * �����ʱ����
 *
 * @param
 * @author wangwei
 * @date 2016/09/07 wangwei ��ʼ�汾
 **/
void CamVideoIsr(int i)
{
	g_pNameAddr = g_stCommand.unCommand.stCmdMovie.aNameAddr;
	printf("g_sInputCommand=%s\n", g_sInputCommand);
	if (g_camStatus == DEV_CAM_VIDEO_START) {	// ����ʼ����ʱ����quit���
		printf("g_sInputCommand=%s\n", g_sInputCommand);
		strncpy(g_sInputCommand, "MOVIE ", COMMAND_LENGTH);
		printf("g_sInputCommand=%s\n", g_sInputCommand);
		strncat(g_sInputCommand, g_pNameAddr, COMMAND_LENGTH);
		printf("g_sInputCommand=%s\n", g_sInputCommand);
		strncat(g_sInputCommand, " 3", COMMAND_LENGTH);

		printf("%s\n", g_sInputCommand);
		g_stReceptionCond.uiCurDevStatus = DEVICE_EXEC;
		g_stReceptionCond.uiCurSysCmdStatus = DEVICE_EXEC;
		g_TongueCtrlLog.I("Dev Movie Timeout.");
		printf("Dev Movie Timeout.\n");
		SwitchLightOnOff(ILLUME_LIGHT, 0);


		sem_post(&g_semCmdRun);
	}

}
