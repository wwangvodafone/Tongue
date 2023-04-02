/*****************************************************************************/
/**
* @file TongueCtrlCommand.h
* @brief 舌苔控制命令头文件
*
* 定义舌苔控制命令定义
*
* @author wangwei
* @date 2016/06/04 wangwei 初始版本
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/
#ifndef TONGUECTRLCOMMAND_H_
#define TONGUECTRLCOMMAND_H_

#include <stdint.h>

#include "TongueCtrlParameter.h"

#define NULLCHAR	""
#define BLANKCHAR	" "
#define NOTHING		"--"
#define DATE_LENGHT			14				/*日期Length YYYYMMDDHHmmSS*/
#define DATE_HYPHEN_LENGHT	19				/*日期Length YYYY-MM-DD-HH-mm-SS*/
#define DATE_DIGIT			6				/*日期位数*/
/** @brief 应答名称Length */
#define RSPNAME_LENGTH		10
/** @brief error名称Length */
#define ERRNAME_LENGTH		50
/** @brief Command长度 */
//#define COMMAND_LENGTH		64
/** @brief Response长度 */
#define RESPONSE_LENGTH		1024
/** @brief IP长度 */
#define IP_LENGTH			20
/** @brief 空格 */
#define BLANK 		0x20
#define CR			0x0D
#define LF			0x0A
//#define CS			0x00
/** @brief Command名称Length */
#define CMDNAME_LENGTH		20

/** @brief IP地址Length*/
#define IP_LENGTH			20
/** @brief 用户名Length*/
#define NAME_LENGTH			50
/** @brief 文件名Length*/
#define FILENAME_LENGTH		1024
/** @brief Link Length*/
#define LINK_LENGTH			10240
/** @brief 文件的最大数量*/
#define MAX_FILENUM			50
/** @brief 无需错误清除 */
#define NO_ERR_CLEAR		0
/** @brief 需要错误清除 */
#define NEED_ERR_CLEAR		1
/** @brief HostName */
#define HOSTNAME_LENGTH		100
/** @brief SSIDName */
#define SSID_LENGTH			50
/** @brief Password */
#define PASS_LENGTH			50
/** @brief Version */
#define VERSION_LENGTH		10
/** @brief Photo Category*/
#define PHOTO_CATE_LENGTH	5

#define PARAM_LENGTH		100				/*参数Length*/
#define PARAM_MAX			20				/*命令参数最大数*/
#define FILE_MAX_LENGTH		255				/*文件Max Length*/

#define HYPHEN		'-'

#define MAX_YEAR					10000		// 最大年
#define MIN_YEAR					999			// 最小年
#define BIG_MONTH_DAY				31			// 日数(1,3,5,7,8,10,12)
#define SMALL_MONTH_DAY				30			// 日数(4,6,9,11)

// Chamber号范围
#define MAX_CHAMBER_NO				16
#define MIN_CHAMBER_NO				1
// Duty范围
#define MAX_DUTY					100.0f
#define MIN_DUTY					0.1f
// Data Type范围
#define MAX_DATA_TYPE				2
#define MIN_DATA_TYPE				0
// Worksapce Id范围
#define MAX_WORKSPACE_ID			8
#define MIN_WORKSPACE_ID			1
// Sub Pixel范围
#define MAX_SUBPIXEL				5
#define MIN_SUBPIXEL				1
// 二值化处理阈值
#define MAX_BIN_THRESH				255
#define MIN_BIN_THRESH				0
// Edge坐标抽出interval范围
#define MAX_EDGE_INTERVAL			20
#define MIN_EDGE_INTERVAL			0
// NOTCH除外点数范围
#define MAX_EXCLU_NUM				50
#define MIN_EXCLU_NUM				0

#define FEB_NO_LEAP					28			// 2月的日数（非闰年）
#define FEB_LEAP					29			// 2月的日数（闰年）

#define LIGHT_ON					"ON"		// LIGHT ON
#define LIGHT_OFF					"OFF"		// LIGHT OFF

/*
 * 命令编号
 */
