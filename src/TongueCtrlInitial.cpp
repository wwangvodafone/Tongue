/*****************************************************************************/
/**
* @file TongueCtrlInitial.cpp
* @brief ��̦���Ƴ��ڻ�
*
* ��̦���Ƴ��ڻ�
*
* @author wangwei
* @date 2016/06/06 wangwei ��ʼ�汾
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "TongueCtrlCommon.h"
#include "TongueCtrlParameter.h"
#include "TongueCtrl.h"
#include "TongueCtrlLogThd.h"
#include "TongueCtrlGPIO.h"
#include "TongueCtrlErrProc.h"
#include "TongueCtrlUtility.h"

/*���ܴ�������*/
ReceptionCond_St		g_stReceptionCond;

CommonSysParam_St g_commonSysParam;
ChamberCtrlParam_St	g_chamberCtrlParam;
ImageCtrlParam_St g_imageCtrlParam;

// Load���
LoadInfo_St g_loadInfo;
// Save���
SaveInfo_St g_saveInfo;

// Network�ź���
sem_t	g_semNetwork;
// UDP�ź���
sem_t   g_semUdpComm;
// ����ִ���ź���
sem_t	g_semCmdRun;
// ����ִ��Result�ź���
sem_t	g_semCmdResult;
// �̳߳�Task���������ź���
sem_t 	g_semThreadPoolTask;
// SysMonitor�ź���
sem_t   g_semSysMonitor;
// �����ź���
sem_t   g_semCamera;
// ��Ƶ�ź���
sem_t   g_semCamVid;
// ��Ϣ�����ź���
sem_t   g_semMsgQueue;
// BlueTooth�ź���
sem_t   g_semBlueTooth;
// ��������
sem_t   g_semPlaySound;
// Calibration InitMap Flag
int g_bInitMaps;
// ����������
int g_iBrightness;
// upgrade�����Ƿ�������
int g_iUpgradeProc;
// Photo����
PhotoCategory_En g_photoCategory;
// ���״̬
CamStatus_En g_camStatus;
// Sound
PlaySound_En g_iPlaySound;

// ͼ����
IMAGE_STEP g_imageStep;

CurrentPattern_En g_CurrentPattern;
MovieState_En g_MovieState;

struct shared_use_st *g_shared = NULL;
int g_shmid;
void *g_shm = NULL;
// Wifi status
int g_iWifiStatus;

// ͼ����������
int g_calibArray[1920*1080];

unsigned int g_arrY[256];

struct timeval g_timerBegin, g_timerEnd, g_timerConvBegin,
				g_timerConvEnd, g_timerImageBegin, g_timerImageEnd,
				g_timerBinaryBegin, g_timerBinaryEnd;

static void InitCommonSysParameters(void);
static void InitChamberCtrlParameters(void);
static void InitImageCtrlParameters(void);
static void InitResolution(TongueCtrlParam_En TongueCtrlParam);
static void SetSystemParamFromCommonSysParam(void);
static void SetSystemParamFromChamberCtrlParam(void);
static void SetSystemParamFromImageCtrlParam(void);
static int  InitSemaphore(void);
static void InitWorkspace(void);
//static void InitYArray(void);
static void InitMat(void);
static void InitParamsFromSysParams(void);
//static void GetCameraParameters(void);
static void InitTongueParam(TongueCtrlParam_En *TongueCtrlParam);
static void InitCamera(void);
static void InitFifo(void);

