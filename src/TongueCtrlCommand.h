/*****************************************************************************/
/**
* @file TongueCtrlCommand.h
* @brief ��̦��������ͷ�ļ�
*
* ������̦���������
*
* @author wangwei
* @date 2016/06/04 wangwei ��ʼ�汾
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
#define DATE_LENGHT			14				/*����Length YYYYMMDDHHmmSS*/
#define DATE_HYPHEN_LENGHT	19				/*����Length YYYY-MM-DD-HH-mm-SS*/
#define DATE_DIGIT			6				/*����λ��*/
/** @brief Ӧ������Length */
#define RSPNAME_LENGTH		10
/** @brief error����Length */
#define ERRNAME_LENGTH		50
/** @brief Command���� */
//#define COMMAND_LENGTH		64
/** @brief Response���� */
#define RESPONSE_LENGTH		1024
/** @brief IP���� */
#define IP_LENGTH			20
/** @brief �ո� */
#define BLANK 		0x20
#define CR			0x0D
#define LF			0x0A
//#define CS			0x00
/** @brief Command����Length */
#define CMDNAME_LENGTH		20

/** @brief IP��ַLength*/
#define IP_LENGTH			20
/** @brief �û���Length*/
#define NAME_LENGTH			50
/** @brief �ļ���Length*/
#define FILENAME_LENGTH		1024
/** @brief Link Length*/
#define LINK_LENGTH			10240
/** @brief �ļ����������*/
#define MAX_FILENUM			50
/** @brief ���������� */
#define NO_ERR_CLEAR		0
/** @brief ��Ҫ������� */
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

#define PARAM_LENGTH		100				/*����Length*/
#define PARAM_MAX			20				/*������������*/
#define FILE_MAX_LENGTH		255				/*�ļ�Max Length*/

#define HYPHEN		'-'

#define MAX_YEAR					10000		// �����
#define MIN_YEAR					999			// ��С��
#define BIG_MONTH_DAY				31			// ����(1,3,5,7,8,10,12)
#define SMALL_MONTH_DAY				30			// ����(4,6,9,11)

// Chamber�ŷ�Χ
#define MAX_CHAMBER_NO				16
#define MIN_CHAMBER_NO				1
// Duty��Χ
#define MAX_DUTY					100.0f
#define MIN_DUTY					0.1f
// Data Type��Χ
#define MAX_DATA_TYPE				2
#define MIN_DATA_TYPE				0
// Worksapce Id��Χ
#define MAX_WORKSPACE_ID			8
#define MIN_WORKSPACE_ID			1
// Sub Pixel��Χ
#define MAX_SUBPIXEL				5
#define MIN_SUBPIXEL				1
// ��ֵ��������ֵ
#define MAX_BIN_THRESH				255
#define MIN_BIN_THRESH				0
// Edge������interval��Χ
#define MAX_EDGE_INTERVAL			20
#define MIN_EDGE_INTERVAL			0
// NOTCH���������Χ
#define MAX_EXCLU_NUM				50
#define MIN_EXCLU_NUM				0

#define FEB_NO_LEAP					28			// 2�µ������������꣩
#define FEB_LEAP					29			// 2�µ����������꣩

#define LIGHT_ON					"ON"		// LIGHT ON
#define LIGHT_OFF					"OFF"		// LIGHT OFF

/*
 * ������
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

/** @brief ������� */
typedef enum _CmdType_En_tag_
{
	/** @brief DUMMY����*/
	DUMMY_CMD = 0,
	/** @brief ��������*/
	ACTION_CMD,
	/** @brief ��������*/
	REQUEST_CMD,
	/** @brief ��������*/
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
	/** @brief ��Ч*/
	ENABLE_OFF = 0,
	/** @brief ����*/
	ENABLE_ON,
	/** @brief ����*/
	ENABLE_DONE
};