typedef enum COMMAND_NUM_EN {
	CMD_DUMMY = 0,			// DUMMY
	CMD_SHUTDOWN,			// SHUTDOWN
	CMD_RESET,				// RESET
	CMD_REBOOT,				// REBOOT
	CMD_LIGHT,				// LIGHT
	CMD_PICTURE,			// PICTURE
	CMD_MOVIE,				// MOVIE
	CMD_LOAD,				// LOAD
	CMD_SAVE,				// SAVE
	CMD_UPLOAD,				// UPLOAD
	CMD_SYNC,				// SYNC
	CMD_DELETE,				// DELETE
	CMD_SET,				// SET
	CMD_RQ,					// RQ
	CMD_ERRRST,				// ERRRST
	CMD_IPCHG,				// IPCHG
	CMD_GETLOG,				// GETLOG
	CMD_UPGRADE,			// UPGRADE
	CMD_SEARCH,				// SEARCH
	CMD_SETWIFI,			// SETWIFI
	CMD_OBTINFO,			// OBTINFO
	CMD_DEVPIC,				// DEVPIC
	CMD_DEVMOVIE,			// DEVMOVIE
	CMD_PHOTOCATG,			// PHOTOCATE
	CMD_RPMSTART,			// RPMSTART
	CMD_NUM
} COMMAND_NUM;

/** @brief 命令分类 */
typedef enum _CmdType_En_tag_
{
	/** @brief DUMMY命令*/
	DUMMY_CMD = 0,
	/** @brief 动作命令*/
	ACTION_CMD,
	/** @brief 请求命令*/
	REQUEST_CMD,
	/** @brief 其他命令*/
	OTHER_CMD,
} CmdType_En;


/** @brief Month*/
enum {
	JANUARARY = 1,
	FEBRUARY,
	MARCH,
	APRIL,
	MAY,
	JUNE,
	JULY,
	AUGUST,
	SEPTEMBER,
	OCTOBER,
	NOVEMBER,
	DECEMBER
};

/** @brief Enable/Disable */
enum {
	/** @brief 无效*/
	ENABLE_OFF = 0,
	/** @brief 有泖*/
	ENABLE_ON,
	/** @brief 其他*/
	ENABLE_DONE
};

/* 应答类型  */
typedef enum _RetMessage_En_tag {
	DUMMY_RET = 0,
	RDY_RET,							/*正常应答*/
	ERR_RET,							/*异常应答*/
	REP_RET,							/*请求命令的应答*/
} RetMessage_En;


/** @brief 命令的Sequence分类 */
enum {
	/** @brief 错误类型*/
	ERROR_CMD = -1,
	/** @brief Dummy*/
	DUM_CMD,
	/** @brief 请求命令*/
	RQ_CMD,
	/** @brief 设定命令*/
	SET_CMD,
	/** @brief 图像处理命令*/
	IMAGE_CMD,
	/** @brief 控制命令*/
	CONTROL_CMD,
	/** @brief For test*/
	TEST_CMD
};

/** @brief LOAD数据分类 */
enum {
	/** @brief 图像数据*/
	IMAGE_DATA = 0,
	/** @brief Edge坐标数据 */
	EDGE_COORD_DATA
};

/** @brief 拍照或重拍或放弃*/
typedef enum _PictureType_En_tag {
	PHOTO_TAKE = 0,
	PHOTO_RETAKE,
	PHOTO_ABANDON
} CaptureType_En;

/** @brief 录相命令*/
typedef enum _VideoCmd_En_tag {
	VIDEO_START = 0,
	VIDEO_STOP,
	VIDEO_QUIT,
	VIDEO_TIMEOUT
} VideoCmd_En;

/** @brief 图像或视频*/
typedef enum _ImgVideo_En_tag {
	IMG_VIDEO_BOTH = 0,
	IMAGE,
	VIDEO
} ImgVideo_En;

/** @brief Light种类*/
typedef enum _ImgLight_En_tag {
	IMG_LIGHT_NOTHING = 0,
	IMAGE_LIGHT,	// 图像
	VIDEO_LIGHT,	// 视频
	ILLUME_LIGHT	// 照明
} ImgLight_En;

/** @brief 图像或视频*/
typedef enum _Delete_En_tag {
	DELETE_ALL = 0,
	DELETE_SPEC
} Delete_En;

