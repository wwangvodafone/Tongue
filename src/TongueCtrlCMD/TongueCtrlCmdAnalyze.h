/*****************************************************************************/
/**
* @file TongueCtrlCmdAnalyze.h
* @brief 舌苔控制命令解析
*
* 解析舌苔控制命令语法
*
* @author wangwei
* @date 2016/06/06 wangwei 初始版本
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/
#ifndef TONGUECTRLCMD_ALIGNCTRLCMDANALYZE_H_
#define TONGUECTRLCMD_ALIGNCTRLCMDANALYZE_H_

#include "TongueCtrlCommand.h"

extern int AnalyzeCmd(COMMAND_NUM COMMAND, char* in_pParameter);
extern int JudgeCmdReception(COMMAND_NUM COMMAND);

#endif /* TONGUECTRLCMD_ALIGNCTRLCMDANALYZE_H_ */
