/*****************************************************************************/
/**
* @file TongueCtrl.h
* @brief ��̦����ͷ�ļ�
*
* ������̦������ز���
*
* @author wangwei
* @date 2016/06/04 wangwei ��ʼ�汾
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/

#ifndef TONGUECTRL_H_
#define TONGUECTRL_H_
#include <semaphore.h>
#include <sys/time.h>
#include <netinet/in.h>

#include "TongueCtrlParameter.h"
#include "TongueCtrlCommand.h"

#define CALIB 1

#define COMMAND_LENGTH		512				/*����Length*/
#define RSPNAME_LENGTH		10				/*��ӦLength*/
#define ERRNAME_LENGTH		50				/*����Length*/
#define UTC_LENGTH			20				/*UTC Time Length*/
#define TCPPACKETSIZE 		1024
#define MAX_LINE_LENGTH		80				/*TCP����Length*/
#define YUV_SIZE			7589376			/*YUV��С*/
#define GREY_SIZE			2592*1944		/*�Ҷ�ͼ�ߴ�*/
#define FULL_WIDTH			2592
#define FULL_HEIGHT			1944
#define DATETIME_LENGTH		20				/*����ʱ���ַ�������*/
#define FIFO_BUF_SIZE		50

#define RASPI_MAX_NUM		5

// Caliberation��
#define IMAGE_NUM  (6)					// ������
#define PAT_ROW    (7)  				// Pattern������
#define PAT_COL    (10) 				// Pattern������
#define PAT_SIZE   (PAT_ROW*PAT_COL)
#define ALL_POINTS (IMAGE_NUM*PAT_SIZE)
#define CHESS_SIZE (24.0)       /* [mm] */

#define UDPPACKETSIZE 1024
#define UDP_BROADCAST_PORT	5000

#define STORE_FILE	"/home/pi/parameters.cfg"
#define STORE_FILE2	"/home/pi/parameters2.cfg"
#define VERSION_FILE "/home/pi/Shell/.version"
#define PHOTO_CATE_FILE	"/home/pi/Shell/.photo_cate"

#define ETH_NAME		"wlan0"

#define IMAGE_FORMAT	".jpg"
#define VIDEO_FORMAT	".h264"
#define MP4_FORMAT		".mp4"
#define APACHE_FOLDER	"/var/www/html/"
#define RPI_FOLDER		"/var/www/html/media/"
#define RPI_PHOTO		"/var/www/html/media/camweb.jpg"
#define RPI_VIDEO		"/var/www/html/media/camweb.mp4"
#define PHOTO_FOLDER	"images/"
#define VIDEO_FOLDER	"videos/"
#define SHELL_FOLDER 	"/home/pi/Shell/"
#define SOUND_FOLDER	"Sound/"
#define LOG_FOLDER		"log/"
#define LOG_TAR_FILE	"log.tar.gz"
#define ORIGIN_FOLDER		"/0/"
#define TONGUE_FOLDER		"/1/"
#define TONGUE_LIP_FOLDER	"/2/"
#define ORI_DIR_NOSLASH		"0/"
#define TON_DIR_NOSLASH		"1/"
#define TON_LIP_DIR_NOSLASH	"2/"
#define TONGUE_FILE			"_1"
#define TONGUE_LIP_FILE		"_2"

#define VERSION			"V1.0"

#define RPI_PIC				"im"
#define RPI_VIDEO_START		"ca 1"
#define RPI_VIDEO_END		"ca 0"
#define RPI_WEB_IMAGE		"camweb.jpg"
#define RPI_WEB_VIDEO		"camweb.mp4"

#define VIDEO_TIMEOUT_TIME		20

// fifo for Image/Video
#define IMG_VID_FIFO	"/var/www/html/FIFO"
// fifo for Image/Video complete
#define ANS_FIFO		"/var/www/html/ANS_FIFO"

// ���տ�������
#define PHOTO_SHUTTER_SOUND			"photo_shutter.wav"
#define VIDEO_START_SHUTTER_SOUND 	"video_start_shutter.wav"
#define VIDEO_END_SHUTTER_SOUND		"video_end_shutter.wav"
#define PHOTO_VIDEO_FAILED_SOUND	"cam_failed.wav"

/// Simply capture for time specified
#define WAIT_METHOD_NONE           0
/// Cycle between capture and pause for times specified
#define WAIT_METHOD_TIMED          1
/// Switch between capture and pause on keypress
#define WAIT_METHOD_KEYPRESS       2

#define ROUND(real, ant) do { \
    if(real >= 0) \
        ant = (int)(real + 0.5);\
    else\
        ant = (int)(real - 0.5);\
	} while (0)