/** @brief Wifi Hotspot/Station*/
typedef enum _WifiCategory_En_tag {
	WIFI_HOTSPOT = 0,
	WIFI_STATION,
	WIFI_ASK
} WifiCategory_En;

/** @brief Device info*/
typedef enum _DeviceInfo_En_tag {
	DEV_INFO_DUMMY = 0,
	DEV_INFO_MAC,	// mac
	DEV_INFO_IP,	// ip
	DEV_INFO_VER	// firm version
} DeviceInfo_En;

/** @brief Device info*/
typedef enum _DeviceReset_En_tag {
	DEV_RESET_ALL = 0,
	DEV_RESET_STA,		// Reset station net interface
	DEV_RESET_LOG,		// Reset log
	DEV_RESET_PIC,		// Reset pictures
	DEV_RESET_VIDEO,	// Reset videos
	DEV_RESET_DEV,		// Reset device
	DEV_RESET_NUM
} DeviceReset_En;

/** @brief 命令Table*/
typedef	struct _CmdTable_St_tag_
{
	/** @brief 命令ID*/
	unsigned int	uiCmdID;
	/** @brief 命令名称*/
	const char		aCmdName[CMDNAME_LENGTH];
	/** @brief 命令分类*/
	unsigned int	uiCmdType;
	/** @brief 命令详细分类*/
	unsigned int	uiCmdDetail;
	/** @brief 命令解析函数*/
	int		(*check)(char* p);
	/** @brief 命令支行函数*/
	int 	(*run)(void* p);
	/** @brief 有必要清除错误否*/
	unsigned int    uiErrClear;
}	CmdTable_St;

/**
 * COMMAND
 */
typedef struct {
	char command[PARAM_LENGTH];
	char param[PARAM_MAX][PARAM_LENGTH];
} COMMAND_BASE;

/**
 * SHUTDOWN 命令
 */
typedef struct _CmdShutDown_St_tag {
	unsigned int  uiDummy;
} CmdShutDown_St;

/**
 * RESET 命令
 */
typedef struct _CmdReset_St_tag {
	unsigned int uiNum;				/*The number of the reset items*/
	unsigned int uiCategory[4];		/*0:all 1:net interface 2:log 3:photos 4:videos*/
} CmdReset_St;

/**
 * LIGHT 命令
 */
typedef struct _CmdLight_St_tag {
	unsigned int uiCategory;	/*Photo/Video*/
	unsigned int uiEnable;		/*ON/OFF*/
	unsigned int uiBrightness;	/*亮度*/
} CmdLight_St;

/**
 * PICTURE 命令
 */
typedef struct _CmdPicture_St_tag {
	char aNameAddr[NAME_LENGTH];	/*用户名或设备名长度*/
	unsigned int uiAction;		/*拍照 重拍 放弃*/
	unsigned int uiResolutionW;	/*分辨率W*/
	unsigned int uiResolutionH; /*分辨率H*/
	char fileName[FILE_MAX_LENGTH];	/*文件名*/
	char folderName[FILE_MAX_LENGTH]; /*folder name*/
} CmdPicture_St;

/**
 * MOVIE 命令
 */
typedef struct _CmdMovie_St_tag {
	char aNameAddr[NAME_LENGTH];			/*用户名或设备名长度*/
	unsigned int uiAction;					/*动作 0:start 1:stop*/
	char fileName[FILE_MAX_LENGTH];			/*文件名*/
	char mp4FileName[FILE_MAX_LENGTH];		/*MP$文件名*/
	char fileNameNoHead[FILE_MAX_LENGTH];	/*文件名(无/var/www)*/
	char mp4NoHead[FILE_MAX_LENGTH];		/*文件名(无/var/www)*/
	char folderName[FILE_MAX_LENGTH]; /*folder name*/
} CmdMovie_St;

/**
 * LOAD 命令
 */
typedef struct _CmdLoad_St_tag {
	unsigned int  uiDataType;		/**/
    char fileName[FILE_MAX_LENGTH];	/*文件名*/
    unsigned int  uiWorkSpaceId;	/*Workspace ID*/
} CmdLoad_St;

/**
 * SAVE 命令
 */
