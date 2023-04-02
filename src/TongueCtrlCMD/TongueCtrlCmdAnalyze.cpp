/*****************************************************************************/
/**
* @file TongueCtrlCmdAnalyze.cpp
* @brief ��̦�����������
*
* ������̦���������﷨
*
* @author wangwei
* @date 2016/06/06 wangwei ��ʼ�汾
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
 * @brief ����Ľ���
 *
 * �������
 *
 * @param COMMAND ����
 * @param in_pParameter  ����1
 * @return 0:����  0����:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
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
 * @brief SHUTDOWN�Ľ���
 *
 * ����SHUTDOWN
 *
 * @param in_pCmdBase ����0
 * @return 0:����  0����:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
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
 * @brief RESET�Ľ���
 *
 * ����RESET
 *
 * @param in_pCmdBase ����0
 * @return 0:����  0����:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
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
 * @brief REBOOT�Ľ���
 *
 * ����RESET
 *
 * @param in_pCmdBase ����0
 * @return 0:����  0����:�쳣
 * @author wangwei
 * @date 2016/12/27 wangwei ��ʼ�汾
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
 * @brief LIGHT�Ľ���
 *
 * ����LIGHT
 *
 * @param in_pCmdBase ����0
 * @return 0:����  0����:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
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
				g_stCommand.unCommand.stCmdLight.uiBrightness = iLedOn;	// ����
				iResult = ERR_CMD_SUCCESS;
			}
			else {
				if ((iLedOn == ENABLE_OFF)
					|| (iLedOn == ENABLE_ON)) {
					g_stCommand.unCommand.stCmdLight.uiEnable = iLedOn; // ����
					iResult = ERR_CMD_SUCCESS;
				}
			}
		}
	}
	return iResult;
}

/**
 * @brief PICTURE�Ľ���
 *
 * ����PICTURE
 *
 * @param in_pCmdBase ����0
 * @return 0:����  0����:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
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
 * @brief MOVIE�Ľ���
 *
 * ����MOVIE
 *
 * @param in_pCmdBase ����0
 * @return 0:����  0����:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
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
 * @brief LOAD�Ľ���
 *
 * ����LOAD
 *
 * @param in_pCmdBase ����0
 * @return 0:����  0����:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
int AnalyzeLoadCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;
	if (IsDigital(in_pCmdBase->param[0])) { /*�����̏ꍇ�A������*/
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
 * @brief SAVE�Ľ���
 *
 * LOAD����
 *
 * @param in_pCmdBase ����0
 * @return 0:����  0����:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
int AnalyzeSaveCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;
	if (IsDigital(in_pCmdBase->param[0])) { /*�����̏ꍇ�A������*/
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
 * @brief UPLOAD�Ľ���
 *
 * ����UPLOAD
 *
 * @param in_pCmdBase ����0
 * @return 0:����  0����:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
int AnalyzeUploadCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;

	if (in_pCmdBase->param[0][0] != '\0') {
		strncpy(g_stCommand.unCommand.stCmdUpload.aNameAddr, in_pCmdBase->param[0], NAME_LENGTH);
		if (IsDigital(in_pCmdBase->param[1])) { /*�ǲ�������*/
			int iImgType = atoi(in_pCmdBase->param[1]);	// �ϴ�����ͼ������Ƶ��
			if ((iImgType >= IMAGE) && (iImgType <= VIDEO)) {
				g_stCommand.unCommand.stCmdUpload.uiCategory = iImgType;
				if (IsDigital(in_pCmdBase->param[2])) { /*�ǲ�������*/
					int iNum = atoi(in_pCmdBase->param[2]); // �ϴ����ļ�����
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
 * @brief SYNC�Ľ���
 *
 * ����SYNC
 *
 * @param in_pCmdBase ����0
 * @return 0:����  0����:�쳣
 * @author wangwei
 * @date 2016/06/07 wangwei ��ʼ�汾
 */
int AnalyzeSyncCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;

	if (in_pCmdBase->param[0][0] != '\0') {
		strncpy(g_stCommand.unCommand.stCmdSync.aNameAddr, in_pCmdBase->param[0], NAME_LENGTH);
		if (IsDigital(in_pCmdBase->param[1])) { /*�ǲ�������*/
			int iImgType = atoi(in_pCmdBase->param[1]);	// ͬ������ͼ������Ƶ��
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
 * @brief DELETE�Ľ���
 *
 * ����DELETE
 *
 * @param in_pCmdBase ����0
 * @return 0:����  0����:�쳣
 * @author wangwei
 * @date 2016/06/07 wangwei ��ʼ�汾
 */
int AnalyzeDeleteCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;
	int iIndex = 0;

	if (in_pCmdBase->param[0][0] != '\0') {
		strncpy(g_stCommand.unCommand.stCmdDelete.aNameAddr, in_pCmdBase->param[0], NAME_LENGTH);
		if (IsDigital(in_pCmdBase->param[1])) { /*�ǲ�������*/
			int iCategory = atoi(in_pCmdBase->param[1]);	// ͼ�����Ƶ
			if ((iCategory == IMAGE) || (iCategory == VIDEO)) {
				g_stCommand.unCommand.stCmdDelete.uiCategory = iCategory;
				if (IsDigital(in_pCmdBase->param[2])) { /*�ǲ�������*/
					int iMount = atoi(in_pCmdBase->param[2]);	// ����
					g_stCommand.unCommand.stCmdDelete.uiMount = iMount;
					if (iMount == DELETE_ALL) {	// ȫɾ��
						if (in_pCmdBase->param[3][0] == '\0') {
							iResult = ERR_CMD_SUCCESS;
						}
					}
					else if (iMount <= MAX_FILENUM) {	// ɾ��ָ���ļ�
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
 * @brief SET�Ľ���
 *
 * ����SET
 *
 * @param in_pCmdBase ����0
 * @return 0:����  0����:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
int AnalyzeSetCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;

	if (IsDigital(in_pCmdBase->param[0])) { /*�����̏ꍇ�A������*/
		int iItem = atoi(in_pCmdBase->param[0]);
		g_stCommand.unCommand.stCmdSet.uiItem = iItem;
		if (iItem > 0 && iItem < SYS_PARAM_NUM) {
			if (IsDigital(in_pCmdBase->param[1])) { /*�����̏ꍇ�A������*/
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
 * @brief RQ�Ľ���
 *
 * ����RQ
 *
 * @param in_pCmdBase ����0
 * @return 0:����  0����:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
int AnalyzeRqCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;

	if (IsDigital(in_pCmdBase->param[0])) { /*��������֣�����ȷ*/
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
 * @brief ERRRST�Ľ���
 *
 * ����ERRRST
 *
 * @param in_pCmdBase ����0
 * @return 0:����  0����:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
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
 * @brief IPCHG�Ľ���
 *
 * ����PCHG
 *
 * @param in_pCmdBase ����0
 * @return 0:����  0����:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
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
 * @brief GETLOG�Ľ���
 *
 * ����GETLOG
 *
 * @param in_pCmdBase ����0
 * @return 0:����  0����:�쳣
 * @author wangwei
 * @date 2016/07/22 wangwei ��ʼ�汾
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
 * @brief UPGRADE�Ľ���
 *
 * ����GETLOG
 *
 * @param in_pCmdBase ����0
 * @return 0:����  0����:�쳣
 * @author wangwei
 * @date 2016/07/22 wangwei ��ʼ�汾
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
 * @brief SEARCH�Ľ���
 *
 * ����GETLOG
 *
 * @param in_pCmdBase ����0
 * @return 0:����  0����:�쳣
 * @author wangwei
 * @date 2016/07/25 wangwei ��ʼ�汾
 */
int AnalyzeSearchCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;

	if (in_pCmdBase->param[0][0] == '\0') {
		iResult = ERR_CMD_SUCCESS;
	}
	else if (IsDigital(in_pCmdBase->param[0])) { /*��������֣�����ȷ*/
		long iUTC = atol(in_pCmdBase->param[0]);
		g_stCommand.unCommand.stCmdSearch.ulUTC = iUTC;
		iResult = ERR_CMD_SUCCESS;
	}

	return iResult;
}


/**
 * @brief SETWIFI�Ľ���
 *
 * ����SETWIFI
 *
 * @param in_pCmdBase ����0
 * @return 0:����  0����:�쳣
 * @author wangwei
 * @date 2016/07/27 wangwei ��ʼ�汾
 */
int AnalyzeSetWifiCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;

	if (IsDigital(in_pCmdBase->param[0])) { /*��������֣�����ȷ*/
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
 * @brief OBTINFO�Ľ���
 *
 * ����OBTINFO
 *
 * @param in_pCmdBase ����0
 * @return 0:����  0����:�쳣
 * @author wangwei
 * @date 2016/09/23 wangwei ��ʼ�汾
 */
int AnalyzeObtInfoCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;

	if (IsDigital(in_pCmdBase->param[0])) { /*��������֣�����ȷ*/
		int iInfo = atoi(in_pCmdBase->param[0]);
		g_stCommand.unCommand.stCmdObtInfo.uiInfo = iInfo;
		if (in_pCmdBase->param[1][0] == '\0') {
			iResult = ERR_CMD_SUCCESS;
		}
	}

	return iResult;
}

/**
 * @brief DEVPIC�Ľ���
 *
 * ����DEVPIC
 *
 * @param in_pCmdBase ����0
 * @return 0:����  0����:�쳣
 * @author wangwei
 * @date 2016/10/19 wangwei ��ʼ�汾
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
				if (iValue == 0) {	// ���տ�ʼ
					iResult = ERR_CMD_SUCCESS;
				}
			}
			else {
				if (iValue == 1) {	// ���ս���
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
 * @brief DEVMOVIE�Ľ���
 *
 * ����DEVMOVIE
 *
 * @param in_pCmdBase ����0
 * @return 0:����  0����:�쳣
 * @author wangwei
 * @date 2016/10/19 wangwei ��ʼ�汾
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
				if ((iValue == 0) && (iValue == 2)) {	// ����ʼ   ����ʱ
					iResult = ERR_CMD_SUCCESS;
				}
			}
			else {
				if (iValue == 1) {	// �������
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
 * @brief PHOTOCATE�Ľ���
 *
 * ����PHOTOCATE
 *
 * @param in_pCmdBase ����0
 * @return 0:����  0����:�쳣
 * @author wangwei
 * @date 2017/07/21 wangwei ��ʼ�汾
 */
int AnalyzePhotoCateCmd(COMMAND_BASE* in_pCmdBase)
{
	int iResult = ERR_CMD_PARSE_ERROR;
	printf("%s\n", in_pCmdBase->param[0]);
	if (IsDigital(in_pCmdBase->param[0])) { /*��������֣�����ȷ*/
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
 * @brief RPMSTART�Ľ���
 *
 * ����RPMSTART
 *
 * @param in_pCmdBase ����0
 * @return 0:����  0����:�쳣
 * @author wangwei
 * @date 2017/09/14 wangwei ��ʼ�汾
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
 * @brief ����Ľ��ܴ��������ж�
 *
 * ����Ľ��ܴ��������ж�
 *
 * @param COMMAND ����0
 * @return 0:����  0����:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
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
