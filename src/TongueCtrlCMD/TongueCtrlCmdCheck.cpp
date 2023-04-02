/*****************************************************************************/
/**
* @file TongueCtrlCmdCheck.cpp
* @brief ��̦���������﷨���
*
* �����̦���������﷨
*
* @author wangwei
* @date 2016/06/06 wangwei ��ʼ�汾
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/
#include "../TongueCtrlErrProc.h"
#include "TongueCtrlCmdAnalyze.h"

/**
 * @brief DUMMY�������
 *
 * ����﷨
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
int	CheckDummyCmd(char* in_pCommand)
{
	return 0;
}

/**
 * @brief SHUTDOWN�������
 *
 * ����﷨
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
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
 * @brief RESET�������
 *
 * ����﷨
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
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
 * @brief REBOOT�������
 *
 * ����﷨
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/12/27 wangwei ��ʼ�汾
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
 * @brief LIGHT�������
 *
 * ����﷨
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
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
 * @brief PICTURE�������
 *
 * ����﷨
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
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
 * @brief MOVIE�������
 *
 * ����﷨
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
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
 * @brief LOAD����
 *
 * ����﷨
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
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
 * @brief SAVE����
 *
 * ����﷨
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
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
 * @brief UPLOAD�������
 *
 * ����﷨
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
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
 * @brief SYNC�������
 *
 * ����﷨
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
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
 * @brief DELETE�������
 *
 * ����﷨
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
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
 * @brief SET�������
 *
 * ����﷨
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
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
 * @brief RQ�������
 *
 * ����﷨
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
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
 * @brief ERRRST�������
 *
 * ����﷨
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
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
 * @brief IPCHG�������
 *
 * ����﷨
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
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
 * @brief GETLOG�������
 *
 * ����﷨
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/07/22 wangwei ��ʼ�汾
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
 * @brief UPGRADE�������
 *
 * ����﷨
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/07/22 wangwei ��ʼ�汾
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
 * @brief SEARCH�������
 *
 * ����﷨
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/07/25 wangwei ��ʼ�汾
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
 * @brief SETWIFI�������
 *
 * ����﷨
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/07/27 wangwei ��ʼ�汾
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
 * @brief OBTINFO�������
 *
 * ����﷨
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/09/27 wangwei ��ʼ�汾
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
 * @brief DEVPIC�������
 *
 * ����﷨
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/10/19 wangwei ��ʼ�汾
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
 * @brief DEVMOVIE�������
 *
 * ����﷨
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/10/19 wangwei ��ʼ�汾
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
 * @brief PHOTOCATG�������
 *
 * ���û�õ���̦ͼ������
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2017/07/21 wangwei ��ʼ�汾
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
 * @brief RPMSTART�������
 *
 * ����Rpi Cam����
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2017/09/14 wangwei ��ʼ�汾
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