#define VIDEO_TIMEOUT_TIME		20
#define CHECK_GPIO_TIME			500
// Reset time
#define RESET_TIMEOUT			5000
// Audio
#define PCM_DEVICE "default"

/* ϵͳ״̬   */
typedef enum _SysStatus_En_tag {
	DUMMY_STATUS = 0,
	SYSTEM_READY,					/*READY״̬*/
	COMMAND_EXEC,					/*����ִ��״̬*/
	DEVICE_EXEC,
	SYSTEM_ERR,						/*����״̬*/
} CmdStatus_En;

/* ���״̬   */
typedef enum _CamStatus_En_tag {
	CAM_IDLE = 0,					/*�������*/
	CAM_PHOTO,						/*����*/
	CAM_VIDEO_START,				/*¼�࿪ʼ*/
	CAM_VIDEO_STOP,				/*¼�����*/
	DEV_CAM_VIDEO_START				/*�豸¼�࿪ʼ*/
} CamStatus_En;

/*ͼ��任 ����*/
typedef enum IMAGE_STEP_EN {
	IMAGE_DUMMY = 0,
	IMAGE_PICTURE,			//
	IMAGE_CUT,				//
	IMAGE_CALC_CALIB,
	IMAGE_CALIB,
	IMAGE_FILTER,
	IMAGE_BAYER2RGB,
	IMAGE_RGB2GRAY,
	IMAGE_SUBPIXEL,
	IMAGE_BINARY,
	IMAGE_EDGE,
	IMAGE_NOTCH,
	IMAGE_CENTER,
	IMAGE_HISTOIGRAM,
	IMAGE_LIGHT_ADJUST,
} IMAGE_STEP;

/*ͼ����Workspace*/
typedef enum IMAGE_WORKSPACE_EN {
	WS_DUMMY = 0,
	WS_PICTURE,
	WS_CUT,
	WS_CALIB,
	WS_FILTER,
	WS_RGB,
	WS_GRAY,
	WS_SUBPIXEL,
	WS_BINARY,
	WS_NUM
} IMAGE_WORKSPACE;

/*����Workspace*/
typedef enum COOR_WORKSPACE_EN {
	WS_COOR_DUMMY = 0,
	WS_COOR_EDGE,
	WS_COOR_NUM
} COOR_WORKSPACE;

/*��������*/
typedef enum IMAGE_DATA_TYPE_EN {
	DATA_ALL = 0,	// ȫ����
	DATA_IMAGE,		// ͼ������
	DATA_EDGE_COOR	// edge��������
} IMAGE_DATA_TYPE;

/*Device Status*/
typedef enum _CurrentPattern_En_tag {
	PHOTO = 0,
	MOVIE,
	MOVIEING,
} CurrentPattern_En;

/*Move state*/
typedef enum _MovieState_En_tag{
	START = 1,
	STOP,
} MovieState_En;


/*Move state*/
typedef enum _IllumeLight_En_tag {
	LIGHT_0 = 0,
	LIGHT_1,
	LIGHT_2,
	LIGHT_3
} IllumeLight_En;

/*Category of the PHOTO*/
typedef enum _PhotoCatagory_En_tag {
	ORIGIN = 0,					// ԭʼͼ��
	ORIGIN_TONGUE,				// ԭʼͼ�����ͷͼ��
	ORIGIN_TONGUE_LIP,			// ԭʼͼ�����ͷ�촽ͼ��
	ORIGIN_TONGUE_TONGUE_LIP	// ԭʼͼ�����ͷͼ�����ͷ�촽ͼ��
} PhotoCategory_En;

/*Category of Sound*/
typedef enum _PlaySound_En_tag {
	PICTURE_START_SOUND = 0,
	VIDEO_START_SOUND,
	VIDEO_END_SOUND,
	FAILED_SOUND
} PlaySound_En;
//#define IMG_COLOR
typedef struct _TongueCtrlParam_En_tag {
	int 	preview;
	int 	iResolutionW;
	int 	iResolutionH;
	float 	fRoiStartX;
	float 	fRoiStartY;
	float 	fRoiLenX;
	float 	fRoiLenY;
	int 	iCropX;
	int 	iCropY;
	int 	iCropWidth;
	int		iCropHeight;
	int		iSensorMode;
	int 	iRunType;
} TongueCtrlParam_En;

typedef struct raspi_param {
	int verbose;
	int width;
	int height;
	char fileName[256];
} Raspi_Param;

