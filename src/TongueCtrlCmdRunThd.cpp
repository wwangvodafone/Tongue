/*****************************************************************************/
/**
* @file TongueCtrlCmdRun.cpp
* @brief ��̦��������ִ���߳�
*
* ��̦��������ִ���߳�
*
* @author wangwei
* @date 2016/06/06 wangwei ��ʼ�汾
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/
#include <stdio.h>
#include "TongueCtrlCommand.h"
#include "TongueCtrl.h"
#include "TongueCtrlErrProc.h"
#include "TongueCtrlLogThd.h"

/** @brief ����No */
unsigned int g_uiCmdNo;

/**
 * @brief ����ִ�к���
 *
 *  ����ִ�к���
 *
 * @param
 * @return
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
void* CmdRunFunc(void* ptr)
{
	int iRunRslt = ERR_CMD_SUCCESS;

	fprintf(stderr, "CmdRun Thread start.\n");
	g_TongueCtrlLog.I("CmdRun Thread start.");

	while (1) {
		// �ȴ��߳̿�ִ��
		sem_wait(&g_semCmdRun);
		/*����*/
		char pCommand[COMMAND_LENGTH];
		GetCommand(pCommand, g_sInputCommand);
		g_uiCmdNo = GetCommandNo(pCommand);
		g_cmdInfo.uiCmdId = g_uiCmdNo;
		g_cmdInfo.uiCmdType = g_staCmdTable[g_uiCmdNo].uiCmdType;
		// �����﷨������жϴ���
		int iJudgeResult = JudgeCmdConstruct(g_sInputCommand, g_uiCmdNo);
		if (iJudgeResult != ERR_CMD_SUCCESS) {
			g_TongueCtrlLog.E("JudgeCmdConstruct Error. iJudgeResult = %d",iJudgeResult);
			g_cmdInfo.uiRet = ERR_RET;
			g_cmdInfo.uiErrCode = iJudgeResult;
		}
		else {
			/*�õ����������*/
			iJudgeResult = JudgeCmdType(g_uiCmdNo);
			g_TongueCtrlLog.D("Command Type = %d", iJudgeResult);
			if (iJudgeResult == RQ_CMD) {
				iRunRslt = g_staCmdTable[g_uiCmdNo].run(g_cmdInfo.data);
				g_cmdInfo.uiErrCode = iRunRslt;
				g_TongueCtrlLog.D("ErrCode = %d", g_cmdInfo.uiErrCode);
				g_cmdInfo.uiRet = REP_RET;
			}
			else {
	    		if (iJudgeResult != SET_CMD) {
					// ϵͳ״̬ EXEC
					g_stReceptionCond.uiCurSysCmdStatus = COMMAND_EXEC;
	    		}
				g_cmdInfo.uiErrCode = g_staCmdTable[g_uiCmdNo].run(g_cmdInfo.data);
	    		if (g_cmdInfo.uiErrCode == ERR_CMD_SUCCESS) {
	    			g_cmdInfo.uiRet = RDY_RET;
	    		}
	    		else {
	    			g_cmdInfo.uiRet = ERR_RET;
	    		}
	    		g_TongueCtrlLog.D("ErrCode = %d", g_cmdInfo.uiErrCode);
			}
		}


		// �ͷ�����Result�ź���
		sem_post(&g_semCmdResult);
	}

	g_TongueCtrlLog.I("CmdRun Thread End.");
	return (void*)0;
}