/* Ӧ������  */
typedef enum _RetMessage_En_tag {
	DUMMY_RET = 0,
	RDY_RET,							/*����Ӧ��*/
	ERR_RET,							/*�쳣Ӧ��*/
	REP_RET,							/*���������Ӧ��*/
} RetMessage_En;


/** @brief �����Sequence���� */
enum {
	/** @brief ��������*/
	ERROR_CMD = -1,
	/** @brief Dummy*/
	DUM_CMD,
	/** @brief ��������*/
	RQ_CMD,
	/** @brief �趨����*/
	SET_CMD,
	/** @brief ͼ��������*/
	IMAGE_CMD,
	/** @brief ��������*/
	CONTROL_CMD,
	/** @brief For test*/
	TEST_CMD
};

/** @brief LOAD���ݷ��� */
enum {
	/** @brief ͼ������*/
	IMAGE_DATA = 0,
	/** @brief Edge�������� */
	EDGE_COORD_DATA
};

/** @brief ���ջ����Ļ����*/
typedef enum _PictureType_En_tag {
	PHOTO_TAKE = 0,
	PHOTO_RETAKE,
	PHOTO_ABANDON
} CaptureType_En;

/** @brief ¼������*/
typedef enum _VideoCmd_En_tag {
	VIDEO_START = 0,
	VIDEO_STOP,
	VIDEO_QUIT,
	VIDEO_TIMEOUT
} VideoCmd_En;

/** @brief ͼ�����Ƶ*/
typedef enum _ImgVideo_En_tag {
	IMG_VIDEO_BOTH = 0,
	IMAGE,
	VIDEO
} ImgVideo_En;

/** @brief Light����*/
typedef enum _ImgLight_En_tag {
	IMG_LIGHT_NOTHING = 0,
	IMAGE_LIGHT,	// ͼ��
	VIDEO_LIGHT,	// ��Ƶ
	ILLUME_LIGHT	// ����
} ImgLight_En;

/** @brief ͼ�����Ƶ*/
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