/** @brief RaspiVid��Ring Buffer */
typedef struct _RaspiVidBuffer_St_tag
{
	/** @brief  */
	unsigned char buffer[YUV_SIZE][RASPI_MAX_NUM];
	/** @brief  Buffer�м�������� */
	unsigned int ulSize;
	/** @brief Buffer��Writeλ�� */
	unsigned int ulWriteIndex;
	/** @brief Buffer��Readλ�� */
	unsigned int ulReadIndex;
} RaspiVidBuffer_St;

/* ��������  */
typedef struct _ReceptionCond_St_tag {
	unsigned int	uiCurSysCmdStatus;		/*ϵͳ״̬ */
	unsigned int 	uiCurDevStatus;			/*�豸״̬*/
} ReceptionCond_St;

/* Ӧ����Ϣ�ַ��� */
typedef struct _CmdRes_St_tag {
	unsigned int	uiCmdResId;						/*Ӧ��No.*/
	char			aCmdResName[RSPNAME_LENGTH];	/*Ӧ������*/
} CmdRes_St;

/* ͼ���д��� */
typedef struct _Parallel_St_tag {
	int		iStartPos;
	int 	iEndPos;
} Parallel_St;

/* �����ڴ����ݽṹ */
struct shared_use_st {
	int written;//��Ϊһ����־����0����ʾ�ɶ���0��ʾ��д
	int read;
	int iPhotoStart;
	int iPhotoEnd;
	int iVideoStart;
	int iVideoStop;
};

/* UDP */
typedef struct _UdpComm_St_tag {
	int socket;
	struct sockaddr_in sin;
} UdpComm_St;

extern UdpComm_St g_udpComm;
// accept socket
extern int g_clientTcpIpCommfd;
extern int g_clientBTIpcCommfd;
typedef enum {
	CT_NONE,
	CT_FROM_TCPIP,
	CT_FROM_UDP,
	CT_FROM_BTIPC
} CLIENT_TYPE;

extern CLIENT_TYPE g_clientFlag;
// ��������
extern char g_sInputCommand[COMMAND_LENGTH];
// Network�ź���
extern sem_t	g_semNetwork;
// UDP�ź���
extern sem_t   g_semUdpComm;
// ����ִ���ź���
extern sem_t	g_semCmdRun;
// ����ִ��Result�ź���
extern sem_t	g_semCmdResult;
// �̳߳���������� �ź���
extern sem_t 	g_semThreadPoolTask;
// SysMonitor�ź���
extern sem_t   g_semSysMonitor;
// Log����ź���
extern sem_t	g_semLogOutput;
// �����ź���
extern sem_t   g_semCamera;
// ��Ƶ�ź���
// ��Ϣ�����ź���
extern sem_t   g_semMsgQueue;

extern sem_t   g_semCamVid;

extern sem_t   g_semBlueTooth;
// ��������
extern sem_t   g_semPlaySound;

// ���״̬
extern CamStatus_En g_camStatus;

extern NumericType	g_aSystemParams[SYS_PARAM_NUM];

extern unsigned int g_arrY[256];

extern ReceptionCond_St		g_stReceptionCond;
extern CommonSysParam_St 	g_commonSysParam;
extern ChamberCtrlParam_St	g_chamberCtrlParam;
extern ImageCtrlParam_St 	g_imageCtrlParam;

// ͼ����
extern IMAGE_STEP g_imageStep;
// Load���
extern LoadInfo_St g_loadInfo;
// Save���
extern SaveInfo_St g_saveInfo;


// Raspivid ring buffer
extern RaspiVidBuffer_St g_raspiVidBuffer;

// Calibration InitMaps Flag
extern int g_bInitMaps;

// ͼ�����������
extern int g_calibArray[1920*1080];

// Timer
extern struct timeval g_timerBegin, g_timerEnd, g_timerConvBegin,
				g_timerConvEnd, g_timerImageBegin, g_timerImageEnd,
				g_timerBinaryBegin, g_timerBinaryEnd;
extern void YUV420_C_RGB( unsigned char* pYUV, unsigned char* pRGB, int height, int width);
extern void YUV420_C_GREY( unsigned char* pYUV, unsigned char* pGrey, int height, int width);
extern void YUV420_C_RGB1( unsigned char* pYUV, unsigned char* pRGB, int height, int width);

extern bool CutNetDelimiter(char* io_sInput);

extern struct shared_use_st *g_shared;
extern int g_shmid;
extern void *g_shm;

// Wifi status
extern int g_iWifiStatus;
// ����������
extern int g_iBrightness;
// upgrade�����Ƿ�������
extern int g_iUpgradeProc;

// Photo����
extern PhotoCategory_En g_photoCategory;

extern CurrentPattern_En g_CurrentPattern;
extern MovieState_En g_MovieState;
// Sound
extern PlaySound_En g_iPlaySound;

#endif /* TONGUECTRL_H_ */