typedef struct _CmdSave_St_tag {
	unsigned int  uiDataType;		/**/
    unsigned int  uiWorkSpaceId;	/*Workspace ID*/
    char fileName[FILE_MAX_LENGTH];	/*文件名*/
} CmdSave_St;

/**
 * UPLOAD 命令
 */
typedef struct _CmdUpload_St_tag {
	char aNameAddr[NAME_LENGTH];	/*用户名或设备名长度*/
	unsigned int  uiCategory;		/*上传文件类型  1:图像 2:视频 */
	unsigned int  uiFileNum;		/*上传文件数量*/
} CmdUpload_St;

/**
 * SYNC 命令
 */
typedef struct _CmdSync_St_tag {
	char aNameAddr[NAME_LENGTH];	/*用户名或设备名长度*/
	unsigned int  uiCategory;		/*下传文件类型 0:图像和视频 1:图像 2:视频 */
} CmdSync_St;

/**
 * Delete 命令
 */
typedef struct _CmdDelete_St_tag {
	char aNameAddr[NAME_LENGTH];	/*用户名或设备名长度*/
	unsigned int  uiCategory;		/*删除类别 0：全删除 1：指定删除*/
	unsigned int uiMount;			/*删除文件数量*/
	char aDeleteFile[MAX_FILENUM][FILENAME_LENGTH];	/*删除的文件名*/
} CmdDelete_St;

/**
 * SET命令
 */
typedef struct _CmdSet_St_tag
{
	unsigned int	 uiItem;		/*Set项*/
	NumericType	 value;			/*Set值*/
	bool		 bFloatValue;	/*是不是Float*/
	bool		 bCharValue;	/*是不是Char数组*/
} CmdSet_St;

/**
 * RQ命令
 */
typedef struct _CmdRq_St_tag
{
	unsigned int	uiItem;			/*请求项*/
} CmdRq_St;


/**
 * ERRRST命令
 */
typedef struct _CmdErrrst_St_tag {
	unsigned int	uiDummy;
} CmdErrrst_St;

/**
 * IPCHG命令
 */
typedef struct _CmdIpChg_St_tag {
    char ipaddr[IP_LENGTH];					/*IP地址*/
    char netmask[IP_LENGTH];				/*网络掩码*/
} CmdIpChg_St;

/**
 * GETLOG命令
 */
typedef struct _CmdGetLog_St_tag {
	unsigned int	uiDummy;
} CmdGetLog_St;

/**
 * SEARCH命令
 */
typedef struct _CmdSearch_St_tag {
	unsigned long	ulUTC;
} CmdSearch_St;

/**
 * SETWIFI命令
 */
typedef struct _CmdSetWifi_St_tag {
	unsigned int category;		/*0:hotspot 1:station*/
	char ssid[SSID_LENGTH];
	char pass[SSID_LENGTH];
	unsigned int status;		/*wifi status*/
} CmdSetWifi_St;

/**
 * OBTINFO命令
 */
typedef struct _CmdObtInfo_St_tag {
	unsigned int uiInfo;	/*1:mac 2:ip 3:ver*/
} CmdObtInfo_St;

/**
 * DEVPIC命令
 */
typedef struct _CmdDevPic_St_tag {
	char mac[NAME_LENGTH];	/*mac of the device*/
	unsigned int uiAction;	/*0:拍照开始 1:拍照结束*/
	char sResultLink[RESPONSE_LENGTH];	/*结果link*/
} CmdDevPic_St;

/**
 * DEVPIC命令
 */
typedef struct _CmdDevMovie_St_tag {
	char mac[NAME_LENGTH];	/*mac of the device*/
	unsigned int uiAction;	/*0-摄像开始 1-摄像结束 2-超时终止*/
	char sResultLink[RESPONSE_LENGTH];	/*结果link*/
} CmdDevMovie_St;

/**
 * DEVPIC命令
 */
typedef struct _CmdPhotoCate_St_tag {
	unsigned int uiCategory;	/*0-原始图像 1-原始图像和舌头图像 2-原始图像和舌头嘴唇图像 3-原始图像和舌头图像和舌头嘴唇图像*/
} CmdPhotoCate_St;

/**
 * Command构造体
 */
