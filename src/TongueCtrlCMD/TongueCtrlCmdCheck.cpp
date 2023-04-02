/*****************************************************************************/
/**
* @file TongueCtrlCmdCheck.cpp
* @brief 舌苔控制命令语法检测
*
* 检测舌苔控制命令语法
*
* @author wangwei
* @date 2016/06/06 wangwei 初始版本
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/
#include "../TongueCtrlErrProc.h"
#include "TongueCtrlCmdAnalyze.h"

/**
 * @brief DUMMY命令解析
 *
 * 检查语法
 *
 * @param pCommand 命令
 * @return 0:正常  1:异常
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int	CheckDummyCmd(char* in_pCommand)
{
	return 0;
}

/**
 * @brief SHUTDOWN命令解析
 *
 * 检查语法
 *
 * @param pCommand 命令
 * @return 0:正常  1:异常
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int	CheckShutdownCmd(char* in_pCommand)
{
	int iResult = -1;
	iResult = AnalyzeCmd(CMD_SHUTDOWN, in_pCommand);
	if (iResult == ERR_CMD_SUCCESS) {
		iResult = JudgeCmdReception(CMD_SHUTDOWN);
	}
	return iResult;
}

/**
 * @brief RESET命令解析
 *
 * 检查语法
 *
 * @param pCommand 命令
 * @return 0:正常  1:异常
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int	CheckResetCmd(char* in_pCommand)
{
	int iResult = -1;
	iResult = AnalyzeCmd(CMD_RESET, in_pCommand);
	if (iResult == ERR_CMD_SUCCESS) {
		iResult = JudgeCmdReception(CMD_RESET);
	}
	return iResult;
}

/**
 * @brief REBOOT命令解析
 *
 * 检查语法
 *
 * @param pCommand 命令
 * @return 0:正常  1:异常
 * @author wangwei
 * @date 2016/12/27 wangwei 初始版本
 */
int	CheckRebootCmd(char* in_pCommand)
{
	int iResult = -1;
	iResult = AnalyzeCmd(CMD_REBOOT, in_pCommand);
	if (iResult == ERR_CMD_SUCCESS) {
		iResult = JudgeCmdReception(CMD_REBOOT);
	}
	return iResult;
}

