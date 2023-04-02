/*****************************************************************************/
/**
* @file TongueCtrlCmdRun.cpp
* @brief ��̦��������ִ��
*
* �����̦��������ִ��
*
* @author wangwei
* @date 2016/06/06 wangwei ��ʼ�汾
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>

#include "TongueCtrlParameter.h"
#include "TongueCtrlErrProc.h"
#include "TongueCtrlCommand.h"
#include "TongueCtrlCommon.h"
#include "TongueCtrl.h"
#include "TongueCtrlUtility.h"
#include "TongueCtrlCamThd.h"
#include "TongueCtrlLogThd.h"
#include "TongueCtrlGPIO.h"
#include "TongueCtrlMsgThd.h"
#include "TongueCtrlCmdRun.h"
#include "TongueCtrlSysMonitorThd.h"

using namespace std;
TongueVidParam_En g_tongueVidParam;
void CreatePictureLink(char* in_ipAddr, char* in_folderName, char* in_originFileName,
						char* in_tongueFileName, char* in_tongueLipFileName,
						char* out_link, int in_category);

/**
 * @brief DUMMY����ִ��
 *
 * ִ������
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
int RunDummyCmd(void* io_pValue)
{
	return 0;
}

/**
 * @brief SHUTDOWN����ִ��
 *
 * ִ������
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
int RunShutdownCmd(void* io_pValue)
{
	ShutDown();
	return 0;
}

/**
 * @brief RESET����ִ��
 *
 * ִ������
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
int RunResetCmd(void* io_pValue)
{
	Reset();
	return 0;
}


/**
 * @brief REBOOT����ִ��
 *
 * ִ������
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/12/27 wangwei ��ʼ�汾
 */
int RunRebootCmd(void* io_pValue)
{
	Reboot();
	return 0;
}