typedef struct _Command_St_tag {
	unsigned int 	uiCmdId;
	union {
		CmdShutDown_St 		stCmdShutDown;			/*SHUTDOWN命令*/
		CmdReset_St			stCmdReset;				/*RESET命令*/
		CmdLight_St			stCmdLight;				/*LIGHT命令*/
		CmdPicture_St		stCmdPicture;			/*PICTURE命令*/
		CmdMovie_St			stCmdMovie;				/*MOVIE命令*/
		CmdLoad_St			stCmdLoad;				/*LOAD命令*/
		CmdSave_St			stCmdSave;				/*SAVE命令*/
		CmdUpload_St		stCmdUpload;			/*UPLOAD命令*/
		CmdSync_St			stCmdSync;				/*SYNC命令*/
		CmdDelete_St		stCmdDelete;			/*DELETE命令*/
		CmdSet_St			stCmdSet;				/*SET命令*/
		CmdRq_St			stCmdRq;				/*RQ命令*/
		CmdErrrst_St		stCmdErrrst;			/*ERRRST命令*/
		CmdIpChg_St			stCmdIpChg;				/*IPCHG命令*/
		CmdGetLog_St		stCmdGetLog;			/*GETLOG命令*/
		CmdSearch_St		stCmdSearch;			/*SEARCH命令*/
		CmdSetWifi_St		stCmdSetWifi;			/*SETWIFI*/
		CmdObtInfo_St		stCmdObtInfo;			/*OBTINFO*/
		CmdDevPic_St		stCmdDevPic;			/*DEVPIC*/
		CmdDevMovie_St		stCmdDevMovie;			/*DEVMOVIE*/
		CmdPhotoCate_St		stCmdPhotoCate;			/*PHOTOCATE*/
	} unCommand;
} Command_St;

// Command Info
typedef struct _CmdInfo_St_tag {
	unsigned int uiCmdId;			// 命令ID
	unsigned int uiCmdType;			// 命令种类
	unsigned int uiSeqId;			// Sequence ID
	RetMessage_En uiRet;			// 应答类型
	unsigned int uiErrCode;			// Error Code
	char	data[RESPONSE_LENGTH];
} CmdInfo_St;

// LOAD Info
typedef struct _LoadInfo_St_tag {
	unsigned int uiDataType;		//
	char fileName[FILE_MAX_LENGTH];	/*文件名*/
	unsigned int  uiWorkSpaceId;	/*Workspace ID*/
} LoadInfo_St;

// SAVE Info
typedef struct _SaveInfo_St_tag {
	unsigned int uiDataType;		//
	unsigned int  uiWorkSpaceId;	/*Workspace ID*/
	char fileName[FILE_MAX_LENGTH];	/*文件名*/
} SaveInfo_St;

extern CmdInfo_St g_cmdInfo;

extern Command_St	g_stCommand;
extern SystemParam_Map 	g_systemParamMap[];
extern CmdTable_St g_staCmdTable[];

extern void InitCmdBase(COMMAND_BASE* io_pCmdBase);
extern int SplitCmd(char* in_pParameter, COMMAND_BASE* io_pCmdBase);
extern unsigned int GetCommandNo(char* pCommand);
extern int JudgeCmdType(uint32_t uiCmdNo);
extern char* GetCommand(char* pTo, const char* pCommand);
extern int JudgeCmdConstruct(char* pCommand, unsigned int uiCmdNo);
extern bool IsDigital(char* in_pStr);
extern bool IsFloatDigital(char* in_pStr);
extern int IpToInt(char* in_pIp);
extern bool IntToIp(int iIp, char* out_pIp);
extern bool IsNetMask(char* in_pMask);
extern int CheckMaskValue(int in_Value);
extern bool IsDate(char* in_pDate, int iLength);
extern int diffMs(struct timeval t1, struct timeval t2);
extern const char* CreateImageJSONObj(char* in_pUserID, char * in_pMachine, char* in_IpAddr);
extern const char* CreateVideoJSONObj(char* in_pUserID, char * in_pMachine, char* in_IpAddr);

extern const char* getErrName(int iEnumNum);
extern int 	getErrId(int iEnumNum);
extern const char*	getErrDetail(int iEnumNum);

#endif /* TONGUECTRLCOMMAND_H_ */
