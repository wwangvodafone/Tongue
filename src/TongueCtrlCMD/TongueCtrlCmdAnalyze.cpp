/*****************************************************************************/
/**
* @file TongueCtrlCmdAnalyze.cpp
* @brief 舌苔控制命令解析
*
* 解析舌苔控制命令语法
*
* @author wangwei
* @date 2016/06/06 wangwei 初始版本
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/
#include "TongueCtrlCmdAnalyze.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../TongueCtrlErrProc.h"
#include "TongueCtrlCommand.h"
#include "TongueCtrlCommon.h"
#include "TongueCtrlParameter.h"
#include "TongueCtrl.h"

int AnalyzeShutDownCmd(COMMAND_BASE* in_pCmdBase);
int AnalyzeLightCmd(COMMAND_BASE* in_pCmdBase);
int AnalyzePictureCmd(COMMAND_BASE* in_pCmdBase);
int AnalyzeMovieCmd(COMMAND_BASE* in_pCmdBase);
int AnalyzeLoadCmd(COMMAND_BASE* in_pCmdBase);
int AnalyzeSaveCmd(COMMAND_BASE* in_pCmdBase);
int AnalyzeUploadCmd(COMMAND_BASE* in_pCmdBase);
int AnalyzeSyncCmd(COMMAND_BASE* in_pCmdBase);
int AnalyzeDeleteCmd(COMMAND_BASE* in_pCmdBase);
int AnalyzeSetCmd(COMMAND_BASE* in_pCmdBase);
int AnalyzeRqCmd(COMMAND_BASE* in_pCmdBase);
int AnalyzeResetCmd(COMMAND_BASE* in_pCmdBase);
int AnalyzeErrrstCmd(COMMAND_BASE* in_pCmdBase);
int AnalyzeIpChgCmd(COMMAND_BASE* in_pCmdBase);
int AnalyzeGetLogCmd(COMMAND_BASE* in_pCmdBase);
int AnalyzeUpgradeCmd(COMMAND_BASE* in_pCmdBase);
int AnalyzeSearchCmd(COMMAND_BASE* in_pCmdBase);
int AnalyzeSetWifiCmd(COMMAND_BASE* in_pCmdBase);
int AnalyzeObtInfoCmd(COMMAND_BASE* in_pCmdBase);
int AnalyzeDevPicCmd(COMMAND_BASE* in_pCmdBase);
int AnalyzeDevMovieCmd(COMMAND_BASE* in_pCmdBase);
int AnalyzePhotoCateCmd(COMMAND_BASE* in_pCmdBase);
int AnalyzeRebootCmd(COMMAND_BASE* in_pCmdBase);
int AnalyzeRpmStartCmd(COMMAND_BASE* in_pCmdBase);
//int AnalyzeInitMapsCmd(COMMAND_BASE* in_pCmdBase);

/**
 * @brief 命令的解析
 *
 * 命令解析
 *
 * @param COMMAND 命令
 * @param in_pParameter  参数1
 * @return 0:正常  0以外:异常
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int AnalyzeCmd(COMMAND_NUM COMMAND, char* in_pParameter)
{
	int iResult = -1;
	COMMAND_BASE		cmdBase;

	InitCmdBase(&cmdBase);
	SplitCmd(in_pParameter, &cmdBase);

	switch (COMMAND) {
	case CMD_SHUTDOWN:
	{
		iResult = AnalyzeShutDownCmd(&cmdBase);
		break;
	}
	case CMD_SET:
	{
		g_stCommand.unCommand.stCmdSet.bFloatValue = false;
		g_stCommand.unCommand.stCmdSet.bCharValue = false;
		iResult = AnalyzeSetCmd(&cmdBase);
		break;
	}
	case CMD_RQ:
	{
		iResult = AnalyzeRqCmd(&cmdBase);
		break;
	}
	case CMD_RESET:
	{
		iResult = AnalyzeResetCmd(&cmdBase);
		break;
	}
	case CMD_REBOOT:
	{
		iResult = AnalyzeRebootCmd(&cmdBase);
		break;
	}
	case CMD_ERRRST:
	{
		iResult = AnalyzeErrrstCmd(&cmdBase);
		break;
	}
	case CMD_LIGHT:
	{
		iResult = AnalyzeLightCmd(&cmdBase);
		break;
	}
	case CMD_PICTURE:
	{
		g_stCommand.unCommand.stCmdPicture.uiResolutionW = g_imageCtrlParam.iResolutionX;
		g_stCommand.unCommand.stCmdPicture.uiResolutionH = g_imageCtrlParam.iResolutionY;
		iResult = AnalyzePictureCmd(&cmdBase);
		break;
	}
	case CMD_MOVIE:
	{
		iResult = AnalyzeMovieCmd(&cmdBase);
		break;
	}
	case CMD_LOAD:
	{
		iResult = AnalyzeLoadCmd(&cmdBase);
		break;
	}
	case CMD_SAVE:
	{
		iResult = AnalyzeSaveCmd(&cmdBase);
		break;
	}
	case CMD_UPLOAD:
	{
		iResult = AnalyzeUploadCmd(&cmdBase);
		break;
	}
	case CMD_SYNC:
	{
		iResult = AnalyzeSyncCmd(&cmdBase);
		break;
	}
	case CMD_DELETE:
	{
		iResult = AnalyzeDeleteCmd(&cmdBase);
		break;
	}
	case CMD_IPCHG:
	{
		iResult = AnalyzeIpChgCmd(&cmdBase);
		break;
	}
	case CMD_GETLOG:
	{
		iResult = AnalyzeGetLogCmd(&cmdBase);
		break;
	}
	case CMD_UPGRADE:
	{
		iResult = AnalyzeUpgradeCmd(&cmdBase);
		break;
	}
	case CMD_SEARCH:
	{
		g_stCommand.unCommand.stCmdSearch.ulUTC = 0;
		iResult = AnalyzeSearchCmd(&cmdBase);
		break;
	}
	case CMD_SETWIFI:
	{
		iResult = AnalyzeSetWifiCmd(&cmdBase);
		break;
	}
	case CMD_OBTINFO:
	{
		iResult = AnalyzeObtInfoCmd(&cmdBase);
		break;
	}
	case CMD_DEVPIC:
	{
		iResult = AnalyzeDevPicCmd(&cmdBase);
		break;
	}
	case CMD_DEVMOVIE:
	{
		iResult = AnalyzeDevMovieCmd(&cmdBase);
		break;
	}
	case CMD_PHOTOCATG:
	{
		iResult = AnalyzePhotoCateCmd(&cmdBase);
		break;
	}
	case CMD_RPMSTART:
	{
		iResult = AnalyzeRpmStartCmd(&cmdBase);
		break;
	}
	default:
		break;
	}

	return iResult;
}

/**
 * @brief SHUTDOWN的解析
 *
 * 解析SHUTDOWN
 *
 * @param in_pCmdBase 参数0
 * @return 0:正常  0以外:异常
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int AnalyzeShutDownCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;

	if (in_pCmdBase->param[0][0] == '\0') {
		iResult = ERR_CMD_SUCCESS;
	}

	return iResult;
}

/**
 * @brief RESET的解析
 *
 * 解析RESET
 *
 * @param in_pCmdBase 参数0
 * @return 0:正常  0以外:异常
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int AnalyzeResetCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;
	int i = 0;

	g_stCommand.unCommand.stCmdReset.uiNum = 0;
	while (in_pCmdBase->param[i][0] != '\0') {
		if (IsDigital(in_pCmdBase->param[i])) {
			int iResetType = atoi(in_pCmdBase->param[i]);
			if ((iResetType >= DEV_RESET_ALL)
				&& (iResetType <= DEV_RESET_DEV)) {
				g_stCommand.unCommand.stCmdReset.uiCategory[i] = iResetType;
				iResult = ERR_CMD_SUCCESS;
			}
			else {
				iResult = ERR_CMD_PARSE_ERROR;
				break;
			}
		}
		else {
			iResult = ERR_CMD_PARSE_ERROR;
			break;
		}
		if (g_stCommand.unCommand.stCmdReset.uiNum++ >= DEV_RESET_NUM) {
			iResult = ERR_CMD_PARSE_ERROR;
			break;
		}

		i++;
	}

	return iResult;
}


/**
 * @brief REBOOT的解析
 *
 * 解析RESET
 *
 * @param in_pCmdBase 参数0
 * @return 0:正常  0以外:异常
 * @author wangwei
 * @date 2016/12/27 wangwei 初始版本
 */
int AnalyzeRebootCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;

	if (in_pCmdBase->param[0][0] == '\0') {
		iResult = ERR_CMD_SUCCESS;
	}

	return iResult;
}
/**
 * @brief LIGHT的解析
 *
 * 解析LIGHT
 *
 * @param in_pCmdBase 参数0
 * @return 0:正常  0以外:异常
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */

int AnalyzeLightCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;
	if (IsDigital(in_pCmdBase->param[0])) { /*Photo/Video*/
		int iLedType = atoi(in_pCmdBase->param[0]);
		if ((iLedType >= IMAGE_LIGHT)
			&& (iLedType <= ILLUME_LIGHT)) {
			g_stCommand.unCommand.stCmdLight.uiCategory = iLedType;
			int iLedOn = atoi(in_pCmdBase->param[1]);
			if (iLedType == ILLUME_LIGHT) {
				g_stCommand.unCommand.stCmdLight.uiBrightness = iLedOn;	// 亮度
				iResult = ERR_CMD_SUCCESS;
			}
			else {
				if ((iLedOn == ENABLE_OFF)
					|| (iLedOn == ENABLE_ON)) {
					g_stCommand.unCommand.stCmdLight.uiEnable = iLedOn; // 亮灭
					iResult = ERR_CMD_SUCCESS;
				}
			}
		}
	}
	return iResult;
}