/**
 * @brief ���õ�ϵͳ��������
 *
 * ���õ�ϵͳ��������
 *
 * @param
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
void InitCommonSysParameters(void)
{
	g_commonSysParam.uiPortMMI = 10000;
	g_commonSysParam.uiPortMaster = 10001;
	g_commonSysParam.uiPortTool = 10002;

	SetSystemParamFromCommonSysParam();
}


/**
 * @brief Chamber������ʼ��
 *
 * Chamber������ʼ��
 *
 * @param
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
void InitChamberCtrlParameters()
{
	for (int i = 0; i < CHAMBER_NUM; i++) {
		g_chamberCtrlParam.fDuty[i] = 50.0f;
	}

	SetSystemParamFromChamberCtrlParam();
}

/**
 * @brief ͼ����Ʋ����ĳ�ʼ������
 *
 * ͼ����Ʋ����ĳ�ʼ������
 *
 * @param
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
void InitImageCtrlParameters(void)
{
	g_imageCtrlParam.iResolutionX = 2592;
	g_imageCtrlParam.iResolutionY = 1944;
	g_imageCtrlParam.fRoiStartX = 0.0f;
	g_imageCtrlParam.fRoiStartY = 0.0f;
	g_imageCtrlParam.fRoiLenX = 1.0f;
	g_imageCtrlParam.fRoiLenY = 1.0f;
	g_imageCtrlParam.iCropX = 0;
	g_imageCtrlParam.iCropY = 0;
	g_imageCtrlParam.iCropWidth = 0;
	g_imageCtrlParam.iCropHeight = 0;
	g_imageCtrlParam.iPreview = 1;
	g_imageCtrlParam.iSensorMode = 2;
	g_imageCtrlParam.iRunType = 0;

	g_imageCtrlParam.iStartX = 1;
	g_imageCtrlParam.iStartY = 1;
	g_imageCtrlParam.iEndX = 1;
	g_imageCtrlParam.iEndY = 1;
	g_imageCtrlParam.iDivNumber = 2;
	g_imageCtrlParam.iBinaryThreadValue = 120;
	g_imageCtrlParam.iInterval = 10;
	g_imageCtrlParam.iExclusionNumber = 30;
	g_imageCtrlParam.iLightAdjustThreshValue = 120;
	g_imageCtrlParam.iXRef = 1296;
	g_imageCtrlParam.iYRef = 2890;
	g_imageCtrlParam.iRadiusAccuracy = 50;
	g_imageCtrlParam.iPresenceThreshValue = 120;
	g_imageCtrlParam.fXdissolution = 77.8f;
	g_imageCtrlParam.fYdissolution = 77.8f;
	g_imageCtrlParam.fCalibGain = 0.0f;
	g_imageCtrlParam.fCalibOffset = 0.0f;

	SetSystemParamFromImageCtrlParam();

}


/**
 * @brief Resolution��ʼ������
 *
 * Resolution��ʼ������
 *
 * @param
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
void InitResolution(TongueCtrlParam_En TongueCtrlParam)
{
	g_imageCtrlParam.iResolutionX = TongueCtrlParam.iResolutionW;
	g_imageCtrlParam.iResolutionY = TongueCtrlParam.iResolutionH;
}

/**
 * @brief �ӹ��õ�ϵͳ�������趨ϵͳ����
 *
 * �ӹ��õ�ϵͳ�������趨ϵͳ����
 *
 * @param
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
void SetSystemParamFromCommonSysParam(void)
{
	g_aSystemParams[PORT_MMI].iValue = g_commonSysParam.uiPortMMI;
	g_aSystemParams[PORT_MASTER].iValue = g_commonSysParam.uiPortMaster;
	g_aSystemParams[PORT_TOOL].iValue = g_commonSysParam.uiPortTool;
}

/**
 * @brief ��ChamberCtrl�������趨ϵͳ����
 *
 * ��ChamberCtrl�������趨ϵͳ����
 *
 * @param
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
void SetSystemParamFromChamberCtrlParam()
{
	for (int i = 0; i < CHAMBER_NUM; i++) {
		g_aSystemParams[DUTY_1 + i].fValue = g_chamberCtrlParam.fDuty[i];
	}
}

/**
 * @brief ��ͼ����Ʋ���������ϵͳ����
 *
 * ��ͼ����Ʋ���������ϵͳ����
 *
 * @param
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
void SetSystemParamFromImageCtrlParam(void)
{
	g_aSystemParams[RESOLUTION_W].iValue = g_imageCtrlParam.iResolutionX;
	g_aSystemParams[RESOLUTION_H].iValue = g_imageCtrlParam.iResolutionY;
	g_aSystemParams[ROI_START_X].fValue = g_imageCtrlParam.fRoiLenX;
	g_aSystemParams[ROI_START_Y].fValue = g_imageCtrlParam.fRoiLenY;
	g_aSystemParams[ROI_LEN_X].fValue = g_imageCtrlParam.fRoiLenX;
	g_aSystemParams[ROI_LEN_Y].fValue = g_imageCtrlParam.fRoiLenY;
	g_aSystemParams[CROP_X].iValue = g_imageCtrlParam.iCropX;
	g_aSystemParams[CROP_Y].iValue = g_imageCtrlParam.iCropY;
	g_aSystemParams[CROP_WIDTH].iValue = g_imageCtrlParam.iCropWidth;
	g_aSystemParams[CROP_HEIGHT].iValue = g_imageCtrlParam.iCropHeight;
	g_aSystemParams[PREVIEW].iValue = g_imageCtrlParam.iPreview;
	g_aSystemParams[SENSOR_MODE].iValue = g_imageCtrlParam.iSensorMode;
	g_aSystemParams[RUN_TYPE].iValue = g_imageCtrlParam.iRunType;


	g_aSystemParams[START_X].iValue = g_imageCtrlParam.iStartX;
	g_aSystemParams[START_Y].iValue = g_imageCtrlParam.iStartY;
	g_aSystemParams[END_X].iValue = g_imageCtrlParam.iEndX;
	g_aSystemParams[END_Y].iValue = g_imageCtrlParam.iEndY;
	g_aSystemParams[DIV_NUMBER].iValue = g_imageCtrlParam.iDivNumber;
	g_aSystemParams[BINARY_THRESH_VALUE].iValue = g_imageCtrlParam.iBinaryThreadValue;
	g_aSystemParams[EDGE_INTERVAL].iValue = g_imageCtrlParam.iInterval;
	g_aSystemParams[NOTCH_EXCLUSION_NUMBER].iValue = g_imageCtrlParam.iExclusionNumber;
	g_aSystemParams[LIGHTADJUST_THRESH_VALUE].iValue = g_imageCtrlParam.iLightAdjustThreshValue;
	g_aSystemParams[X_REF].iValue = g_imageCtrlParam.iXRef;
	g_aSystemParams[Y_REF].iValue = g_imageCtrlParam.iYRef;
	g_aSystemParams[RADIUS_ACCURACY].iValue = g_imageCtrlParam.iRadiusAccuracy;
	g_aSystemParams[PRESENCE_THRESH_VALUE].iValue = g_imageCtrlParam.iPresenceThreshValue;
	g_aSystemParams[X_DISSOLUTION].fValue = g_imageCtrlParam.fXdissolution;
	g_aSystemParams[Y_DISSOLUTION].fValue = g_imageCtrlParam.fYdissolution;
	g_aSystemParams[CALIB_GAIN].fValue = g_imageCtrlParam.fCalibGain;
	g_aSystemParams[CALIB_OFFSET].fValue = g_imageCtrlParam.fCalibOffset;

}

/**
 * @brief �ź����ĳ�ʼ��
 *
 * �ź����ĳ�ʼ��
 *
 * @param
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
int InitSemaphore(void)
{
	int iRetSem = 0;
	// �Z�}�t�H������
	iRetSem = sem_init(&g_semNetwork, 0, 0);
	iRetSem = sem_init(&g_semUdpComm, 0, 0);
	iRetSem = sem_init(&g_semCmdRun, 0, 0);
	iRetSem = sem_init(&g_semCmdResult, 0, 0);
	iRetSem = sem_init(&g_semThreadPoolTask, 0, 0);
	iRetSem = sem_init(&g_semSysMonitor, 0, 0);
	iRetSem = sem_init(&g_semBlueTooth, 0, 0);
	iRetSem = sem_init(&g_semCamera, 0, 0);
	iRetSem = sem_init(&g_semCamVid, 0, 0);
	iRetSem = sem_init(&g_semMsgQueue, 0, 0);
	iRetSem = sem_init(&g_semPlaySound, 0, 0);

	return iRetSem;
}

/**
 * @brief ��ϵͳ��������ʼ��ϵͳ����
 *
 * ��ϵͳ��������ʼ��ϵͳ����
 *
 * @param
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
void InitParamsFromSysParams(void)
{
	// ϵͳͨ�ò���
	g_commonSysParam.uiPortMMI = g_aSystemParams[PORT_MMI].iValue;
	g_commonSysParam.uiPortMaster = g_aSystemParams[PORT_MASTER].iValue;
	g_commonSysParam.uiPortTool = g_aSystemParams[PORT_TOOL].iValue;

	for (int i = 0; i < CHAMBER_NUM; i++) {
		g_chamberCtrlParam.fDuty[i] = g_aSystemParams[DUTY_1 + i].fValue;
	}
	// ������Ʋ���
	g_imageCtrlParam.iStartX = g_aSystemParams[START_X].iValue;
	g_imageCtrlParam.iStartY = g_aSystemParams[START_Y].iValue;
	g_imageCtrlParam.iEndX = g_aSystemParams[END_X].iValue;
	g_imageCtrlParam.iEndY = g_aSystemParams[END_Y].iValue;
	g_imageCtrlParam.iDivNumber = g_aSystemParams[DIV_NUMBER].iValue;
	g_imageCtrlParam.iBinaryThreadValue = g_aSystemParams[BINARY_THRESH_VALUE].iValue;
	g_imageCtrlParam.iInterval = g_aSystemParams[EDGE_INTERVAL].iValue;
	g_imageCtrlParam.iExclusionNumber = g_aSystemParams[NOTCH_EXCLUSION_NUMBER].iValue;
	g_imageCtrlParam.iLightAdjustThreshValue = g_aSystemParams[LIGHTADJUST_THRESH_VALUE].iValue;
	g_imageCtrlParam.iXRef = g_aSystemParams[X_REF].iValue;
	g_imageCtrlParam.iYRef = g_aSystemParams[Y_REF].iValue;
	g_imageCtrlParam.iRadiusAccuracy = g_aSystemParams[RADIUS_ACCURACY].iValue;
	g_imageCtrlParam.iPresenceThreshValue = g_aSystemParams[PRESENCE_THRESH_VALUE].iValue;
	g_imageCtrlParam.fXdissolution = g_aSystemParams[X_DISSOLUTION].fValue;
	g_imageCtrlParam.fYdissolution = g_aSystemParams[Y_DISSOLUTION].fValue;
	g_imageCtrlParam.fCalibGain = g_aSystemParams[CALIB_GAIN].fValue;
	g_imageCtrlParam.fCalibOffset = g_aSystemParams[CALIB_OFFSET].fValue;

;
}

/**
 * @brief �����ĳ�ʼ��
 *
 * �����ĳ�ʼ��
 *
 * @param
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
void InitParameter(TongueCtrlParam_En* TongueCtrlParam)
{
	int iReadParam = -1;
	char sPhotoCate[PHOTO_CATE_LENGTH];

	// Change hostname
//	SetHostName();
	// Set device folder for videos and images
//	SetDeviceFolder();
	// �ź�����ʼ��
	InitSemaphore();
	// GPIO��ʼ��
	InitGPIO();

	// �������
//	iReadParam = ReadParametersAll(SYS_PARAM_NUM, g_aSystemParams);
	if (iReadParam == 0) {
		// ��ϵͳ��������ʼ������
		InitParamsFromSysParams();
	}
	else {
		// ͨ��ϵͳ������ʼ������
		InitCommonSysParameters();

		InitChamberCtrlParameters();
		// ͼ����Ʋ����ĳ�ʼ������
		InitImageCtrlParameters();
	}

	// Resolution�ĳ�ʼ��
//	InitResolution(TongueCtrlParam);
	// ϵͳ״̬ ����Ϊ��READY
	g_stReceptionCond.uiCurSysCmdStatus = SYSTEM_READY;
	// �豸״̬����Ϊ��DUMMY
	g_stReceptionCond.uiCurDevStatus = DUMMY_STATUS;

	// workspace��ʼ��
	InitWorkspace();

	// Mat��ʼ��
	InitMat();
	// ͼ�����Step
	g_imageStep = IMAGE_DUMMY;
	// Calibration��InitMaps Flag
	g_bInitMaps = 0;

	memset(g_calibArray, -1, sizeof(g_calibArray));

	// TongueCtrlParam��ʼ��
	InitTongueParam(TongueCtrlParam);

	// �����ʼ��
	InitCamera();
	// wifi״̬��ʼ��
	g_iWifiStatus = WIFI_HOTSPOT;
	// ����������
	g_iBrightness = 3;
	// ��������δ����
	g_iUpgradeProc = 0;
	// ��Ƭ��������
	g_photoCategory = ORIGIN;
	if (GetPhotoCategory(sPhotoCate) == ERR_CMD_SUCCESS) {
//		g_photoCategory = (PhotoCategory_En)atoi(sPhotoCate);
	}

	// initialize fifo
	InitFifo();

	g_CurrentPattern = PHOTO;
	g_MovieState = STOP;
	g_iPlaySound = PICTURE_START_SOUND;

#ifdef CALIB
	// ����Calibration �������
//	GetCameraParameter2();
#endif
}

/**
 * @brief workspace��ʼ��
 *
 * workspace��ʼ��
 *
 * @param
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
void InitWorkspace(void)
{
}


/**
 * @brief TongueCtrl������ʼ��
 *
 * TongueCtrl������ʼ��
 *
 * @param TongueCtrlParam TongueCtrl����
 * @author wangwei
 * @date 2016/06/12 wangwei ��ʼ�汾
 */
