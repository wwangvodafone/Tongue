/*****************************************************************************/
/**
* @file TongueCtrl.h
* @brief 舌苔控制头文件
*
* 定义舌苔控制相关参数
*
* @author wangwei
* @date 2016/06/04 wangwei 初始版本
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

#define COMMAND_LENGTH		512				/*命令Length*/
#define RSPNAME_LENGTH		10				/*响应Length*/
#define ERRNAME_LENGTH		50				/*错误Length*/
#define UTC_LENGTH			20				/*UTC Time Length*/
#define TCPPACKETSIZE 		1024
#define MAX_LINE_LENGTH		80				/*TCP最大包Length*/
#define YUV_SIZE			7589376			/*YUV大小*/
#define GREY_SIZE			2592*1944		/*灰度图尺寸*/
#define FULL_WIDTH			2592
#define FULL_HEIGHT			1944
#define DATETIME_LENGTH		20				/*日期时间字符串长度*/
#define FIFO_BUF_SIZE		50

#define RASPI_MAX_NUM		5

// Caliberation用
#define IMAGE_NUM  (6)					// 画像数
#define PAT_ROW    (7)  				// Pattern的行数
#define PAT_COL    (10) 				// Pattern的列数
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

// 拍照快门声音
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

/* 系统状态   */
typedef enum _SysStatus_En_tag {
	DUMMY_STATUS = 0,
	SYSTEM_READY,					/*READY状态*/
	COMMAND_EXEC,					/*命令执行状态*/
	DEVICE_EXEC,
	SYSTEM_ERR,						/*错误状态*/
} CmdStatus_En;

/* 相机状态   */
typedef enum _CamStatus_En_tag {
	CAM_IDLE = 0,					/*相机空闲*/
	CAM_PHOTO,						/*拍照*/
	CAM_VIDEO_START,				/*录相开始*/
	CAM_VIDEO_STOP,				/*录相结束*/
	DEV_CAM_VIDEO_START				/*设备录相开始*/
} CamStatus_En;

/*图像变换 步骤*/
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

/*图像处理Workspace*/
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

/*坐标Workspace*/
typedef enum COOR_WORKSPACE_EN {
	WS_COOR_DUMMY = 0,
	WS_COOR_EDGE,
	WS_COOR_NUM
} COOR_WORKSPACE;

/*数据种类*/
typedef enum IMAGE_DATA_TYPE_EN {
	DATA_ALL = 0,	// 全数据
	DATA_IMAGE,		// 图像数据
	DATA_EDGE_COOR	// edge坐标数据
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
	ORIGIN = 0,					// 原始图像
	ORIGIN_TONGUE,				// 原始图像和舌头图像
	ORIGIN_TONGUE_LIP,			// 原始图像和舌头嘴唇图像
	ORIGIN_TONGUE_TONGUE_LIP	// 原始图像和舌头图像和舌头嘴唇图像
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

/** @brief RaspiVid用Ring Buffer */
typedef struct _RaspiVidBuffer_St_tag
{
	/** @brief  */
	unsigned char buffer[YUV_SIZE][RASPI_MAX_NUM];
	/** @brief  Buffer中加入的项数 */
	unsigned int ulSize;
	/** @brief Buffer的Write位置 */
	unsigned int ulWriteIndex;
	/** @brief Buffer的Read位置 */
	unsigned int ulReadIndex;
} RaspiVidBuffer_St;

/* 处理条件  */
typedef struct _ReceptionCond_St_tag {
	unsigned int	uiCurSysCmdStatus;		/*系统状态 */
	unsigned int 	uiCurDevStatus;			/*设备状态*/
} ReceptionCond_St;

/* 应答消息字符串 */
typedef struct _CmdRes_St_tag {
	unsigned int	uiCmdResId;						/*应答No.*/
	char			aCmdResName[RSPNAME_LENGTH];	/*应答名称*/
} CmdRes_St;

/* 图像并行处理 */
typedef struct _Parallel_St_tag {
	int		iStartPos;
	int 	iEndPos;
} Parallel_St;

/* 共享内存数据结构 */
struct shared_use_st {
	int written;//作为一个标志，非0：表示可读，0表示可写
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
// 输入命令
extern char g_sInputCommand[COMMAND_LENGTH];
// Network信号量
extern sem_t	g_semNetwork;
// UDP信号量
extern sem_t   g_semUdpComm;
// 命令执行信号量
extern sem_t	g_semCmdRun;
// 命令执行Result信号量
extern sem_t	g_semCmdResult;
// 线程池任务处理完成 信号量
extern sem_t 	g_semThreadPoolTask;
// SysMonitor信号量
extern sem_t   g_semSysMonitor;
// Log输出信号量
extern sem_t	g_semLogOutput;
// 拍照信号量
extern sem_t   g_semCamera;
// 视频信号量
// 消息队列信号量
extern sem_t   g_semMsgQueue;

extern sem_t   g_semCamVid;

extern sem_t   g_semBlueTooth;
// 声音播放
extern sem_t   g_semPlaySound;

// 相机状态
extern CamStatus_En g_camStatus;

extern NumericType	g_aSystemParams[SYS_PARAM_NUM];

extern unsigned int g_arrY[256];

extern ReceptionCond_St		g_stReceptionCond;
extern CommonSysParam_St 	g_commonSysParam;
extern ChamberCtrlParam_St	g_chamberCtrlParam;
extern ImageCtrlParam_St 	g_imageCtrlParam;

// 图像处理
extern IMAGE_STEP g_imageStep;
// Load忣曬
extern LoadInfo_St g_loadInfo;
// Save忣曬
extern SaveInfo_St g_saveInfo;


// Raspivid ring buffer
extern RaspiVidBuffer_St g_raspiVidBuffer;

// Calibration InitMaps Flag
extern int g_bInitMaps;

// 图像的修正数组
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
// 照明灯亮度
extern int g_iBrightness;
// upgrade进程是否已启动
extern int g_iUpgradeProc;

// Photo种类
extern PhotoCategory_En g_photoCategory;

extern CurrentPattern_En g_CurrentPattern;
extern MovieState_En g_MovieState;
// Sound
extern PlaySound_En g_iPlaySound;

#endif /* TONGUECTRL_H_ */