/**
 * @brief PICTURE的解析
 *
 * 解析PICTURE
 *
 * @param in_pCmdBase 参数0
 * @return 0:正常  0以外:异常
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int AnalyzePictureCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;

	if (in_pCmdBase->param[0][0] != '\0') {
		strncpy(g_stCommand.unCommand.stCmdPicture.aNameAddr, in_pCmdBase->param[0], NAME_LENGTH);
		if (IsDigital(in_pCmdBase->param[1])) {
			int iValue = atoi(in_pCmdBase->param[1]);
			g_stCommand.unCommand.stCmdPicture.uiAction = iValue;
			if ((iValue >= PHOTO_TAKE) && (iValue <= PHOTO_ABANDON)) {
				if (in_pCmdBase->param[2][0] == '\0') {
					iResult = ERR_CMD_SUCCESS;
				}
			}
		}
	}


	return iResult;
}

/**
 * @brief MOVIE的解析
 *
 * 解析MOVIE
 *
 * @param in_pCmdBase 参数0
 * @return 0:正常  0以外:异常
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int AnalyzeMovieCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;
	if (in_pCmdBase->param[0][0] != '\0') {
		strncpy(g_stCommand.unCommand.stCmdMovie.aNameAddr, in_pCmdBase->param[0], NAME_LENGTH);
		if (IsDigital(in_pCmdBase->param[1])) {
			int iVideoStat = atoi(in_pCmdBase->param[1]);
			if ((iVideoStat >= VIDEO_START)
				&& (iVideoStat <= VIDEO_TIMEOUT)) {
				g_stCommand.unCommand.stCmdMovie.uiAction = iVideoStat;
				if (in_pCmdBase->param[2][0] == '\0') {
					iResult = ERR_CMD_SUCCESS;
				}
			}
		}
	}

	return iResult;
}

/**
 * @brief LOAD的解析
 *
 * 解析LOAD
 *
 * @param in_pCmdBase 参数0
 * @return 0:正常  0以外:异常
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int AnalyzeLoadCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;
	if (IsDigital(in_pCmdBase->param[0])) { /*悢帤偺応崌丄惓偟偄*/
		int iDataType = atoi(in_pCmdBase->param[0]);
		if ((iDataType >= MIN_DATA_TYPE)
			&& (iDataType <= MAX_DATA_TYPE)) {
			g_stCommand.unCommand.stCmdLoad.uiDataType = iDataType;
			strncpy(g_stCommand.unCommand.stCmdLoad.fileName, in_pCmdBase->param[1], FILE_MAX_LENGTH);
			if (IsDigital(in_pCmdBase->param[2])) {
				int iWorkspaceId = atoi(in_pCmdBase->param[2]);
				if ((iWorkspaceId >= MIN_WORKSPACE_ID) && (iWorkspaceId <= MAX_WORKSPACE_ID)) {
						g_stCommand.unCommand.stCmdLoad.uiWorkSpaceId = iWorkspaceId;
						iResult = ERR_CMD_SUCCESS;
				}
			}
		}
		if (iResult == ERR_CMD_SUCCESS) {
			if (in_pCmdBase->param[3][0] != '\0') {
				iResult = ERR_CMD_PARSE_ERROR;
			}
		}

	}

	return iResult;
}