/**
 * @brief LIGHT命令解析
 *
 * 检查语法
 *
 * @param pCommand 命令
 * @return 0:正常  1:异常
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int	CheckLightCmd(char* in_pCommand)
{
	int iResult = -1;
	iResult = AnalyzeCmd(CMD_LIGHT, in_pCommand);
	if (iResult == ERR_CMD_SUCCESS) {
		iResult = JudgeCmdReception(CMD_LIGHT);
	}
	return iResult;
}

/**
 * @brief PICTURE命令解析
 *
 * 检查语法
 *
 * @param pCommand 命令
 * @return 0:正常  1:异常
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int	CheckPictureCmd(char* in_pCommand)
{
	int iResult = -1;
	iResult = AnalyzeCmd(CMD_PICTURE, in_pCommand);
	if (iResult == ERR_CMD_SUCCESS) {
		iResult = JudgeCmdReception(CMD_PICTURE);
	}
	return iResult;
}

/**
 * @brief MOVIE命令解析
 *
 * 检查语法
 *
 * @param pCommand 命令
 * @return 0:正常  1:异常
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int	CheckMovieCmd(char* in_pCommand)
{
	int iResult = -1;
	iResult = AnalyzeCmd(CMD_MOVIE, in_pCommand);
	if (iResult == ERR_CMD_SUCCESS) {
		iResult = JudgeCmdReception(CMD_MOVIE);
	}
	return iResult;
}


/**
 * @brief LOAD命令
 *
 * 检查语法
 *
 * @param pCommand 命令
 * @return 0:正常  1:异常
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int	CheckLoadCmd(char* in_pCommand)
{
	int iResult = -1;
	iResult = AnalyzeCmd(CMD_LOAD, in_pCommand);
	if (iResult == ERR_CMD_SUCCESS) {
		iResult = JudgeCmdReception(CMD_LOAD);
	}
	return iResult;
}

/**
 * @brief SAVE命令
 *
 * 检查语法
 *
 * @param pCommand 命令
 * @return 0:正常  1:异常
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int	CheckSaveCmd(char* in_pCommand)
{
	int iResult = -1;
	iResult = AnalyzeCmd(CMD_SAVE, in_pCommand);
	if (iResult == ERR_CMD_SUCCESS) {
		iResult = JudgeCmdReception(CMD_SAVE);
	}
	return iResult;
}

/**
 * @brief UPLOAD命令解析
 *
 * 检查语法
 *
 * @param pCommand 命令
 * @return 0:正常  1:异常
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int	CheckUploadCmd(char* in_pCommand)
{
	int iResult = -1;
	iResult = AnalyzeCmd(CMD_UPLOAD, in_pCommand);
	if (iResult == ERR_CMD_SUCCESS) {
		iResult = JudgeCmdReception(CMD_UPLOAD);
	}
	return iResult;
}

/**
 * @brief SYNC命令解析
 *
 * 检查语法
 *
 * @param pCommand 命令
 * @return 0:正常  1:异常
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int	CheckSyncCmd(char* in_pCommand)
{
	int iResult = -1;
	iResult = AnalyzeCmd(CMD_SYNC, in_pCommand);
	if (iResult == ERR_CMD_SUCCESS) {
		iResult = JudgeCmdReception(CMD_SYNC);
	}
	return iResult;
}

/**
 * @brief DELETE命令解析
 *
 * 检查语法
 *
 * @param pCommand 命令
 * @return 0:正常  1:异常
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int	CheckDeleteCmd(char* in_pCommand)
{
	int iResult = -1;
	iResult = AnalyzeCmd(CMD_DELETE, in_pCommand);
	if (iResult == ERR_CMD_SUCCESS) {
		iResult = JudgeCmdReception(CMD_DELETE);
	}
	return iResult;
}

/**
 * @brief SET命令解析
 *
 * 检查语法
 *
 * @param pCommand 命令
 * @return 0:正常  1:异常
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int	CheckSetCmd(char* in_pCommand)
{
	int iResult = -1;
	iResult = AnalyzeCmd(CMD_SET, in_pCommand);
	if (iResult == ERR_CMD_SUCCESS) {
		iResult = JudgeCmdReception(CMD_SET);
	}
	return iResult;
}

/**
 * @brief RQ命令解析
 *
 * 检查语法
 *
 * @param pCommand 命令
 * @return 0:正常  1:异常
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int	CheckRqCmd(char* in_pCommand)
{
	int iResult = -1;
	iResult = AnalyzeCmd(CMD_RQ, in_pCommand);
	if (iResult == ERR_CMD_SUCCESS) {
		iResult = JudgeCmdReception(CMD_RQ);
	}
	return iResult;
}

/**
 * @brief ERRRST命令解析
 *
 * 检查语法
 *
 * @param pCommand 命令
 * @return 0:正常  1:异常
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int	CheckErrrstCmd(char* in_pCommand)
{
	int iResult = -1;
	iResult = AnalyzeCmd(CMD_ERRRST, in_pCommand);
	if (iResult == ERR_CMD_SUCCESS) {
		iResult = JudgeCmdReception(CMD_ERRRST);
	}
	return iResult;
}

/**
 * @brief IPCHG命令解析
 *
 * 检查语法
 *
 * @param pCommand 命令
 * @return 0:正常  1:异常
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int	CheckIpChgCmd(char* in_pCommand)
{
	int iResult = -1;
	iResult = AnalyzeCmd(CMD_IPCHG, in_pCommand);
	if (iResult == ERR_CMD_SUCCESS) {
		iResult = JudgeCmdReception(CMD_IPCHG);
	}
	return iResult;
}

/**
 * @brief GETLOG命令解析
 *
 * 检查语法
 *
 * @param pCommand 命令
 * @return 0:正常  1:异常
 * @author wangwei
 * @date 2016/07/22 wangwei 初始版本
 */
int	CheckGetLogCmd(char* in_pCommand)
{
	int iResult = -1;
	iResult = AnalyzeCmd(CMD_GETLOG, in_pCommand);
	if (iResult == ERR_CMD_SUCCESS) {
		iResult = JudgeCmdReception(CMD_GETLOG);
	}
	return iResult;
}

/**
 * @brief UPGRADE命令解析
 *
 * 检查语法
 *
 * @param pCommand 命令
 * @return 0:正常  1:异常
 * @author wangwei
 * @date 2016/07/22 wangwei 初始版本
 */
