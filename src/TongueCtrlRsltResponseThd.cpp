/*****************************************************************************/
/**
* @file TongueCtrlRsltResponseThd.cpp
* @brief ��̦���������������߳�
*
* ��̦���������������߳�
*
* @author wangwei
* @date 2016/06/06 wangwei ��ʼ�汾
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#include "TongueCtrl.h"
#include "TongueCtrlCommand.h"
#include "TongueCtrlErrProc.h"
#include "TongueCtrlLogThd.h"

const CmdRes_St g_CmdRes[] = {
		{DUMMY_RET,	""},
		{RDY_RET, 	"_RDY"},
		{ERR_RET,	"_ERR"},
		{REP_RET,	""}
};

CLIENT_TYPE g_clientFlag = CT_NONE;

void WriteToTerminal(RetMessage_En in_enRetMsg, const char* in_pMsg);

/**
 * @brief ����ִ��Result�̺߳���
 *
 *  ����ִ��Result�̺߳���
 *
 * @param
 * @return
 * @author wangwei
 * @date 2016/06/06 wangwei �V�K�쐬
 */
void* RsltResponseFunc(void* ptr)
{
	char aErrorCode[5];

	fprintf(stderr, "RsltResponse Thread start.\n");
	g_TongueCtrlLog.I("RsltResponseFunc Start");

	while (1) {
		// �ȴ�����ִ��
		sem_wait(&g_semCmdResult);
		sprintf(aErrorCode, "%05d", getErrId(g_cmdInfo.uiErrCode));
		g_TongueCtrlLog.D("g_cmdInfo.uiRet = %d, g_cmdInfo.uiErrCode = %d\n", g_cmdInfo.uiRet, g_cmdInfo.uiErrCode);
		if (g_cmdInfo.uiRet == ERR_RET) {
			if (g_cmdInfo.uiErrCode != ERR_CMD_SUCCESS) {
				sprintf(aErrorCode, "%05d", getErrId(g_cmdInfo.uiErrCode));
				WriteToTerminal(ERR_RET, aErrorCode);
			}
		}
		else if (g_cmdInfo.uiRet == RDY_RET) {
			if (g_cmdInfo.uiErrCode == ERR_CMD_SUCCESS) {
				WriteToTerminal(RDY_RET, NULLCHAR);
			}
		}
		else if (g_cmdInfo.uiRet == REP_RET) {
			if (g_cmdInfo.uiErrCode == ERR_CMD_SUCCESS) {
				/*Ӧ������*/
				WriteToTerminal(REP_RET, g_cmdInfo.data);
			}
			else {
				sprintf(aErrorCode, "%05d", getErrId(g_cmdInfo.uiErrCode));
				WriteToTerminal(ERR_RET, aErrorCode);
			}
		}


//		if (g_cmdInfo.uiErrCode == ERR_CMD_SUCCESS) {
//			// ϵͳ״̬ ����ΪREADY
//			g_stReceptionCond.uiCurSysCmdStatus = SYSTEM_READY;
//		}
//		else {
//			if ((g_stReceptionCond.uiCurSysCmdStatus == COMMAND_EXEC)) {
//				g_stReceptionCond.uiCurSysCmdStatus = SYSTEM_ERR;
//			}
//		}
		g_stReceptionCond.uiCurSysCmdStatus = SYSTEM_READY;
		g_stReceptionCond.uiCurDevStatus = DUMMY_STATUS;
		printf("Response end to READY.\n");
	}
	g_TongueCtrlLog.I("RsltResponseFunc End");
//	printf("systemCmdStatus: %d\n", g_stReceptionCond.uiCurSysCmdStatus);

	return (void*)0;
}

/**
 * @brief  TCP������Ӧ��
 *
 *  TCP������Ӧ��
 *
 * @param in_enRetMsg Ӧ������
 * @param in_pMsg 	       Ӧ��Message
 * @return
 * @author wangwei
 * @date 2016/06/06 wangwei �V�K�쐬
 */
void WriteToTerminal(RetMessage_En in_enRetMsg, const char* in_pMsg)
{
	char* p;
	char* q;
	int nbytes = 0;
	char aResMsg[RSPNAME_LENGTH + ERRNAME_LENGTH + RESPONSE_LENGTH];

	strncpy(aResMsg, g_CmdRes[in_enRetMsg].aCmdResName, RSPNAME_LENGTH);
	if (strcmp(in_pMsg, NULLCHAR)) { /*����NULL*/
		if (in_enRetMsg != REP_RET) {
			strncat(aResMsg, BLANKCHAR, 2);
		}
		strncat(aResMsg, in_pMsg, RSPNAME_LENGTH + ERRNAME_LENGTH + RESPONSE_LENGTH);
	}

	p = aResMsg;

	q = p;
	while (*p++ != '\0') nbytes++;
	--p;
	*p++ = CR;

	*p = '\0';
	p = q;

	printf("g_clientFlag=%d\n", g_clientFlag);

	if (g_clientFlag == CT_FROM_TCPIP) {
		send(g_clientTcpIpCommfd, q, nbytes+1/*TCPPACKETSIZE*/, 0 );
		g_clientFlag = CT_NONE;
		printf("Response: %s\n", q);
		// Network semaphore
		sem_post(&g_semNetwork);
	}
	else if (g_clientFlag == CT_FROM_UDP) {
		if ((g_stReceptionCond.uiCurDevStatus != DEVICE_EXEC) && (g_stReceptionCond.uiCurSysCmdStatus != DEVICE_EXEC)) {	// �豸��������
			sendto(g_udpComm.socket, q, nbytes+1, 0, (struct sockaddr *)&g_udpComm.sin,sizeof(g_udpComm.sin));
		}
		g_clientFlag = CT_NONE;
		printf("Response: %s\n", q);
		// Network semaphore
		sem_post(&g_semUdpComm);
	}
	else if (g_clientFlag == CT_FROM_BTIPC) {
		send(g_clientBTIpcCommfd, q, nbytes+1, 0);
		g_clientFlag = CT_NONE;
		sem_post(&g_semBlueTooth);
	}
	else {
		g_TongueCtrlLog.I("WriteToTerminal Unkown g_clientFlag");
	}

}