/**
 * @brief SAVE的解析
 *
 * LOAD解析
 *
 * @param in_pCmdBase 参数0
 * @return 0:正常  0以外:异常
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int AnalyzeSaveCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;
	if (IsDigital(in_pCmdBase->param[0])) { /*悢帤偺応崌丄惓偟偄*/
		int iDataType = atoi(in_pCmdBase->param[0]);
		if ((iDataType >= MIN_DATA_TYPE)
			&& (iDataType <= MAX_DATA_TYPE)) {
			g_stCommand.unCommand.stCmdSave.uiDataType = iDataType;
			if (IsDigital(in_pCmdBase->param[1])) {
				int iWorkspaceId = atoi(in_pCmdBase->param[1]);
				if ((iWorkspaceId >= MIN_WORKSPACE_ID) && (iWorkspaceId <= MAX_WORKSPACE_ID)) {
						g_stCommand.unCommand.stCmdSave.uiWorkSpaceId = iWorkspaceId;
						iResult = ERR_CMD_SUCCESS;
				}
			}
			strncpy(g_stCommand.unCommand.stCmdSave.fileName, in_pCmdBase->param[2], FILE_MAX_LENGTH);
		}
		if (iResult == ERR_CMD_SUCCESS) {
			if (in_pCmdBase->param[3][0] != '\0') {
				iResult = ERR_CMD_PARSE_ERROR;
			}
		}
	}

	return iResult;
}

/**
 * @brief UPLOAD的解析
 *
 * 解析UPLOAD
 *
 * @param in_pCmdBase 参数0
 * @return 0:正常  0以外:异常
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int AnalyzeUploadCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;

	if (in_pCmdBase->param[0][0] != '\0') {
		strncpy(g_stCommand.unCommand.stCmdUpload.aNameAddr, in_pCmdBase->param[0], NAME_LENGTH);
		if (IsDigital(in_pCmdBase->param[1])) { /*是不是数字*/
			int iImgType = atoi(in_pCmdBase->param[1]);	// 上传的是图像还是视频？
			if ((iImgType >= IMAGE) && (iImgType <= VIDEO)) {
				g_stCommand.unCommand.stCmdUpload.uiCategory = iImgType;
				if (IsDigital(in_pCmdBase->param[2])) { /*是不是数字*/
					int iNum = atoi(in_pCmdBase->param[2]); // 上传的文件数量
					g_stCommand.unCommand.stCmdUpload.uiFileNum = iNum;
					if (in_pCmdBase->param[3][0] == '\0') {
						iResult = ERR_CMD_SUCCESS;
					}
				}
			}
		}
	}

	return iResult;
}

/**
 * @brief SYNC的解析
 *
 * 解析SYNC
 *
 * @param in_pCmdBase 参数0
 * @return 0:正常  0以外:异常
 * @author wangwei
 * @date 2016/06/07 wangwei 初始版本
 */
int AnalyzeSyncCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;

	if (in_pCmdBase->param[0][0] != '\0') {
		strncpy(g_stCommand.unCommand.stCmdSync.aNameAddr, in_pCmdBase->param[0], NAME_LENGTH);
		if (IsDigital(in_pCmdBase->param[1])) { /*是不是数字*/
			int iImgType = atoi(in_pCmdBase->param[1]);	// 同步的是图像还是视频？
			if ((iImgType >= IMAGE) &&(iImgType <= VIDEO)) {
				g_stCommand.unCommand.stCmdSync.uiCategory = iImgType;
				if (in_pCmdBase->param[2][0] == '\0') {
					iResult = ERR_CMD_SUCCESS;
				}
			}
		}
	}


	return iResult;
}


/**
 * @brief DELETE的解析
 *
 * 解析DELETE
 *
 * @param in_pCmdBase 参数0
 * @return 0:正常  0以外:异常
 * @author wangwei
 * @date 2016/06/07 wangwei 初始版本
 */
int AnalyzeDeleteCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;
	int iIndex = 0;

	if (in_pCmdBase->param[0][0] != '\0') {
		strncpy(g_stCommand.unCommand.stCmdDelete.aNameAddr, in_pCmdBase->param[0], NAME_LENGTH);
		if (IsDigital(in_pCmdBase->param[1])) { /*是不是数字*/
			int iCategory = atoi(in_pCmdBase->param[1]);	// 图像或视频
			if ((iCategory == IMAGE) || (iCategory == VIDEO)) {
				g_stCommand.unCommand.stCmdDelete.uiCategory = iCategory;
				if (IsDigital(in_pCmdBase->param[2])) { /*是不是数字*/
					int iMount = atoi(in_pCmdBase->param[2]);	// 数量
					g_stCommand.unCommand.stCmdDelete.uiMount = iMount;
					if (iMount == DELETE_ALL) {	// 全删除
						if (in_pCmdBase->param[3][0] == '\0') {
							iResult = ERR_CMD_SUCCESS;
						}
					}
					else if (iMount <= MAX_FILENUM) {	// 删除指定文件
						iIndex = 3;
						do {
							strncpy(g_stCommand.unCommand.stCmdDelete.aDeleteFile[iIndex-3],
									in_pCmdBase->param[iIndex], FILENAME_LENGTH);
						} while (in_pCmdBase->param[iIndex++][0] != '\0');
						if (in_pCmdBase->param[iIndex][0] == '\0') {
							iResult = ERR_CMD_SUCCESS;
						}
					}
				}
			}
		}
	}

	return iResult;
}

/**
 * @brief SET的解析
 *
 * 解析SET
 *
 * @param in_pCmdBase 参数0
 * @return 0:正常  0以外:异常
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int AnalyzeSetCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;

	if (IsDigital(in_pCmdBase->param[0])) { /*悢帤偺応崌丄惓偟偄*/
		int iItem = atoi(in_pCmdBase->param[0]);
		g_stCommand.unCommand.stCmdSet.uiItem = iItem;
		if (iItem > 0 && iItem < SYS_PARAM_NUM) {
			if (IsDigital(in_pCmdBase->param[1])) { /*悢帤偺応崌丄惓偟偄*/
				g_stCommand.unCommand.stCmdSet.value.iValue = atoi(in_pCmdBase->param[1]);
				iResult = ERR_CMD_SUCCESS;
			}
			else if (IsFloatDigital(in_pCmdBase->param[1])) {
				g_stCommand.unCommand.stCmdSet.value.fValue = atof(in_pCmdBase->param[1]);
				g_stCommand.unCommand.stCmdSet.bFloatValue = true;
				iResult = ERR_CMD_SUCCESS;
			}
			else {
				int i = IpToInt(in_pCmdBase->param[1]);
				if (i == -1) {
					iResult = ERR_CMD_PARSE_ERROR;
				}
				else {
					g_stCommand.unCommand.stCmdSet.value.iValue = i;
					g_stCommand.unCommand.stCmdSet.bCharValue = true;
					iResult = ERR_CMD_SUCCESS;
				}
			}
			if (iResult == ERR_CMD_SUCCESS) {
				if (in_pCmdBase->param[2][0] != '\0') {
					iResult = ERR_CMD_PARSE_ERROR;
				}
			}
		}
	}

	return iResult;
}