/**
 * @brief LIGHT����ִ��
 *
 * ִ������
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
int RunLightCmd(void* io_pValue)
{
	int iResult = ERR_CMD_FAILED;
	int iCategory =  g_stCommand.unCommand.stCmdLight.uiCategory;
	int iBrightness = g_stCommand.unCommand.stCmdLight.uiBrightness;
	int iOnOff =  g_stCommand.unCommand.stCmdLight.uiEnable;

	// light on/off
	if ((iCategory == IMAGE_LIGHT) || (iCategory == VIDEO_LIGHT)) {
		printf("Light command: %d\n", iCategory);
		printf("g_camStatus: %d\n", g_camStatus);

		if (iCategory == IMAGE_LIGHT) {
			if ((g_camStatus != CAM_IDLE) && (g_camStatus != CAM_VIDEO_STOP)) {	// �����С�����
				iResult = ERR_ALREADY_VIDEO;
			}
			else {
				iResult = ERR_CMD_SUCCESS;
			}
		}
		else if (iCategory == VIDEO_LIGHT) {
			if ((g_camStatus == CAM_VIDEO_START) || (g_camStatus == DEV_CAM_VIDEO_START)) {	// �����С�����
				iResult = ERR_ALREADY_VIDEO;
			}
			else if ((g_camStatus != CAM_IDLE) && (g_camStatus != CAM_VIDEO_STOP)) {	// �����С�����
				iResult = ERR_ALREADY_PHOTO;
			}
			else {
				iResult = ERR_CMD_SUCCESS;
			}
		}
		if (iResult == ERR_CMD_SUCCESS) {
			iResult = SwitchLightOnOff(iCategory, iOnOff);
		}
	}
	else {
		g_iBrightness = iBrightness;
		iResult = SwitchLightOnOff(ILLUME_LIGHT, g_iBrightness);
	}

	return iResult;
}

/**
 * @brief PICTURE����ִ��
 *
 * ִ������
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
int RunPictureCmd(void* io_pValue)
{
	int iResult = ERR_CMD_FAILED;
	char strFileFullName[FILE_MAX_LENGTH];
	char strOriginFileName[FILE_MAX_LENGTH];
	char strTongueFileName[FILE_MAX_LENGTH];
	char strTongueLipFileName[FILE_MAX_LENGTH];
	char strFolderName[FILE_MAX_LENGTH];
	char strOriginFolderName[FILE_MAX_LENGTH];
	char strTongueFolderName[FILE_MAX_LENGTH];
	char strTongueLipFolderName[FILE_MAX_LENGTH];
	char link[RESPONSE_LENGTH];
	char link1[RESPONSE_LENGTH];
	char ipAddr[IP_LENGTH];
	TonguePhotoParam_En param;

	unsigned int uiAction = g_stCommand.unCommand.stCmdPicture.uiAction;

	if ((g_camStatus == CAM_VIDEO_START) || (g_camStatus == DEV_CAM_VIDEO_START)) {
		iResult = ERR_ALREADY_VIDEO;
	}
	else if (g_camStatus == CAM_PHOTO) {
		iResult = ERR_ALREADY_PHOTO;
	}
	else {
		iResult = ERR_CMD_SUCCESS;
	}

	if (iResult == ERR_CMD_SUCCESS) {
		if (uiAction == PHOTO_RETAKE) {
			iResult = Remove(g_stCommand.unCommand.stCmdPicture.fileName);
		}
		else if (uiAction == PHOTO_ABANDON) {
			strncpy(strOriginFileName, g_stCommand.unCommand.stCmdPicture.folderName, FILE_MAX_LENGTH);
			strncat(strOriginFileName, "/", FILE_MAX_LENGTH);
			strncat(strOriginFileName, g_stCommand.unCommand.stCmdPicture.fileName, FILE_MAX_LENGTH);
			iResult = Remove(strOriginFileName);
			strncpy((char*)io_pValue, "_RDY", RESPONSE_LENGTH);
			return iResult;
		}

		if (iResult != ERR_FILE_NOT_EXIST) {
			g_camStatus = CAM_PHOTO;
			// Get IP
			iResult = GetOwnIP(ipAddr);

			char *folderName = g_stCommand.unCommand.stCmdPicture.aNameAddr;

			// �õ���ǰ����ʱ���ַ���
			GetDateTimeString();

			// Create File Name
			strncpy(strOriginFileName, g_strDateTime, FILE_MAX_LENGTH);
			strncat(strOriginFileName, IMAGE_FORMAT, FILE_MAX_LENGTH);
			strncpy(strTongueFileName, g_strDateTime, FILE_MAX_LENGTH);
			strncat(strTongueFileName, TONGUE_FILE, FILE_MAX_LENGTH);
			strncat(strTongueFileName, IMAGE_FORMAT, FILE_MAX_LENGTH);
			strncpy(strTongueLipFileName, g_strDateTime, FILE_MAX_LENGTH);
			strncat(strTongueLipFileName, TONGUE_LIP_FILE, FILE_MAX_LENGTH);
			strncat(strTongueLipFileName, IMAGE_FORMAT, FILE_MAX_LENGTH);

			// �õ���MACΪֹ��Ŀ¼��
			strncpy(strFolderName, APACHE_FOLDER, FILE_MAX_LENGTH);
			strncat(strFolderName, PHOTO_FOLDER, FILE_MAX_LENGTH);
			strncat(strFolderName, folderName, FILE_MAX_LENGTH);

			// original photo folder
			strncpy(strOriginFolderName, strFolderName, FILE_MAX_LENGTH);
			strncat(strOriginFolderName, ORIGIN_FOLDER, FILE_MAX_LENGTH);
			// tongue photo folder
			strncpy(strTongueFolderName, strFolderName, FILE_MAX_LENGTH);
			strncat(strTongueFolderName, TONGUE_FOLDER, FILE_MAX_LENGTH);
			// tongue and lip photo folder
			strncpy(strTongueLipFolderName, strFolderName, FILE_MAX_LENGTH);
			strncat(strTongueLipFolderName, TONGUE_LIP_FOLDER, FILE_MAX_LENGTH);

			// �ж�Ŀ¼�Ƿ����
			if (access(strFolderName, 0) == -1) {	// ������
				printf("base=%s\n", strFolderName);
				if (mkdir(strFolderName, 0755)) {	// ����
					g_TongueCtrlLog.E("Can not create base folder for the image.");
					iResult = ERR_FOLDER_NOT_EXIST;
				}
				else {
					iResult = ERR_CMD_SUCCESS;
				}
			}
			if (access(strOriginFolderName, 0) == -1) {	// ������
				printf("origin=%s\n", strOriginFolderName);
				if (mkdir(strOriginFolderName, 0755)) {	// ����
					g_TongueCtrlLog.E("Can not create folder for the origin image.");
					iResult = ERR_FOLDER_NOT_EXIST;
				}
				else {
					iResult = ERR_CMD_SUCCESS;
				}
			}
			if (access(strTongueFolderName, 0) == -1) {	// ������
				printf("tongue=%s\n", strTongueFolderName);
				if (mkdir(strTongueFolderName, 0755)) {	// ����
					g_TongueCtrlLog.E("Can not create folder for the tongue image.");
					iResult = ERR_FOLDER_NOT_EXIST;
				}
				else {
					iResult = ERR_CMD_SUCCESS;
				}
			}
			if (access(strTongueLipFolderName, 0) == -1) {	// ������
				if (mkdir(strTongueLipFolderName, 0755)) {	// ����
					g_TongueCtrlLog.E("Can not create folder for the tongue and lip image.");
					iResult = ERR_FOLDER_NOT_EXIST;
				}
				else {
					iResult = ERR_CMD_SUCCESS;
				}
			}


			if (iResult == ERR_CMD_SUCCESS) {
				strncpy(strFileFullName, strOriginFolderName, FILE_MAX_LENGTH);
				strncat(strFileFullName, "/", FILE_MAX_LENGTH);
				strncat(strFileFullName, strOriginFileName, FILE_MAX_LENGTH);
				strncpy(g_stCommand.unCommand.stCmdPicture.fileName, strOriginFileName, FILE_MAX_LENGTH);
				strncpy(g_stCommand.unCommand.stCmdPicture.folderName, strOriginFolderName, FILE_MAX_LENGTH);

				// from process start

				param.sFileName = g_stCommand.unCommand.stCmdPicture.fileName;
				param.sFolderName = g_stCommand.unCommand.stCmdPicture.folderName;
				param.sTongueFileName = strTongueFileName;
				param.sTongueFolderName = strTongueFolderName;
				param.sTongueLipFileName = strTongueLipFileName;
				param.sTongueLipFolderName = strTongueLipFolderName;
				// ���
	//			SwitchLightOnOff(ILLUME_LIGHT, 1);
				iResult = StartProcess(TONGUE_PHOTO, &param);
				g_TongueCtrlLog.D("Take Picture End.");
			}

			if (iResult == ERR_CMD_SUCCESS) {
				iResult = CreatePhoto(&param, g_photoCategory);
				if (iResult == ERR_CMD_SUCCESS) {
					if (g_photoCategory == ORIGIN) {
						strncpy(link, "http://", RESPONSE_LENGTH);
						strncat(link, ipAddr, RESPONSE_LENGTH);
						strncat(link, "/html/", RESPONSE_LENGTH);
						strncat(link, PHOTO_FOLDER, RESPONSE_LENGTH);
						strncat(link, folderName, RESPONSE_LENGTH);
						strncat(link, ORIGIN_FOLDER, RESPONSE_LENGTH);
						strncat(link, strOriginFileName, RESPONSE_LENGTH);
					}
					else if (g_photoCategory == ORIGIN_TONGUE) {
						strncpy(link1, "http://", RESPONSE_LENGTH);
						strncat(link1, ipAddr, RESPONSE_LENGTH);
						strncat(link1, "/html/", RESPONSE_LENGTH);
						strncat(link1, PHOTO_FOLDER, RESPONSE_LENGTH);
						strncat(link1, folderName, RESPONSE_LENGTH);
						strncat(link1, TONGUE_FOLDER, RESPONSE_LENGTH);
						strncat(link1, strTongueFileName, RESPONSE_LENGTH);
					}

					CreatePictureLink(ipAddr, folderName, strOriginFileName, strTongueFileName,
							 strTongueLipFileName, link, g_photoCategory);
					if (g_stReceptionCond.uiCurSysCmdStatus!=DEVICE_EXEC) {
							strncpy((char*)io_pValue, link, RESPONSE_LENGTH);
					}
					else {
						if (g_stReceptionCond.uiCurDevStatus == DEVICE_EXEC) {	// �豸����
							g_stCommand.unCommand.stCmdDevPic.uiAction = 1;	// ���ս���
	//						strncpy(g_stCommand.unCommand.stCmdDevPic.sResultLink, link, RESPONSE_LENGTH);
	//						iResult = RunDevPicCmd(io_pValue);		// not doing
							PlaySound(PHOTO_SHUTTER_SOUND);
	//						g_iPlaySound = PICTURE_START_SOUND;
	//						sem_post(&g_semPlaySound);
							SwitchLightOnOff(ILLUME_LIGHT, 0);
						}
					}
				}
			}
			else {
				if (g_stReceptionCond.uiCurDevStatus == DEVICE_EXEC) {	// �豸����
					PlaySound(PHOTO_VIDEO_FAILED_SOUND);
	//				g_iPlaySound = FAILED_SOUND;
	//				sem_post(&g_semPlaySound);
				}
			}
			// Ϩ��
	//		SwitchLightOnOff(ILLUME_LIGHT, 0);
			g_camStatus = CAM_IDLE;
		}
	}

	return iResult;
}

/**
 * @brief MOVIE����ִ��
 *
 * ִ������
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
int RunMovieCmd(void* io_pValue)
{
	int iResult = ERR_CMD_FAILED;
	char strFileFullName[FILE_MAX_LENGTH];
	char strMp4FullName[FILE_MAX_LENGTH];
//	char strFileName[FILE_MAX_LENGTH];
	char strFolderName[FILE_MAX_LENGTH];
	char *folderName = g_stCommand.unCommand.stCmdMovie.aNameAddr;
	char link[RESPONSE_LENGTH];
	char mp4Link[RESPONSE_LENGTH];
	char ipAddr[IP_LENGTH];

	// Get IP
	iResult = GetOwnIP(ipAddr);
	unsigned int uiStatus = g_stCommand.unCommand.stCmdMovie.uiAction;
	if (uiStatus == VIDEO_START) {
		// �õ���ǰ����ʱ���ַ���
		GetDateTimeString();
	}
	strcpy(g_stCommand.unCommand.stCmdMovie.fileNameNoHead, g_strDateTime);
	strcat(g_stCommand.unCommand.stCmdMovie.fileNameNoHead, VIDEO_FORMAT);
	strcpy(g_stCommand.unCommand.stCmdMovie.mp4NoHead, g_strDateTime);
	strcat(g_stCommand.unCommand.stCmdMovie.mp4NoHead, MP4_FORMAT);

	strcpy(strFolderName, APACHE_FOLDER);
	strcat(strFolderName, VIDEO_FOLDER);
	strcat(strFolderName, folderName);

	// �ж�Ŀ¼�Ƿ����
	if (access(strFolderName, 0) == -1) {	// ������
		if (mkdir(strFolderName, 0755)) {	// ����
			g_TongueCtrlLog.E("Can not create folder for the image.");
		}
		else {
			iResult = ERR_CMD_SUCCESS;
		}
	}
	else {
		iResult = ERR_CMD_SUCCESS;
	}

	if (iResult == ERR_CMD_SUCCESS) {
		strcpy(strFileFullName, strFolderName);
		strcat(strFileFullName, "/");
		strcat(strFileFullName, g_strDateTime);
		strcpy(strMp4FullName, strFileFullName); // For mp4
		strcat(strFileFullName, VIDEO_FORMAT);
		strcat(strMp4FullName, MP4_FORMAT);

		strncpy(g_stCommand.unCommand.stCmdMovie.fileName, strFileFullName, FILE_MAX_LENGTH);
		strncpy(g_stCommand.unCommand.stCmdMovie.mp4FileName, strMp4FullName, FILE_MAX_LENGTH);
		strncpy(g_stCommand.unCommand.stCmdMovie.folderName, strFolderName, FILE_MAX_LENGTH);
	}

	if (uiStatus == VIDEO_START) {
		if ((g_camStatus != CAM_IDLE) && (g_camStatus != CAM_VIDEO_STOP)) {
			iResult = ERR_VIDEO_NOTBEGIN;
		}
		else {
			strncpy((char*)io_pValue, "_RDY", RESPONSE_LENGTH);

			g_tongueVidParam.iWidth = g_imageCtrlParam.iResolutionX;
			g_tongueVidParam.iHeigth = g_imageCtrlParam.iResolutionY;
			g_tongueVidParam.sFileName = g_stCommand.unCommand.stCmdMovie.mp4NoHead;
			g_tongueVidParam.sFolderName = g_stCommand.unCommand.stCmdMovie.folderName;
			g_tongueVidParam.iType = 1;
			// ��������
			SwitchLightOnOff(ILLUME_LIGHT, 1);
			// LED�ƺ���
			SwitchLightOnOff(VIDEO_LIGHT, ENABLE_DONE);
			g_camStatus = CAM_VIDEO_START;
			sem_post(&g_semCamera);

			if ((g_stReceptionCond.uiCurDevStatus == DEVICE_EXEC) && (g_stReceptionCond.uiCurSysCmdStatus == DEVICE_EXEC)) { // command from device
				// ��������ʱ
//				TimerSet(g_stCommand.unCommand.stCmdMovie.aNameAddr, VIDEO_TIMEOUT_TIME);
				g_stCommand.unCommand.stCmdDevMovie.uiAction = VIDEO_START;	// ����ʼ
//				iResult = RunDevMovieCmd(io_pValue);	// not doing
				g_stReceptionCond.uiCurSysCmdStatus = SYSTEM_READY;
				g_camStatus = DEV_CAM_VIDEO_START;
			}
			else {
				iResult = ERR_CMD_SUCCESS;
			}

		}
	}
	else if ((uiStatus == VIDEO_STOP) || (uiStatus == VIDEO_TIMEOUT)) {	// stop or timeout
		if ((g_camStatus != CAM_VIDEO_START) && (g_camStatus != DEV_CAM_VIDEO_START)) {
			iResult = ERR_VIDEO_NOTBEGIN;
		}
		else {
			g_camStatus = CAM_VIDEO_STOP;
			iResult = StartProcess(TONGUE_VIDEO_END, &g_tongueVidParam);
			if (iResult == ERR_CMD_SUCCESS) {
				strcpy(link, "http://");
				strcat(link, ipAddr);
				strcat(link, "/html/");
				strcat(link, VIDEO_FOLDER);
				strcat(link, folderName);
				strcat(link, "/");
				strcpy(mp4Link, link);
				strcat(mp4Link, g_stCommand.unCommand.stCmdMovie.mp4NoHead);

				printf("%d %d\n", g_stReceptionCond.uiCurDevStatus, g_stReceptionCond.uiCurSysCmdStatus);
				if ((g_stReceptionCond.uiCurDevStatus == DEVICE_EXEC) && (g_stReceptionCond.uiCurSysCmdStatus == DEVICE_EXEC)) {	// �豸��������
					g_stCommand.unCommand.stCmdDevMovie.uiAction = 1;	// �������
//					strncpy(g_stCommand.unCommand.stCmdDevMovie.sResultLink, mp4Link, RESPONSE_LENGTH);
//					iResult = RunDevMovieCmd(io_pValue);	// not doing
					PlaySound(VIDEO_END_SHUTTER_SOUND);
//					g_iPlaySound = VIDEO_END_SOUND;
//					sem_post(&g_semPlaySound);
					SwitchLightOnOff(ILLUME_LIGHT, 0);
				}
				else {
					strncpy((char*)io_pValue, mp4Link, RESPONSE_LENGTH);
				}
			}
			else {

				if ((g_stReceptionCond.uiCurDevStatus == DEVICE_EXEC) && (g_stReceptionCond.uiCurSysCmdStatus == DEVICE_EXEC)) {	// �豸��������
					PlaySound(PHOTO_VIDEO_FAILED_SOUND);
//					g_iPlaySound = FAILED_SOUND;
//					sem_post(&g_semPlaySound);
				}
			}

//			g_camStatus = CAM_VIDEO_STOP;
//			TimerSet(g_stCommand.unCommand.stCmdMovie.aNameAddr, 0);

			// Ϩ������
			SwitchLightOnOff(ILLUME_LIGHT, 0);
			// LED�Ƶ���
			SwitchLightOnOff(VIDEO_LIGHT, ENABLE_ON);
		}
//		SysMonitorTimerSet();
	}
	else if (uiStatus == VIDEO_QUIT) {
		if ((g_camStatus != CAM_VIDEO_START) && (g_camStatus != DEV_CAM_VIDEO_START)) {
			iResult = ERR_VIDEO_NOTBEGIN;
		}
		else {
			sem_post(&g_semMsgQueue);
			iResult = ERR_CMD_SUCCESS;
//			iResult = Remove(g_tongueVidParam.sFileName);
			iResult = StartProcess(TONGUE_VIDEO_QUIT, &g_tongueVidParam);
			g_camStatus = CAM_VIDEO_STOP;
			if ((g_stReceptionCond.uiCurDevStatus == DEVICE_EXEC) && (g_stReceptionCond.uiCurSysCmdStatus == DEVICE_EXEC)) {	// �豸����ʱ
				g_stCommand.unCommand.stCmdDevMovie.uiAction = 2;	// ����ʱ
//				iResult = RunDevMovieCmd(io_pValue);	// not doing
			}
			// Ϩ������
			SwitchLightOnOff(ILLUME_LIGHT, 0);
			// LED�Ƶ���
			SwitchLightOnOff(VIDEO_LIGHT, ENABLE_ON);
			g_MovieState = STOP;
		}
	}


	return iResult;
}

/**
 * @brief LOAD�R�}���h���s
 *
 * �R�}���h�����s����B
 *
 * @param pCommand �R�}���h
 * @return 0:����@1:�ُ�
 * @author wangwei
 * @date 2015/03/24 wangwei �V�K�쐬
 */