int	CheckUpgradeCmd(char* in_pCommand)
{
	int iResult = -1;
	iResult = AnalyzeCmd(CMD_UPGRADE, in_pCommand);
	if (iResult == ERR_CMD_SUCCESS) {
		iResult = JudgeCmdReception(CMD_UPGRADE);
	}
	return iResult;
}

/**
 * @brief SEARCH命令解析
 *
 * 检查语法
 *
 * @param pCommand 命令
 * @return 0:正常  1:异常
 * @author wangwei
 * @date 2016/07/25 wangwei 初始版本
 */
int	CheckSearchCmd(char* in_pCommand)
{
	int iResult = -1;
	iResult = AnalyzeCmd(CMD_SEARCH, in_pCommand);
	if (iResult == ERR_CMD_SUCCESS) {
		iResult = JudgeCmdReception(CMD_SEARCH);
	}
	return iResult;
}

/**
 * @brief SETWIFI命令解析
 *
 * 检查语法
 *
 * @param pCommand 命令
 * @return 0:正常  1:异常
 * @author wangwei
 * @date 2016/07/27 wangwei 初始版本
 */
int	CheckSetWifiCmd(char* in_pCommand)
{
	int iResult = -1;
	iResult = AnalyzeCmd(CMD_SETWIFI, in_pCommand);
	if (iResult == ERR_CMD_SUCCESS) {
		iResult = JudgeCmdReception(CMD_SETWIFI);
	}
	return iResult;
}

/**
 * @brief OBTINFO命令解析
 *
 * 检查语法
 *
 * @param pCommand 命令
 * @return 0:正常  1:异常
 * @author wangwei
 * @date 2016/09/27 wangwei 初始版本
 */
int	CheckObtInfoCmd(char* in_pCommand)
{
	int iResult = -1;
	iResult = AnalyzeCmd(CMD_OBTINFO, in_pCommand);
	if (iResult == ERR_CMD_SUCCESS) {
		iResult = JudgeCmdReception(CMD_OBTINFO);
	}
	return iResult;
}

/**
 * @brief DEVPIC命令解析
 *
 * 检查语法
 *
 * @param pCommand 命令
 * @return 0:正常  1:异常
 * @author wangwei
 * @date 2016/10/19 wangwei 初始版本
 */
int	CheckDevPicCmd(char* in_pCommand)
{
	int iResult = -1;
	iResult = AnalyzeCmd(CMD_DEVPIC, in_pCommand);
	if (iResult == ERR_CMD_SUCCESS) {
		iResult = JudgeCmdReception(CMD_DEVPIC);
	}
	return iResult;
}

/**
 * @brief DEVMOVIE命令解析
 *
 * 检查语法
 *
 * @param pCommand 命令
 * @return 0:正常  1:异常
 * @author wangwei
 * @date 2016/10/19 wangwei 初始版本
 */
int	CheckDevMovieCmd(char* in_pCommand)
{
	int iResult = -1;
	iResult = AnalyzeCmd(CMD_DEVMOVIE, in_pCommand);
	if (iResult == ERR_CMD_SUCCESS) {
		iResult = JudgeCmdReception(CMD_DEVMOVIE);
	}
	return iResult;
}

/**
 * @brief PHOTOCATG命令解析
 *
 * 设置获得的舌苔图像种类
 *
 * @param pCommand 命令
 * @return 0:正常  1:异常
 * @author wangwei
 * @date 2017/07/21 wangwei 初始版本
 */
int CheckPhotoCatgCmd(char* in_pCommand)
{
	int iResult = -1;
	iResult = AnalyzeCmd(CMD_PHOTOCATG, in_pCommand);
	if (iResult == ERR_CMD_SUCCESS) {
		iResult = JudgeCmdReception(CMD_PHOTOCATG);
	}
	return iResult;
}

/**
 * @brief RPMSTART命令解析
 *
 * 重启Rpi Cam进程
 *
 * @param pCommand 命令
 * @return 0:正常  1:异常
 * @author wangwei
 * @date 2017/09/14 wangwei 初始版本
 */
int CheckRpmStartCmd(char* in_pCommand)
{
	int iResult = -1;
	iResult = AnalyzeCmd(CMD_RPMSTART, in_pCommand);
	if (iResult == ERR_CMD_SUCCESS) {
		iResult = JudgeCmdReception(CMD_RPMSTART);
	}
	return iResult;
}