void InitTongueParam(TongueCtrlParam_En *TongueCtrlParam)
{
	TongueCtrlParam->iResolutionW = g_imageCtrlParam.iResolutionX;
	TongueCtrlParam->iResolutionH = g_imageCtrlParam.iResolutionY;
	TongueCtrlParam->fRoiStartX = g_imageCtrlParam.fRoiStartX;
	TongueCtrlParam->fRoiStartY = g_imageCtrlParam.fRoiStartY;
	TongueCtrlParam->fRoiLenX = g_imageCtrlParam.fRoiLenX;
	TongueCtrlParam->fRoiLenY = g_imageCtrlParam.fRoiLenY;
	TongueCtrlParam->iCropX = g_imageCtrlParam.iCropX;
	TongueCtrlParam->iCropY = g_imageCtrlParam.iCropY;
	TongueCtrlParam->iCropWidth = g_imageCtrlParam.iCropWidth;
	TongueCtrlParam->iCropHeight = g_imageCtrlParam.iCropHeight;
	TongueCtrlParam->preview = g_imageCtrlParam.iPreview;
	TongueCtrlParam->iSensorMode = g_imageCtrlParam.iSensorMode;
	TongueCtrlParam->iRunType = g_imageCtrlParam.iRunType;
}

/**
 * @brief Mat��ʼ��
 *
 * Mat��ʼ��
 *
 * @param
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
void InitMat(void)
{
//	int w = g_imageCtrlParam.iResolutionX;
//	int h = g_imageCtrlParam.iResolutionY;
//	g_dstImage.create(Size(w, h), CV_8UC1);
//	g_grayImage.create(Size(w, h), CV_8UC1);
//	g_rgbImage.create(Size(w, h), CV_8UC3);
//	g_workspaceID[0].idImage.create(Size(w, h), CV_8UC3);
//	g_workspaceID[WS_RGB].idImage.zeros(Size(w, h), CV_8UC3);



}

/**
 * @brief �����ʼ��
 *
 * �����ʼ��
 *
 * @param
 * @author wangwei
 * @date 2016/06/17 wangwei ��ʼ�汾
 */
void InitCamera(void)
{
//	   CamStillFunction();
//	CamVidFunction();
//	   CamVideoFunction();

//	CamOperFunction();
	g_camStatus = CAM_IDLE;
}


/**
 * @brief FIFO��ʼ��
 *
 * �����ʼ��
 *
 * @param
 * @author wangwei
 * @date 2016/12/21 wangwei ��ʼ�汾
 */
void InitFifo(void)
{
	int res = 0;

	if (access(ANS_FIFO, F_OK) == -1) {
		res = mkfifo(ANS_FIFO, 0777);
		if (res != 0) {
			g_TongueCtrlLog.E("Can not create Answer FIFO.");
		}
	}

	return;
}