int RunLoadCmd(void* io_pValue)
{
	int iResult = ERR_CMD_FAILED;

	return iResult;
}

/**
 * @brief SAVE�R�}���h���s
 *
 * �R�}���h�����s����B
 *
 * @param pCommand �R�}���h
 * @return 0:����@1:�ُ�
 * @author wangwei
 * @date 2015/03/24 wangwei �V�K�쐬
 */
int RunSaveCmd(void* io_pValue)
{
	int iResult = ERR_CMD_FAILED;


	return iResult;
}


/**
 * @brief UPLOAD����ִ��
 *
 * ִ������
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/06/07 wangwei ��ʼ�汾
 */
int RunUploadCmd(void* io_pValue)
{
	int iResult = ERR_CMD_FAILED;

	// ��������url���������ͻ��ˡ�

	return iResult;
}

/**
 * @brief SYNC����ִ��
 *
 * ִ������
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/06/07 wangwei ��ʼ�汾
 */
int RunSyncCmd(void* io_pValue)
{
	int iResult = ERR_CMD_FAILED;
	FILE *fp = NULL;
//	char strFolderFullName[FILE_MAX_LENGTH];
	char fileName[FILENAME_LENGTH];
	char destFileName[FILENAME_LENGTH];
	char link[RESPONSE_LENGTH];
	char ipAddr[IP_LENGTH];
	const char* pStrRelt = NULL;
	char mac[NAME_LENGTH];

	iResult = GetOwnIP(ipAddr);

	// Get mac address
	GetMacAddr(mac);

	if ((g_stCommand.unCommand.stCmdSync.uiCategory == IMAGE)
		|| (g_stCommand.unCommand.stCmdSync.uiCategory == IMG_VIDEO_BOTH)) {
		// ͬ��ͼƬ
		pStrRelt = CreateImageJSONObj(g_stCommand.unCommand.stCmdSync.aNameAddr, mac, ipAddr);
	}
	else if ((g_stCommand.unCommand.stCmdSync.uiCategory == VIDEO)
		|| (g_stCommand.unCommand.stCmdSync.uiCategory == IMG_VIDEO_BOTH)) {
		// ͬ����Ƶ
		pStrRelt = CreateVideoJSONObj(g_stCommand.unCommand.stCmdSync.aNameAddr, mac, ipAddr);
	}

	if (pStrRelt == NULL) {
		iResult = ERR_NO_USER;
		g_TongueCtrlLog.E("Sync = %d", iResult);
	}
	else {
		iResult = ERR_CMD_SUCCESS;
		g_TongueCtrlLog.D("Sync = %d", iResult);
	}

	if (iResult == ERR_CMD_SUCCESS) {
		strncpy(fileName, g_stCommand.unCommand.stCmdSync.aNameAddr, FILENAME_LENGTH);
		strncat(fileName, ".json", FILENAME_LENGTH);
		if ((g_stCommand.unCommand.stCmdSync.uiCategory == IMAGE)) {
			strncpy(destFileName, APACHE_FOLDER, FILE_MAX_LENGTH);
			strncat(destFileName, PHOTO_FOLDER, FILE_MAX_LENGTH);
			strncat(destFileName, fileName, FILE_MAX_LENGTH);
		}
		else if ((g_stCommand.unCommand.stCmdSync.uiCategory == VIDEO)) {
			strncpy(destFileName, APACHE_FOLDER, FILE_MAX_LENGTH);
			strncat(destFileName, VIDEO_FOLDER, FILE_MAX_LENGTH);
			strncat(destFileName, fileName, FILE_MAX_LENGTH);
		}

		fp = fopen(destFileName, "wa");

		if (fp != NULL) {
			while (*pStrRelt != '\0') {
				if (*pStrRelt != '\\') {
					fputc(*pStrRelt, fp);
				}
				pStrRelt++;
			}
			fclose(fp);
			iResult = ERR_CMD_SUCCESS;
		}
		g_TongueCtrlLog.D("Sync file created.");

		if ((g_stCommand.unCommand.stCmdSync.uiCategory == IMAGE)) {
			if (iResult == ERR_CMD_SUCCESS) {
				strcpy(link, "http://");
				strcat(link, ipAddr);
				strcat(link, "/html/");
				strcat(link, PHOTO_FOLDER);
				strcat(link, fileName);
				strncpy((char*)io_pValue, link, RESPONSE_LENGTH);
			}
		}
		else if ((g_stCommand.unCommand.stCmdSync.uiCategory == VIDEO)) {
			if (iResult == ERR_CMD_SUCCESS) {
				strcpy(link, "http://");
				strcat(link, ipAddr);
				strcat(link, "/html/");
				strcat(link, VIDEO_FOLDER);
				strcat(link, fileName);
				strncpy((char*)io_pValue, link, RESPONSE_LENGTH);
			}
		}

		g_TongueCtrlLog.D("Sync link: %s", link);
	}

	return iResult;
}