/** @brief ����Table*/
typedef	struct _CmdTable_St_tag_
{
	/** @brief ����ID*/
	unsigned int	uiCmdID;
	/** @brief ��������*/
	const char		aCmdName[CMDNAME_LENGTH];
	/** @brief �������*/
	unsigned int	uiCmdType;
	/** @brief ������ϸ����*/
	unsigned int	uiCmdDetail;
	/** @brief �����������*/
	int		(*check)(char* p);
	/** @brief ����֧�к���*/
	int 	(*run)(void* p);
	/** @brief �б�Ҫ��������*/
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
 * SHUTDOWN ����
 */
typedef struct _CmdShutDown_St_tag {
	unsigned int  uiDummy;
} CmdShutDown_St;

/**
 * RESET ����
 */
typedef struct _CmdReset_St_tag {
	unsigned int uiNum;				/*The number of the reset items*/
	unsigned int uiCategory[4];		/*0:all 1:net interface 2:log 3:photos 4:videos*/
} CmdReset_St;

/**
 * LIGHT ����
 */
typedef struct _CmdLight_St_tag {
	unsigned int uiCategory;	/*Photo/Video*/
	unsigned int uiEnable;		/*ON/OFF*/
	unsigned int uiBrightness;	/*����*/
} CmdLight_St;

/**
 * PICTURE ����
 */
typedef struct _CmdPicture_St_tag {
	char aNameAddr[NAME_LENGTH];	/*�û������豸������*/
	unsigned int uiAction;		/*���� ���� ����*/
	unsigned int uiResolutionW;	/*�ֱ���W*/
	unsigned int uiResolutionH; /*�ֱ���H*/
	char fileName[FILE_MAX_LENGTH];	/*�ļ���*/
	char folderName[FILE_MAX_LENGTH]; /*folder name*/
} CmdPicture_St;

/**
 * MOVIE ����
 */
typedef struct _CmdMovie_St_tag {
	char aNameAddr[NAME_LENGTH];			/*�û������豸������*/
	unsigned int uiAction;					/*���� 0:start 1:stop*/
	char fileName[FILE_MAX_LENGTH];			/*�ļ���*/
	char mp4FileName[FILE_MAX_LENGTH];		/*MP$�ļ���*/
	char fileNameNoHead[FILE_MAX_LENGTH];	/*�ļ���(��/var/www)*/
	char mp4NoHead[FILE_MAX_LENGTH];		/*�ļ���(��/var/www)*/
	char folderName[FILE_MAX_LENGTH]; /*folder name*/
} CmdMovie_St;

/**
 * LOAD ����
 */
typedef struct _CmdLoad_St_tag {
	unsigned int  uiDataType;		/**/
    char fileName[FILE_MAX_LENGTH];	/*�ļ���*/
    unsigned int  uiWorkSpaceId;	/*Workspace ID*/
} CmdLoad_St;

/**
 * SAVE ����
 */
typedef struct _CmdSave_St_tag {
	unsigned int  uiDataType;		/**/
    unsigned int  uiWorkSpaceId;	/*Workspace ID*/
    char fileName[FILE_MAX_LENGTH];	/*�ļ���*/
} CmdSave_St;

/**
 * UPLOAD ����
 */
typedef struct _CmdUpload_St_tag {
	char aNameAddr[NAME_LENGTH];	/*�û������豸������*/
	unsigned int  uiCategory;		/*�ϴ��ļ�����  1:ͼ�� 2:��Ƶ */
	unsigned int  uiFileNum;		/*�ϴ��ļ�����*/
} CmdUpload_St;

/**
 * SYNC ����
 */
typedef struct _CmdSync_St_tag {
	char aNameAddr[NAME_LENGTH];	/*�û������豸������*/
	unsigned int  uiCategory;		/*�´��ļ����� 0:ͼ�����Ƶ 1:ͼ�� 2:��Ƶ */
} CmdSync_St;

/**
 * Delete ����
 */
typedef struct _CmdDelete_St_tag {
	char aNameAddr[NAME_LENGTH];	/*�û������豸������*/
	unsigned int  uiCategory;		/*ɾ����� 0��ȫɾ�� 1��ָ��ɾ��*/
	unsigned int uiMount;			/*ɾ���ļ�����*/
	char aDeleteFile[MAX_FILENUM][FILENAME_LENGTH];	/*ɾ�����ļ���*/
} CmdDelete_St;

/**
 * SET����
 */
typedef struct _CmdSet_St_tag
{
	unsigned int	 uiItem;		/*Set��*/
	NumericType	 value;			/*Setֵ*/
	bool		 bFloatValue;	/*�ǲ���Float*/
	bool		 bCharValue;	/*�ǲ���Char����*/
} CmdSet_St;

/**
 * RQ����
 */
typedef struct _CmdRq_St_tag
{
	unsigned int	uiItem;			/*������*/
} CmdRq_St;


/**
 * ERRRST����
 */
typedef struct _CmdErrrst_St_tag {
	unsigned int	uiDummy;
} CmdErrrst_St;

/**
 * IPCHG����
 */
typedef struct _CmdIpChg_St_tag {
    char ipaddr[IP_LENGTH];					/*IP��ַ*/
    char netmask[IP_LENGTH];				/*��������*/
} CmdIpChg_St;

/**
 * GETLOG����
 */
typedef struct _CmdGetLog_St_tag {
	unsigned int	uiDummy;
} CmdGetLog_St;

/**
 * SEARCH����
 */
typedef struct _CmdSearch_St_tag {
	unsigned long	ulUTC;
} CmdSearch_St;

/**
 * SETWIFI����
 */
typedef struct _CmdSetWifi_St_tag {
	unsigned int category;		/*0:hotspot 1:station*/
	char ssid[SSID_LENGTH];
	char pass[SSID_LENGTH];
	unsigned int status;		/*wifi status*/
} CmdSetWifi_St;

/**
 * OBTINFO����
 */
typedef struct _CmdObtInfo_St_tag {
	unsigned int uiInfo;	/*1:mac 2:ip 3:ver*/
} CmdObtInfo_St;

/**
 * DEVPIC����
 */
typedef struct _CmdDevPic_St_tag {
	char mac[NAME_LENGTH];	/*mac of the device*/
	unsigned int uiAction;	/*0:���տ�ʼ 1:���ս���*/
	char sResultLink[RESPONSE_LENGTH];	/*���link*/
} CmdDevPic_St;

/**
 * DEVPIC����
 */
typedef struct _CmdDevMovie_St_tag {
	char mac[NAME_LENGTH];	/*mac of the device*/
	unsigned int uiAction;	/*0-����ʼ 1-������� 2-��ʱ��ֹ*/
	char sResultLink[RESPONSE_LENGTH];	/*���link*/
} CmdDevMovie_St;

/**
 * DEVPIC����
 */
typedef struct _CmdPhotoCate_St_tag {
	unsigned int uiCategory;	/*0-ԭʼͼ�� 1-ԭʼͼ�����ͷͼ�� 2-ԭʼͼ�����ͷ�촽ͼ�� 3-ԭʼͼ�����ͷͼ�����ͷ�촽ͼ��*/
} CmdPhotoCate_St;

/**
 * Command������
 */
typedef struct _Command_St_tag {
	unsigned int 	uiCmdId;
	union {
		CmdShutDown_St 		stCmdShutDown;			/*SHUTDOWN����*/
		CmdReset_St			stCmdReset;				/*RESET����*/
		CmdLight_St			stCmdLight;				/*LIGHT����*/
		CmdPicture_St		stCmdPicture;			/*PICTURE����*/
		CmdMovie_St			stCmdMovie;				/*MOVIE����*/
		CmdLoad_St			stCmdLoad;				/*LOAD����*/
		CmdSave_St			stCmdSave;				/*SAVE����*/
		CmdUpload_St		stCmdUpload;			/*UPLOAD����*/
		CmdSync_St			stCmdSync;				/*SYNC����*/
		CmdDelete_St		stCmdDelete;			/*DELETE����*/
		CmdSet_St			stCmdSet;				/*SET����*/
		CmdRq_St			stCmdRq;				/*RQ����*/
		CmdErrrst_St		stCmdErrrst;			/*ERRRST����*/
		CmdIpChg_St			stCmdIpChg;				/*IPCHG����*/
		CmdGetLog_St		stCmdGetLog;			/*GETLOG����*/
		CmdSearch_St		stCmdSearch;			/*SEARCH����*/
		CmdSetWifi_St		stCmdSetWifi;			/*SETWIFI*/
		CmdObtInfo_St		stCmdObtInfo;			/*OBTINFO*/
		CmdDevPic_St		stCmdDevPic;			/*DEVPIC*/
		CmdDevMovie_St		stCmdDevMovie;			/*DEVMOVIE*/
		CmdPhotoCate_St		stCmdPhotoCate;			/*PHOTOCATE*/
	} unCommand;
} Command_St;

// Command Info
typedef struct _CmdInfo_St_tag {
	unsigned int uiCmdId;			// ����ID
	unsigned int uiCmdType;			// ��������
	unsigned int uiSeqId;			// Sequence ID
	RetMessage_En uiRet;			// Ӧ������
	unsigned int uiErrCode;			// Error Code
	char	data[RESPONSE_LENGTH];
} CmdInfo_St;

// LOAD Info
typedef struct _LoadInfo_St_tag {
	unsigned int uiDataType;		//
	char fileName[FILE_MAX_LENGTH];	/*�ļ���*/
	unsigned int  uiWorkSpaceId;	/*Workspace ID*/
} LoadInfo_St;

// SAVE Info
typedef struct _SaveInfo_St_tag {
	unsigned int uiDataType;		//
	unsigned int  uiWorkSpaceId;	/*Workspace ID*/
	char fileName[FILE_MAX_LENGTH];	/*�ļ���*/
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
