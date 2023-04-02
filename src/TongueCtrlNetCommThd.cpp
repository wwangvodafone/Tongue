/*****************************************************************************/
/**
* @file TongueCtrlNetCommThd.h
* @brief ��̦�������������߳�
*
* ��̦�������������߳�
*
* @author wangwei
* @date 2016/06/07 wangwei ��ʼ�汾
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/
#include "TongueCtrlNetCommThd.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#include "TongueCtrl.h"
#include "TongueCtrlCommand.h"
#include "TongueCtrlLogThd.h"

// ��������
char g_sInputCommand[COMMAND_LENGTH];
// accept socket
int g_clientTcpIpCommfd;

static bool DoReceiveMessage(int iSocket);
static void StartCmdRun(char* command);


/**
 * @brief Network�̺߳���
 *
 *  Network�̺߳���
 *
 * @param
 * @return
 * @author wangwei
 * @date 2015/06/07 wangwei ��ʼ�汾
 */
void* NetCommFunc(void* ptr)
{
	g_TongueCtrlLog.I("NetCommFunc Start");

	int lSocket;
	struct sockaddr_in sLocalAddr;
	struct sockaddr_in client_addr;
	int addrlen=sizeof(client_addr);
	int status;
	int on;

	lSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (lSocket < 0) {
		fprintf(stderr, "Create socket failed.\n");
		g_TongueCtrlLog.E("Create socket failed.");
		return (void*)0;
	}
	on = 1;
	setsockopt( lSocket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );
	memset((char *)&sLocalAddr, 0, sizeof(sLocalAddr));
	sLocalAddr.sin_family = AF_INET;
	sLocalAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sLocalAddr.sin_port = htons(g_commonSysParam.uiPortMaster);

	status = bind(lSocket, (struct sockaddr *)&sLocalAddr, sizeof(sLocalAddr));
	if (status < 0) {
		fprintf(stderr, "Bind socket failed.\n");
		g_TongueCtrlLog.E("Bind socket failed.");
		close(lSocket);
		return (void*)0;
	}

	if (listen(lSocket, 5) != 0) {
		fprintf(stderr, "Listen socket failed.\n");
		g_TongueCtrlLog.E("Listen socket failed.");
		close(lSocket);
		return (void*)0;
	}

	fprintf(stderr, "NetComm Thread start.\n");
	g_TongueCtrlLog.I("NetComm Thread start.");

	while (1) {
		g_clientTcpIpCommfd = accept(lSocket, (struct sockaddr*)&client_addr, (socklen_t*)&addrlen);
		if (g_clientTcpIpCommfd < 0) {
			close(lSocket);
			g_TongueCtrlLog.E("accept Error.");
			return (void*)0;
		}
		for (;;) {
			bool bResult = DoReceiveMessage(g_clientTcpIpCommfd);
			if (!bResult) {
				g_TongueCtrlLog.E("DoReceiveMessage Error.");
				break;
			}
			// �ȴ�Network�ź����ͷ�
			sem_wait(&g_semNetwork);
		}
		close(g_clientTcpIpCommfd);
	}

	g_TongueCtrlLog.I("NetCommFunc End");

}

/**
 * @brief Message����
 *
 *  Message����
 *
 * @param iSocket ����Socket
 * @return true:�ɹ�  false:ʧ��
 * @author wangwei
 * @date 2015/06/07 wangwei ��ʼ�汾
 */
bool DoReceiveMessage(int iSocket)
{
	g_TongueCtrlLog.D("DoReceiveMessage Start(iSocket = %d).", iSocket);

    bool bFlag = true;
    char inputChar;
    bool bTimerStartFlag = true;
    int iRecv = 0;

    memset(g_sInputCommand, '\0', sizeof(g_sInputCommand));
	int i = 0;

    while (bFlag) {
    	iRecv = recv(iSocket, &inputChar, 1, 0);
    	if (iRecv <= 0) {	// error or close
    		bFlag = false;
    		break;
    	}
    	if (bTimerStartFlag) {
    		bTimerStartFlag = false;
    	}

    	if (inputChar == 0x0D) {
    		g_sInputCommand[i++] = inputChar;
			break;
    	}
    	g_sInputCommand[i++] = inputChar;
		if (i == COMMAND_LENGTH) {
			g_TongueCtrlLog.E("i = COMMAND_LENGTH");
			bFlag = false;
		}
    }
    if (bFlag == true) {
		g_sInputCommand[i] = '\0';
		CutNetDelimiter(g_sInputCommand);
		fprintf(stderr, "Net input command: %s\n", g_sInputCommand);
		g_TongueCtrlLog.D("Net input command: %s", g_sInputCommand);

		// Command
		StartCmdRun(g_sInputCommand);
    }

    g_TongueCtrlLog.D("DoReceiveMessage End(return = %d).", bFlag);
    return bFlag;
}

/**
 * @brief ����ִ������
 *
 *  ����ִ������
 *
 * @param iSocket ����Socket
 * @return
 * @author wangwei
 * @date 2015/06/07 wangwei ��ʼ�汾
 */
void StartCmdRun(char* command)
{
	// �ͷ�����ִ���ź���
	g_clientFlag = CT_FROM_TCPIP;
	sem_post(&g_semCmdRun);

}


/**
 * @brief �г�����Ķ����
 *
 * �г�����Ķ����
 *
 * @param	io_sInput	�ַ���
 * @return	True: ��ȷ	False:����
 * @author wangwei
 * @date 2015/06/07 wangwei ��ʼ�汾
 */
bool CutNetDelimiter(char* io_sInput)
{
	bool bResult = false;
	char *p = io_sInput;

	while (*p != 0x0D && *p != '\0') p++;
	if (*p == 0x0D) {
		*p = '\0';
		bResult = true;
	}

	return bResult;
}