/**
 * @brief Delete����ִ��
 *
 * ִ������
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
int RunDeleteCmd(void* io_pValue)
{
	int iResult = ERR_CMD_FAILED;
	unsigned int i = 0;
	char fileName[FILENAME_LENGTH];
	char strFolderName[FILE_MAX_LENGTH];
	char strDeviceFolderName[FILE_MAX_LENGTH];
	char strOperFolderName[FILE_MAX_LENGTH];
	char strOperDevFolderName[FILE_MAX_LENGTH];
	char mac[NAME_LENGTH];
	unsigned int uiAction = g_stCommand.unCommand.stCmdDelete.uiMount;
	unsigned int uiMount = g_stCommand.unCommand.stCmdDelete.uiMount;
	unsigned int uiCategory = g_stCommand.unCommand.stCmdDelete.uiCategory;
	int iFileCategory = 0;

	// Get mac address
	GetMacAddr(mac);

	if (uiCategory == IMAGE) {	// ͼ��
		// user directory
		strcpy(strFolderName, APACHE_FOLDER);
		strncat(strFolderName, PHOTO_FOLDER, FILENAME_LENGTH);
		strncat(strFolderName, g_stCommand.unCommand.stCmdDelete.aNameAddr, FILENAME_LENGTH);
		strcat(strFolderName, "/");
		// device directory
		strcpy(strDeviceFolderName, APACHE_FOLDER);
		strncat(strDeviceFolderName, PHOTO_FOLDER, FILENAME_LENGTH);
		strncat(strDeviceFolderName, mac, FILENAME_LENGTH);
		strcat(strDeviceFolderName, "/");

		// ɾ�������ļ�
		if (uiAction == DELETE_ALL) {
			iResult = RemoveAll(strFolderName);
		}
		else { // ɾ��ָ���ļ�
			while (i < uiMount) {
				strncpy(fileName, g_stCommand.unCommand.stCmdDelete.aDeleteFile[i], FILENAME_LENGTH);
				iFileCategory = JudgeFileCategory(fileName, 3);
				strncpy(strOperFolderName, strFolderName, FILENAME_LENGTH);
				strncpy(strOperDevFolderName, strDeviceFolderName, FILENAME_LENGTH);
				if (iFileCategory == 0) {
					strncat(strOperFolderName, ORI_DIR_NOSLASH, FILENAME_LENGTH);
					strncat(strOperDevFolderName, ORI_DIR_NOSLASH, FILENAME_LENGTH);
				}
				else if (iFileCategory == 1) {
					strncat(strOperFolderName, TON_DIR_NOSLASH, FILENAME_LENGTH);
					strncat(strOperDevFolderName, TON_DIR_NOSLASH, FILENAME_LENGTH);
				}
				else if (iFileCategory == 2) {
					strncat(strOperFolderName, TON_LIP_DIR_NOSLASH, FILENAME_LENGTH);
					strncat(strOperDevFolderName, TON_LIP_DIR_NOSLASH, FILENAME_LENGTH);
				}
				strcpy(fileName, strOperFolderName);
				strncat(fileName, g_stCommand.unCommand.stCmdDelete.aDeleteFile[i], FILENAME_LENGTH);
				printf("%s\n", fileName);
				iResult = Remove(fileName);
				if (iResult != ERR_CMD_SUCCESS) { // δ�ɹ������豸Ŀ¼��ɾ
					strcpy(fileName, strOperDevFolderName);
					strncat(fileName, g_stCommand.unCommand.stCmdDelete.aDeleteFile[i], FILENAME_LENGTH);
					iResult = Remove(fileName);
					if (iResult != ERR_CMD_SUCCESS) {
						break;
					}
				}

				i++;
			}
		}
	}
	else if (uiCategory == VIDEO) {	// ͼ��
		// user directory
		strcpy(strFolderName, APACHE_FOLDER);
		strncat(strFolderName, VIDEO_FOLDER, FILENAME_LENGTH);
		strncat(strFolderName, g_stCommand.unCommand.stCmdDelete.aNameAddr, FILENAME_LENGTH);
		strcat(strFolderName, "/");
		// device directory
		strcpy(strDeviceFolderName, APACHE_FOLDER);
		strncat(strDeviceFolderName, VIDEO_FOLDER, FILENAME_LENGTH);
		strncat(strDeviceFolderName, mac, FILENAME_LENGTH);
		strcat(strDeviceFolderName, "/");
		// ɾ�������ļ�
		if (uiAction == DELETE_ALL) {
			strcpy(fileName, strFolderName);
			strncat(fileName, g_stCommand.unCommand.stCmdDelete.aNameAddr, FILENAME_LENGTH);
			iResult = RemoveAll(strFolderName);
		}
		else { // ɾ��ָ���ļ�
			while (i < uiMount) {
				strcpy(fileName, strFolderName);
				strncat(fileName, g_stCommand.unCommand.stCmdDelete.aDeleteFile[i], FILENAME_LENGTH);
				iResult = Remove(fileName);
				if (iResult != ERR_CMD_SUCCESS) { // δ�ɹ������豸Ŀ¼��ɾ
					strcpy(fileName, strDeviceFolderName);
					strncat(fileName, g_stCommand.unCommand.stCmdDelete.aDeleteFile[i], FILENAME_LENGTH);
					iResult = Remove(fileName);
					if (iResult != ERR_CMD_SUCCESS) {
						break;
					}
				}

				i++;
			}
		}
	}

	return iResult;
}

/**
 * @brief SET����ִ��
 *
 * ִ������
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
int RunSetCmd(void* io_pValue)
{
	int iResult = ERR_CMD_SUCCESS;
	unsigned int uiItemNo = g_stCommand.unCommand.stCmdSet.uiItem;


	// g_systemParamMap�����±�ļ���
	unsigned int uiMapNo = GetParamIdForSetting(uiItemNo);

	if (g_stCommand.unCommand.stCmdSet.bCharValue
		&& g_systemParamMap[uiMapNo].enDataType != IP_TYPE) {
		iResult = ERR_SET_VALUE;
	}
	else if (!g_stCommand.unCommand.stCmdSet.bCharValue
			&& g_systemParamMap[uiMapNo].enDataType == IP_TYPE) {
			iResult = ERR_SET_VALUE;
	}
	else if (g_stCommand.unCommand.stCmdSet.bFloatValue
		&& g_systemParamMap[uiMapNo].enDataType != FLOAT_TYPE) {
		iResult = ERR_SET_VALUE;
	}
	else if (!g_stCommand.unCommand.stCmdSet.bFloatValue
			 && g_systemParamMap[uiMapNo].enDataType == FLOAT_TYPE) {
		g_stCommand.unCommand.stCmdSet.value.fValue =
				(float)g_stCommand.unCommand.stCmdSet.value.iValue;
		g_stCommand.unCommand.stCmdSet.bFloatValue = true;
	}
	if (iResult == ERR_CMD_SUCCESS) {
		iResult = g_systemParamMap[uiMapNo].set(
				g_stCommand.unCommand.stCmdSet.uiItem,
				g_stCommand.unCommand.stCmdSet.value);
	}


	return iResult;
}

/**
 * @brief RQ����ִ��
 *
 * ִ������
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
int RunRqCmd(void* io_pValue)
{
	char *pChar = (char*)io_pValue;
	int32_t iResult = ERR_CMD_SUCCESS;
	NumericType rqRelt;
	bool bFloat = false;
	bool bIp = false;
	char ch1[PARAM_LENGTH];
	char ch2[PARAM_LENGTH];

	uint32_t uiItemNo = g_stCommand.unCommand.stCmdSet.uiItem;

	unsigned int uiMapNo = GetParamIdForSetting(uiItemNo);
	rqRelt = g_systemParamMap[uiMapNo].request(g_stCommand.unCommand.stCmdSet.uiItem);

	if (g_systemParamMap[uiMapNo].enDataType == FLOAT_TYPE) {
		bFloat = true;
	}
	else if (g_systemParamMap[uiMapNo].enDataType == IP_TYPE) {
		bIp = true;
	}

	sprintf(ch1, "%d", g_stCommand.unCommand.stCmdSet.uiItem);
	if (!bFloat && !bIp) {
		sprintf(ch2, "%d", rqRelt.iValue);
	}
	else if (bIp) {
		IntToIp(rqRelt.iValue, ch2);
	}
	else {
		sprintf(ch2, "%f", rqRelt.fValue);
	}
	strncat(ch1, BLANKCHAR, 2);
	strncat(ch1, ch2, PARAM_LENGTH);

	strncpy(pChar, ch1, PARAM_LENGTH);

	return iResult;
}

/**
 * @brief ERRRST����ִ��
 *
 * ִ������
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
int RunErrrstCmd(void* io_pValue)
{
	g_stReceptionCond.uiCurSysCmdStatus = SYSTEM_READY;
	return 0;
}

/**
 * @brief IPCHG����ִ��
 *
 * ִ������
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
int RunIpChgCmd(void* io_pValue)
{
	IpChg();
	return 0;
}

/**
 * @brief GETLOG����ִ��
 *
 * ִ������
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/07/22 wangwei ��ʼ�汾
 */
