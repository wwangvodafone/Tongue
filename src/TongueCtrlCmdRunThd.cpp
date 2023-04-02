/*****************************************************************************/
/**
* @file TongueCtrlCmdRun.cpp
* @brief 舌苔控制命令执行线程
*
* 舌苔控制命令执行线程
*
* @author wangwei
* @date 2016/06/06 wangwei 初始版本
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/
#include <stdio.h>
#include "TongueCtrlCommand.h"
#include "TongueCtrl.h"
#include "TongueCtrlErrProc.h"
#include "TongueCtrlLogThd.h"

/** @brief 命令No */
unsigned int g_uiCmdNo;

/**
 * @brief 命令执行函数
 *
 *  命令执行函数
 *
 * @param
 * @return
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
void* CmdRunFunc(void* ptr)
{
	int iRunRslt = ERR_CMD_SUCCESS;

	fprintf(stderr, "CmdRun Thread start.\n");
	g_TongueCtrlLog.I("CmdRun Thread start.");

	while (1) {
		// 等待线程可执行
		sem_wait(&g_semCmdRun);
		/*命令*/
		char pCommand[COMMAND_LENGTH];
		GetCommand(pCommand, g_sInputCommand);
		g_uiCmdNo = GetCommandNo(pCommand);
		g_cmdInfo.uiCmdId = g_uiCmdNo;
		g_cmdInfo.uiCmdType = g_staCmdTable[g_uiCmdNo].uiCmdType;
		// 命令语法错误的判断处理
		int iJudgeResult = JudgeCmdConstruct(g_sInputCommand, g_uiCmdNo);
		if (iJudgeResult != ERR_CMD_SUCCESS) {
			g_TongueCtrlLog.E("JudgeCmdConstruct Error. iJudgeResult = %d",iJudgeResult);
			g_cmdInfo.uiRet = ERR_RET;
			g_cmdInfo.uiErrCode = iJudgeResult;
		}
		else {
			/*得到命令的种类*/
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
					// 系统状态 EXEC
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


		// 释放命令Result信号量
		sem_post(&g_semCmdResult);
	}

	g_TongueCtrlLog.I("CmdRun Thread End.");
	return (void*)0;
}