/**
 * @brief RQ的解析
 *
 * 解析RQ
 *
 * @param in_pCmdBase 参数0
 * @return 0:正常  0以外:异常
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int AnalyzeRqCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;

	if (IsDigital(in_pCmdBase->param[0])) { /*如果是数字，则正确*/
		int iItem = atoi(in_pCmdBase->param[0]);
		if (iItem > 0 && iItem < SYS_PARAM_NUM) {
			g_stCommand.unCommand.stCmdRq.uiItem = iItem;
			if (in_pCmdBase->param[1][0] == '\0') {
				iResult = ERR_CMD_SUCCESS;
			}
		}
	}

	return iResult;
}

/**
 * @brief ERRRST的解析
 *
 * 解析ERRRST
 *
 * @param in_pCmdBase 参数0
 * @return 0:正常  0以外:异常
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int AnalyzeErrrstCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;

	if (in_pCmdBase->param[0][0] == '\0') {
		iResult = ERR_CMD_SUCCESS;
	}

	return iResult;
}


/**
 * @brief IPCHG的解析
 *
 * 解析PCHG
 *
 * @param in_pCmdBase 参数0
 * @return 0:正常  0以外:异常
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int AnalyzeIpChgCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;

	int i = IpToInt(in_pCmdBase->param[0]);
	if (i != -1) {
		bool b = IsNetMask(in_pCmdBase->param[1]);
		if (b == true) {
			if (in_pCmdBase->param[2][0] == '\0') {
				strncpy(g_stCommand.unCommand.stCmdIpChg.ipaddr, in_pCmdBase->param[0], IP_LENGTH);
				strncpy(g_stCommand.unCommand.stCmdIpChg.netmask, in_pCmdBase->param[1], IP_LENGTH);
				iResult = ERR_CMD_SUCCESS;
			}
		}
	}

	return iResult;
}

/**
 * @brief GETLOG的解析
 *
 * 解析GETLOG
 *
 * @param in_pCmdBase 参数0
 * @return 0:正常  0以外:异常
 * @author wangwei
 * @date 2016/07/22 wangwei 初始版本
 */
int AnalyzeGetLogCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;

	if (in_pCmdBase->param[0][0] == '\0') {
		iResult = ERR_CMD_SUCCESS;
	}

	return iResult;
}

/**
 * @brief UPGRADE的解析
 *
 * 解析GETLOG
 *
 * @param in_pCmdBase 参数0
 * @return 0:正常  0以外:异常
 * @author wangwei
 * @date 2016/07/22 wangwei 初始版本
 */
int AnalyzeUpgradeCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;

	if (in_pCmdBase->param[0][0] == '\0') {
		iResult = ERR_CMD_SUCCESS;
	}

	return iResult;
}


/**
 * @brief SEARCH的解析
 *
 * 解析GETLOG
 *
 * @param in_pCmdBase 参数0
 * @return 0:正常  0以外:异常
 * @author wangwei
 * @date 2016/07/25 wangwei 初始版本
 */
int AnalyzeSearchCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;

	if (in_pCmdBase->param[0][0] == '\0') {
		iResult = ERR_CMD_SUCCESS;
	}
	else if (IsDigital(in_pCmdBase->param[0])) { /*如果是数字，则正确*/
		long iUTC = atol(in_pCmdBase->param[0]);
		g_stCommand.unCommand.stCmdSearch.ulUTC = iUTC;
		iResult = ERR_CMD_SUCCESS;
	}

	return iResult;
}


/**
 * @brief SETWIFI的解析
 *
 * 解析SETWIFI
 *
 * @param in_pCmdBase 参数0
 * @return 0:正常  0以外:异常
 * @author wangwei
 * @date 2016/07/27 wangwei 初始版本
 */
int AnalyzeSetWifiCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;

	if (IsDigital(in_pCmdBase->param[0])) { /*如果是数字，则正确*/
		int iCategory = atoi(in_pCmdBase->param[0]);
		g_stCommand.unCommand.stCmdSetWifi.category = iCategory;
		if (iCategory == WIFI_HOTSPOT) {
			if (in_pCmdBase->param[1][0] == '\0') {
					iResult = ERR_CMD_SUCCESS;
				}
		}
		else if (iCategory == WIFI_STATION) {
			if (in_pCmdBase->param[1][0] != '\0') {
				strncpy(g_stCommand.unCommand.stCmdSetWifi.ssid, in_pCmdBase->param[1], SSID_LENGTH);
				if (in_pCmdBase->param[2][0] != '\0') {
						strncpy(g_stCommand.unCommand.stCmdSetWifi.pass, in_pCmdBase->param[2], PASS_LENGTH);
				}
				if (in_pCmdBase->param[3][0] == '\0') {
					iResult = ERR_CMD_SUCCESS;
				}
			}
		}
		else if (iCategory == WIFI_ASK) {
			if (in_pCmdBase->param[1][0] == '\0') {
					iResult = ERR_CMD_SUCCESS;
				}
		}
	}


	return iResult;
}

/**
 * @brief OBTINFO的解析
 *
 * 解析OBTINFO
 *
 * @param in_pCmdBase 参数0
 * @return 0:正常  0以外:异常
 * @author wangwei
 * @date 2016/09/23 wangwei 初始版本
 */
int AnalyzeObtInfoCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;

	if (IsDigital(in_pCmdBase->param[0])) { /*如果是数字，则正确*/
		int iInfo = atoi(in_pCmdBase->param[0]);
		g_stCommand.unCommand.stCmdObtInfo.uiInfo = iInfo;
		if (in_pCmdBase->param[1][0] == '\0') {
			iResult = ERR_CMD_SUCCESS;
		}
	}

	return iResult;
}

/**
 * @brief DEVPIC的解析
 *
 * 解析DEVPIC
 *
 * @param in_pCmdBase 参数0
 * @return 0:正常  0以外:异常
 * @author wangwei
 * @date 2016/10/19 wangwei 初始版本
 */
int AnalyzeDevPicCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;

	if (in_pCmdBase->param[0][0] != '\0') {
		strncpy(g_stCommand.unCommand.stCmdDevPic.mac, in_pCmdBase->param[0], NAME_LENGTH);
		if (IsDigital(in_pCmdBase->param[1])) {
			int iValue = atoi(in_pCmdBase->param[1]);
			g_stCommand.unCommand.stCmdDevPic.uiAction = iValue;
			if (in_pCmdBase->param[2][0] == '\0') {
				if (iValue == 0) {	// 拍照开始
					iResult = ERR_CMD_SUCCESS;
				}
			}
			else {
				if (iValue == 1) {	// 拍照结束
					strncpy(g_stCommand.unCommand.stCmdDevPic.sResultLink, in_pCmdBase->param[2], RESPONSE_LENGTH);
					if (in_pCmdBase->param[3][0] == '\0') {
						iResult = ERR_CMD_SUCCESS;
					}
				}
			}

		}
	}

	return iResult;
}


/**
 * @brief DEVMOVIE的解析
 *
 * 解析DEVMOVIE
 *
 * @param in_pCmdBase 参数0
 * @return 0:正常  0以外:异常
 * @author wangwei
 * @date 2016/10/19 wangwei 初始版本
 */
int AnalyzeDevMovieCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;

	if (in_pCmdBase->param[0][0] != '\0') {
		strncpy(g_stCommand.unCommand.stCmdDevMovie.mac, in_pCmdBase->param[0], NAME_LENGTH);
		if (IsDigital(in_pCmdBase->param[1])) {
			int iValue = atoi(in_pCmdBase->param[1]);
			g_stCommand.unCommand.stCmdDevMovie.uiAction = iValue;
			if (in_pCmdBase->param[2][0] == '\0') {
				if ((iValue == 0) && (iValue == 2)) {	// 摄像开始   摄像超时
					iResult = ERR_CMD_SUCCESS;
				}
			}
			else {
				if (iValue == 1) {	// 摄像结束
					strncpy(g_stCommand.unCommand.stCmdDevMovie.sResultLink, in_pCmdBase->param[2], RESPONSE_LENGTH);
					if (in_pCmdBase->param[3][0] == '\0') {
						iResult = ERR_CMD_SUCCESS;
					}
				}
			}

		}
	}

	return iResult;
}