int RunGetLogCmd(void* io_pValue)
{
	int iResult = ERR_CMD_FAILED;
//	char strFileFullName[FILE_MAX_LENGTH];
	char strFileName[FILE_MAX_LENGTH];
	char strFolderName[FILE_MAX_LENGTH];
	char ipAddr[IP_LENGTH];
	char link[RESPONSE_LENGTH];

	iResult = GetOwnIP(ipAddr);

	strcpy(strFolderName, APACHE_FOLDER);
	strncat(strFolderName, LOG_FOLDER, FILE_MAX_LENGTH);

	CreateLogFile(strFolderName, LOG_TAR_FILE);

	strcpy(strFileName, LOG_TAR_FILE);

	if (iResult == ERR_CMD_SUCCESS) {
		strcpy(link, "http://");
		strcat(link, ipAddr);
		strcat(link, "/html/");
		strcat(link, LOG_FOLDER);
		strcat(link, strFileName);
		strncpy((char*)io_pValue, link, RESPONSE_LENGTH);
	}
	return iResult;
}

/**
 * @brief UPGRADE����ִ��
 *
 * ִ������
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/07/22 wangwei ��ʼ�汾
 */
int RunUpgradeCmd(void* io_pValue)
{
	int iResult = ERR_CMD_SUCCESS;

	if (g_iUpgradeProc == 0) {
		printf("StartUpgrade.\n");
		g_TongueCtrlLog.D("RunUpgradeCmd��");
		StartUpgrade();
		g_iUpgradeProc = 1;
	}

	return iResult;
}


