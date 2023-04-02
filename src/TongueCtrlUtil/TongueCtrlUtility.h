/*****************************************************************************/
/**
* @file TongueCtrlCommon.h
* @brief 舌苔控制Utility涵 数定义
*
* 舌苔控制Utility涵 数定义
*
* @author wangwei
* @date 2016/06/12 wangwei 初始版本
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/

#ifndef TONGUECTRLUTILITY_H_
#define TONGUECTRLUTILITY_H_

/** @brief IPCHG命令Length*/
#define IPCHG_COMMAND_MAX			52

/*进程种类*/
typedef enum PROCESS_TYPE_EN {
	TONGUE_PHOTO = 0,
	TONGUE_VIDEO,
	TONGUE_VIDEO_END,
	TONGUE_VIDEO_QUIT
} PROCESS_TYPE;

typedef struct _TonguePhotoParam_En_tag {
	int		iWidth;
	int 	iHeigth;
	char*	sFileName;
	char*	sFolderName;
	char*	sTongueFolderName;
	char* 	sTongueLipFolderName;
	char* 	sOriginFileName;	/*no extent*/
	char* 	sTongueFileName;	/*no extent*/
	char* 	sTongueLipFileName;	/*no extent*/
} TonguePhotoParam_En;

typedef struct _TongueVidParam_En_tag {
	int 	iWidth;
	int 	iHeigth;
	char* 	sFileName;
	char*	sFolderName;
	int		iType;		// Video or Photo
} TongueVidParam_En;

extern TongueVidParam_En g_tongueVidParam;

extern void ShutDown();
extern void Reset();
extern void Reboot(void);
extern void IpChg();
extern void Move(char* in_fileName,  char const* in_folderName);
extern int Remove(char *in_fileName);
extern int RemoveAll(char *in_fileName);
extern int StartProcess(PROCESS_TYPE processType, void* param);
extern void CreateLogFile(char *in_folderName, char* out_tarFileName);
extern int CompressFiles(char *in_folderName, char* out_tarFileName);
extern int GetMacAddr(char *out_mac);
extern void ChangeToHotspot(void);
extern void ChangeToStation(char *in_ssid, char *in_pass);
extern void CheckWifiStatus(void);
extern void SetHostName(void);
extern void SetDeviceFolder(void);
extern int GetFirmareVersion(char* out_version);
extern void Change2Mp4(char* in_fileName, char* out_fileName);
extern void StartUpgrade(void);
extern int PlaySound(const char* in_sound);
extern int CreatePhoto(void* param, int in_iCategory);
extern int JudgeFileCategory(char* in_fileName, int in_dotPos);
extern void RpmStart(void);
extern unsigned long GetFileSize(const char* in_path);
extern void SetTime(unsigned long in_lTime);
extern int GetPhotoCategory(char* out_number);
extern int SetPhotoCategory(char *in_number);

#endif /* TONGUECTRLUTILITY_H_ */