/**
 * @brief PHOTOCATE的解析
 *
 * 解析PHOTOCATE
 *
 * @param in_pCmdBase 参数0
 * @return 0:正常  0以外:异常
 * @author wangwei
 * @date 2017/07/21 wangwei 初始版本
 */
int AnalyzePhotoCateCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;
	printf("%s\n", in_pCmdBase->param[0]);
	if (IsDigital(in_pCmdBase->param[0])) { /*如果是数字，则正确*/
		int iCategory = atoi(in_pCmdBase->param[0]);
		printf("category=%d\n", iCategory);
		if ((iCategory >= ORIGIN) && (iCategory <= ORIGIN_TONGUE_TONGUE_LIP)) {
			g_stCommand.unCommand.stCmdPhotoCate.uiCategory = iCategory;
			if (in_pCmdBase->param[1][0] == '\0') {
				iResult = ERR_CMD_SUCCESS;
			}
		}
	}

	return iResult;
}


/**
 * @brief RPMSTART的解析
 *
 * 解析RPMSTART
 *
 * @param in_pCmdBase 参数0
 * @return 0:正常  0以外:异常
 * @author wangwei
 * @date 2017/09/14 wangwei 初始版本
 */
int AnalyzeRpmStartCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;

	if (in_pCmdBase->param[0][0] == '\0') {
		iResult = ERR_CMD_SUCCESS;
	}

	return iResult;
}

/**
 * @brief 命令的接受处理条件判断
 *
 * 命令的接受处理条件判断
 *
 * @param COMMAND 参数0
 * @return 0:正常  0以外:异常
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int JudgeCmdReception(COMMAND_NUM COMMAND)
{
	int iResult = ERR_RECEPTION_COND;

	switch (COMMAND) {
		case CMD_SHUTDOWN:
		case CMD_RESET:
		case CMD_REBOOT:
		case CMD_ERRRST:
		case CMD_DEVPIC:
		case CMD_DEVMOVIE:
		case CMD_SETWIFI:
		case CMD_RPMSTART:
			iResult = ERR_CMD_SUCCESS;
			break;
		case CMD_LIGHT:
		case CMD_LOAD:
		case CMD_SAVE:
		case CMD_UPLOAD:
		case CMD_SYNC:
		case CMD_DELETE:
		case CMD_SET:
		case CMD_RQ:
		case CMD_IPCHG:
		case CMD_GETLOG:
		case CMD_UPGRADE:
		case CMD_SEARCH:
		case CMD_OBTINFO:
		case CMD_PHOTOCATG:
			if ((g_stReceptionCond.uiCurDevStatus == DUMMY_STATUS) && (g_stReceptionCond.uiCurSysCmdStatus == SYSTEM_READY)) {
				iResult = ERR_CMD_SUCCESS;
			}
			break;
		case CMD_PICTURE:
			if ((g_stReceptionCond.uiCurDevStatus == DEVICE_EXEC) && (g_stReceptionCond.uiCurSysCmdStatus == DEVICE_EXEC)) {
				iResult = ERR_CMD_SUCCESS;
			}
			else if ((g_stReceptionCond.uiCurDevStatus == DUMMY_STATUS) && (g_stReceptionCond.uiCurSysCmdStatus == SYSTEM_READY)) {
				iResult = ERR_CMD_SUCCESS;
			}
			break;
		case CMD_MOVIE:
			if ((g_stReceptionCond.uiCurDevStatus == DEVICE_EXEC) && (g_stReceptionCond.uiCurSysCmdStatus == DEVICE_EXEC)) {
				iResult = ERR_CMD_SUCCESS;
			}
			else if (g_stReceptionCond.uiCurDevStatus == DUMMY_STATUS) {
				iResult = ERR_CMD_SUCCESS;
			}
			break;
		default:
			break;
	}

	return iResult;
}