/**
 * @brief SEARCH����ִ��
 *
 * ִ������
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/07/25 wangwei ��ʼ�汾
 */
int RunSearchCmd(void* io_pValue)
{
	int iResult = ERR_CMD_FAILED;
	char mac[NAME_LENGTH];
	char rslt[RESPONSE_LENGTH];
	char hostName[HOSTNAME_LENGTH];
	char ipAddr[IP_LENGTH];
	char version[VERSION_LENGTH];

	// Get IP
	iResult = GetOwnIP(ipAddr);

	if (iResult == ERR_CMD_SUCCESS) {
		// Get mac address
		GetMacAddr(mac);
		if (gethostname(hostName, RESPONSE_LENGTH) == 0) {
			strncpy(rslt, "SEARCH ", RESPONSE_LENGTH);
			strncat(rslt, mac, RESPONSE_LENGTH);
			strncat(rslt, " ", RESPONSE_LENGTH);
			strncat(rslt, ipAddr, RESPONSE_LENGTH);
			strncat(rslt, " ", RESPONSE_LENGTH);
			// Get version
			iResult = GetFirmareVersion(version);
			if (iResult != ERR_CMD_SUCCESS) {
				strncat(rslt, VERSION, RESPONSE_LENGTH);
			}
			else {
				strncat(rslt, version, RESPONSE_LENGTH);
			}
			strncat(rslt, " ", RESPONSE_LENGTH);
			strncat(rslt, hostName, RESPONSE_LENGTH);
			strncpy((char*)io_pValue, rslt, RESPONSE_LENGTH);

//			if (g_stCommand.unCommand.stCmdSearch.ulUTC) {
//				// SetTime
//				SetTime(g_stCommand.unCommand.stCmdSearch.ulUTC);
//			}
		}
		else {
			printf("SEARCH gethostname error.\n");
			g_TongueCtrlLog.E("SEARCH gethostname error��");
			iResult = ERR_CMD_FAILED;
		}
	}

	return iResult;
}


/**
 * @brief SETWIFI����ִ��
 *
 * ִ������
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/07/27 wangwei ��ʼ�汾
 */
int RunSetWifiCmd(void* io_pValue)
{
	int iResult = ERR_CMD_SUCCESS;
	char *pWifiSsid;
	char *pWifiPass;
	unsigned int uiCategory;
	char rslt[5];

	CheckWifiStatus();
	uiCategory = g_stCommand.unCommand.stCmdSetWifi.category;
	if (uiCategory == WIFI_HOTSPOT) {
		printf("Wifi Hotspot.\n");
		if (g_iWifiStatus != WIFI_HOTSPOT) {
			ChangeToHotspot();
		}
	}
	else if (uiCategory == WIFI_STATION) {
		printf("Wifi station.\n");
		if (g_iWifiStatus != WIFI_STATION) {
			pWifiSsid = g_stCommand.unCommand.stCmdSetWifi.ssid;
			pWifiPass = g_stCommand.unCommand.stCmdSetWifi.pass;
			ChangeToStation(pWifiSsid, pWifiPass);
		}
	}
	else {
		CheckWifiStatus();
		sprintf(rslt, "%d", g_iWifiStatus);
		strncpy((char*)io_pValue, rslt, RESPONSE_LENGTH);
	}



	return iResult;
}

/**
 * @brief OBTINFO����ִ��
 *
 * ִ������
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/09/23 wangwei ��ʼ�汾
 */
int RunObtInfoCmd(void* io_pValue)
{
	int iResult = ERR_CMD_FAILED;
	char mac[NAME_LENGTH];
	char ipAddr[IP_LENGTH];
	char version[VERSION_LENGTH];
	char rslt[RESPONSE_LENGTH];
	unsigned int uiCategory;

	uiCategory = g_stCommand.unCommand.stCmdObtInfo.uiInfo;

	if (uiCategory == DEV_INFO_MAC) {
		iResult = GetMacAddr(mac);
		strncpy(rslt, mac, NAME_LENGTH);
	}
	else if (uiCategory == DEV_INFO_IP) {
		iResult = GetOwnIP(ipAddr);
		strncpy(rslt, ipAddr, NAME_LENGTH);
	}
	else if (uiCategory == DEV_INFO_VER) {
		iResult = GetFirmareVersion(version);
		strncpy(rslt, version, NAME_LENGTH);
	}

	if (iResult == ERR_CMD_SUCCESS) {
		strncpy((char*)io_pValue, rslt, RESPONSE_LENGTH);
	}

	return iResult;
}


/**
 * @brief DEVPIC����ִ��
 *
 * ִ������
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/10/19 wangwei ��ʼ�汾
 */
int RunDevPicCmd(void* io_pValue)
{
	int iResult = ERR_CMD_FAILED;
	char rslt[RESPONSE_LENGTH];
	unsigned int uiAction;

	strncpy(rslt, "DEVPIC ", RESPONSE_LENGTH);
	uiAction =  g_stCommand.unCommand.stCmdDevPic.uiAction;

	if (uiAction == 0) { // ���տ�ʼ
		strncat(rslt, g_stCommand.unCommand.stCmdDevPic.mac, RESPONSE_LENGTH);
		strncat(rslt, " 0\r", RESPONSE_LENGTH);
		iResult = ERR_CMD_SUCCESS;
	}
	else if (uiAction == 1) { // ���ս���
		strncat(rslt, g_stCommand.unCommand.stCmdDevPic.mac, RESPONSE_LENGTH);
		strncat(rslt, " 1 ", RESPONSE_LENGTH);
		strncat(rslt, g_stCommand.unCommand.stCmdDevPic.sResultLink, RESPONSE_LENGTH);
		iResult = ERR_CMD_SUCCESS;
	}

	if (iResult == ERR_CMD_SUCCESS) {
		strncpy((char*)io_pValue, rslt, RESPONSE_LENGTH);
	}

	return iResult;

}


/**
 * @brief DEVMOVIE����ִ��
 *
 * ִ������
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2016/10/19 wangwei ��ʼ�汾
 */
int RunDevMovieCmd(void* io_pValue)
{
	int iResult = ERR_CMD_FAILED;
	char rslt[RESPONSE_LENGTH];
	unsigned int uiAction;

	strncpy(rslt, "DEVMOVIE ", RESPONSE_LENGTH);
	uiAction =  g_stCommand.unCommand.stCmdDevMovie.uiAction;

	if (uiAction == 0) { // ����ʼ
		strncat(rslt, g_stCommand.unCommand.stCmdDevMovie.mac, RESPONSE_LENGTH);
		strncat(rslt, " 0\r", RESPONSE_LENGTH);
		iResult = ERR_CMD_SUCCESS;
		printf("Dev movie start.\n");
	}
	else if (uiAction == 1) { // �������
		strncat(rslt, g_stCommand.unCommand.stCmdDevMovie.mac, RESPONSE_LENGTH);
		strncat(rslt, " 1 ", RESPONSE_LENGTH);
		strncat(rslt, g_stCommand.unCommand.stCmdDevMovie.sResultLink, RESPONSE_LENGTH);
		iResult = ERR_CMD_SUCCESS;
		printf("Dev movie end.\n");
	}
	else if (uiAction == 2) { // ����ʱ
		strncat(rslt, g_stCommand.unCommand.stCmdDevMovie.mac, RESPONSE_LENGTH);
		strncat(rslt, " 1 ", RESPONSE_LENGTH);
		strncat(rslt, g_stCommand.unCommand.stCmdDevMovie.sResultLink, RESPONSE_LENGTH);
		iResult = ERR_CMD_SUCCESS;
		printf("Dev movie timeout.\n");
	}

	if (iResult == ERR_CMD_SUCCESS) {
		strncpy((char*)io_pValue, rslt, RESPONSE_LENGTH);
	}

	return iResult;
}

/**
 * @brief PHOTOCATE����ִ��
 *
 * ִ������
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2017/07/21 wangwei ��ʼ�汾
 */
int RunPhotoCatgCmd(void* io_pValue)
{
	int iResult = ERR_CMD_SUCCESS;
	char category[PHOTO_CATE_LENGTH];

	g_photoCategory =  (PhotoCategory_En)g_stCommand.unCommand.stCmdPhotoCate.uiCategory;
	sprintf(category, "%d", g_stCommand.unCommand.stCmdPhotoCate.uiCategory);
	// Set to file for saving
	SetPhotoCategory(category);

	printf("photoCategory=%d\n", g_photoCategory);

	return iResult;
}


/**
 * @brief RPMSTART����ִ��
 *
 * ִ������
 *
 * @param pCommand ����
 * @return 0:����  1:�쳣
 * @author wangwei
 * @date 2017/09/14 wangwei ��ʼ�汾
 */
int RunRpmStartCmd(void* io_pValue)
{
	int iResult = ERR_CMD_SUCCESS;

	RpmStart();

	return iResult;
}

/**
 * @brief ����Link������
 *
 * ����Link������
 *
 * @param pCommand ����
 * @return
 * @author wangwei
 * @date 2017/08/28 wangwei ��ʼ�汾
 */
void CreatePictureLink(char* in_ipAddr, char* in_folderName, char* in_originFileName,
						char* in_tongueFileName, char* in_tongueLipFileName,
						char* out_link, int in_category)
{
	char link[RESPONSE_LENGTH];
	char link1[RESPONSE_LENGTH];
	char link2[RESPONSE_LENGTH];

	strncpy(link, "http://", RESPONSE_LENGTH);
	strncat(link, in_ipAddr, RESPONSE_LENGTH);
	strncat(link, "/html/", RESPONSE_LENGTH);
	strncat(link, PHOTO_FOLDER, RESPONSE_LENGTH);
	strncat(link, in_folderName, RESPONSE_LENGTH);
	strncat(link, ORIGIN_FOLDER, RESPONSE_LENGTH);
	strncat(link, in_originFileName, RESPONSE_LENGTH);

	strncpy(link1, "http://", RESPONSE_LENGTH);
	strncat(link1, in_ipAddr, RESPONSE_LENGTH);
	strncat(link1, "/html/", RESPONSE_LENGTH);
	strncat(link1, PHOTO_FOLDER, RESPONSE_LENGTH);
	strncat(link1, in_folderName, RESPONSE_LENGTH);
	strncat(link1, TONGUE_FOLDER, RESPONSE_LENGTH);
	strncat(link1, in_tongueFileName, RESPONSE_LENGTH);

	strncpy(link2, "http://", RESPONSE_LENGTH);
	strncat(link2, in_ipAddr, RESPONSE_LENGTH);
	strncat(link2, "/html/", RESPONSE_LENGTH);
	strncat(link2, PHOTO_FOLDER, RESPONSE_LENGTH);
	strncat(link2, in_folderName, RESPONSE_LENGTH);
	strncat(link2, TONGUE_LIP_FOLDER, RESPONSE_LENGTH);
	strncat(link2, in_tongueLipFileName, RESPONSE_LENGTH);
	if (in_category == ORIGIN) {
		strncpy(out_link, link, RESPONSE_LENGTH);
	}
	else if (g_photoCategory == ORIGIN_TONGUE) {
		strncpy(out_link, link, RESPONSE_LENGTH);
		strncat(out_link, ";", RESPONSE_LENGTH);
		strncat(out_link, link1, RESPONSE_LENGTH);
	}
	else if (g_photoCategory == ORIGIN_TONGUE_LIP) {
		strncpy(out_link, link, RESPONSE_LENGTH);
		strncat(out_link, ";", RESPONSE_LENGTH);
		strncat(out_link, link2, RESPONSE_LENGTH);
	}
	else if (g_photoCategory == ORIGIN_TONGUE_TONGUE_LIP) {
		strncpy(out_link, link, RESPONSE_LENGTH);
		strncat(out_link, ";", RESPONSE_LENGTH);
		strncat(out_link, link1, RESPONSE_LENGTH);
		strncat(out_link, ";", RESPONSE_LENGTH);
		strncat(out_link, link2, RESPONSE_LENGTH);
	}
}

