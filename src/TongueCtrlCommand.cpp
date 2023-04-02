/*****************************************************************************/
/**
* @file TongueCtrlCommand.cpp
* @brief 舌苔控制命令
*
* 定义舌苔控制命令
*
* @author wangwei
* @date 2016/06/06 wangwei 初始版本
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

extern "C"
{
#include <json/json.h>
}

#include "TongueCtrlCommand.h"
#include "TongueCtrlCMD/TongueCtrlCmdCheck.h"
#include "TongueCtrlCMD/TongueCtrlCmdRun.h"
#include "TongueCtrlCommon.h"
#include "TongueCtrlErrProc.h"
#include "TongueCtrlLogThd.h"


/*CLOCK Info*/
SystemClock_St			g_stSystemClock;

// 命令
CmdInfo_St g_cmdInfo;

Command_St	g_stCommand;

CmdTable_St g_staCmdTable[CMD_NUM] = {
		{CMD_DUMMY,				"DUMMY",			DUMMY_CMD,			DUM_CMD,		CheckDummyCmd,					RunDummyCmd,				NEED_ERR_CLEAR},
		{CMD_SHUTDOWN,			"SHUTDOWN",			ACTION_CMD,			CONTROL_CMD, 	CheckShutdownCmd,				RunShutdownCmd,				NEED_ERR_CLEAR},
		{CMD_RESET,				"RESET",			ACTION_CMD,			CONTROL_CMD,	CheckResetCmd,					RunResetCmd,				NEED_ERR_CLEAR},
		{CMD_REBOOT,			"REBOOT",			ACTION_CMD,			CONTROL_CMD,	CheckRebootCmd,					RunRebootCmd,				NEED_ERR_CLEAR},
		{CMD_LIGHT,				"LIGHT",			ACTION_CMD,			CONTROL_CMD,	CheckLightCmd,					RunLightCmd,				NEED_ERR_CLEAR},
		{CMD_PICTURE,			"PICTURE",			ACTION_CMD,			RQ_CMD,			CheckPictureCmd,				RunPictureCmd,				NEED_ERR_CLEAR},
		{CMD_MOVIE,				"MOVIE",			ACTION_CMD,			RQ_CMD,			CheckMovieCmd,					RunMovieCmd,				NEED_ERR_CLEAR},
		{CMD_LOAD,				"LOAD",				ACTION_CMD,			IMAGE_CMD,		CheckLoadCmd,					RunLoadCmd,					NEED_ERR_CLEAR},
		{CMD_SAVE,				"SAVE",				ACTION_CMD,			IMAGE_CMD,		CheckSaveCmd,					RunSaveCmd,					NEED_ERR_CLEAR},
		{CMD_UPLOAD,			"UPLOAD",			ACTION_CMD,			CONTROL_CMD,	CheckUploadCmd,					RunUploadCmd,				NEED_ERR_CLEAR},
		{CMD_SYNC,				"SYNC",				ACTION_CMD,			RQ_CMD,			CheckSyncCmd,					RunSyncCmd,					NEED_ERR_CLEAR},
		{CMD_DELETE,			"DELETE",			ACTION_CMD,			CONTROL_CMD,	CheckDeleteCmd,					RunDeleteCmd,				NEED_ERR_CLEAR},
		{CMD_SET,				"SET",				ACTION_CMD,			SET_CMD, 		CheckSetCmd,					RunSetCmd,					NEED_ERR_CLEAR},
		{CMD_RQ,				"RQ",				ACTION_CMD,			RQ_CMD, 		CheckRqCmd,						RunRqCmd,					NEED_ERR_CLEAR},
		{CMD_ERRRST,			"ERRRST",			ACTION_CMD,			CONTROL_CMD,	CheckErrrstCmd,					RunErrrstCmd,				NEED_ERR_CLEAR},
		{CMD_IPCHG,				"IPCHG",			ACTION_CMD,			CONTROL_CMD,	CheckIpChgCmd,					RunIpChgCmd,				NEED_ERR_CLEAR},
		{CMD_GETLOG,			"GETLOG",			ACTION_CMD,			RQ_CMD,			CheckGetLogCmd,					RunGetLogCmd,				NEED_ERR_CLEAR},
		{CMD_UPGRADE,			"UPGRADE",			ACTION_CMD,			CONTROL_CMD,	CheckUpgradeCmd,				RunUpgradeCmd,				NEED_ERR_CLEAR},
		{CMD_SEARCH,			"SEARCH",			ACTION_CMD,			RQ_CMD,			CheckSearchCmd,					RunSearchCmd,				NEED_ERR_CLEAR},
		{CMD_SETWIFI,			"SETWIFI",			ACTION_CMD,			RQ_CMD,			CheckSetWifiCmd,				RunSetWifiCmd,				NEED_ERR_CLEAR},
		{CMD_OBTINFO,			"OBTINFO",			ACTION_CMD,			RQ_CMD,			CheckObtInfoCmd,				RunObtInfoCmd,				NEED_ERR_CLEAR},
		{CMD_DEVPIC,			"DEVPIC",			ACTION_CMD,			RQ_CMD,			CheckDevPicCmd,					RunDevPicCmd,				NEED_ERR_CLEAR},
		{CMD_DEVMOVIE,			"DEVMOVIE",			ACTION_CMD,			RQ_CMD,			CheckDevMovieCmd,				RunDevMovieCmd,				NEED_ERR_CLEAR},
		{CMD_PHOTOCATG,			"PHOTOCATG",		ACTION_CMD,			CONTROL_CMD,	CheckPhotoCatgCmd,				RunPhotoCatgCmd,			NEED_ERR_CLEAR},
		{CMD_RPMSTART,			"RPMSTART",			ACTION_CMD,			CONTROL_CMD,	CheckRpmStartCmd,				RunRpmStartCmd,				NEED_ERR_CLEAR},

};

/**
 * @brief 命令参数数组的初始化
 *
 * 初始化命令参数数组
 *
 * @param io_pCmdBase 命令参数数组
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
void InitCmdBase(COMMAND_BASE* io_pCmdBase)
{
	int i = 0;

	memset(io_pCmdBase->command, '\0', PARAM_LENGTH);
	for (i = 0; i < PARAM_MAX; i++) {
		memset(io_pCmdBase->param[i], '\0', PARAM_LENGTH);
	}

	return;
}

/**
 * @brief 命令的分割
 *
 * 分割命令
 *
 * @param in_pParameter 参数1
 * @param io_pCmdBase  参数2
 * @return 0:正常 1:异常
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int SplitCmd(char* in_pParameter, COMMAND_BASE* io_pCmdBase)
{
	int iResult = 0;
	char *p0, *p1, *q;
	unsigned int i = 0;
	bool bLastParam = false;

	p0 = p1 = in_pParameter;
	while (i < PARAM_MAX) {
		while (*p0 != BLANK && *p0 != '\0') {
			p0++;
		}
		if (*p0 != BLANK) {
			bLastParam = true;
		}

		*p0 = '\0';
		q = p0;

		if (!strcmp(p1, "BINARY") || !strcmp(p1, "LIGHT") || !strcmp(p1, "CALC")) {
			*p0++ = BLANK;
			while (*p0 != BLANK && *p0 != '\0') {
				p0++;
			}
			if (*p0 != BLANK) {
				bLastParam = true;
			}
			*p0 = '\0';
			if (!strcmp(p1, "LIGHT ADJUST") || !strcmp(p1, "BINARY ADJUST") || !strcmp(p1, "CALC CALIB")) {
				*p0++ = BLANK;
				while (*p0 != BLANK && *p0 != '\0') {
					p0++;
				}
				if (*p0 != BLANK) {
					bLastParam = true;
				}
				*p0 = '\0';
			}
			else {
				*q++ = '\0';
				strncpy(io_pCmdBase->command, p1, PARAM_LENGTH);
				i++;
				p1 = q;
			}
		}

		if (i == 0) {
			strncpy(io_pCmdBase->command, p1, PARAM_LENGTH);
		}
		else {
			strncpy(io_pCmdBase->param[i-1], p1, PARAM_LENGTH);
		}

		if (bLastParam) {
			break;
		}

		i++;
		p1 = ++p0;
	}

	if (i >= PARAM_MAX) iResult = -1;

	return iResult;
}


/**
 * @brief 取得命令
 *
 * 取得命令
 *
 * @param pTo 命令目的
 * @param pCommand 目的源
 * @return 命令指针
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
char* GetCommand(char* pTo, const char* pCommand)
{
	char* p = pTo;
	char *q;

	while (*pCommand != BLANK && *pCommand != '\0') {
		*pTo++ = *pCommand++;
	}
	pCommand++;
	*pTo = '\0';
	q = pTo;

	if (!strcmp(p, "BINARY") || !strcmp(p, "LIGHT") || !strcmp(p, "CALC")) {
		*pTo++ = BLANK;
		while (*pCommand != BLANK && *pCommand != '\0') {
			*pTo++ = *pCommand++;
		}
		pCommand++;
		*pTo = '\0';
		if (strcmp(p, "BINARY ADJUST") && strcmp(p, "LIGHT ADJUST") && strcmp(p, "CALC CALIB")) {
			*q = '\0';
		}
	}

	return p;
}

/**
 * @brief 命令编号 取得处理
 *
 * 取得命令编号
 *
 * @param pCommand 命令
 * @return 命令编号
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
unsigned int GetCommandNo(char* pCommand)
{
	unsigned int iCommandNo = 0;
	unsigned int i;

	for (i = 0; i < CMD_NUM; i++) {
		if (!strncmp(pCommand, g_staCmdTable[i].aCmdName, CMDNAME_LENGTH)) {
			iCommandNo = i;
			break;
		}
	}

	return iCommandNo;
}


/**
 * @brief 命令类型的判断
 *
 * 判断命令类型
 *
 * @param uiCmdNo 命令ID
 * @return 1: 请求命令 2: 设定命令 3: 命令执行处理 4: Sequence执行处理   5: Test -1: Error
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int JudgeCmdType(uint32_t uiCmdNo)
{
	int iRetValue = ERROR_CMD;

	if (uiCmdNo != 0) {
		/*取得命令分类*/
		g_stCommand.uiCmdId = uiCmdNo;
		iRetValue = g_staCmdTable[uiCmdNo].uiCmdDetail;
	}

	return iRetValue;
}

/**
 * @brief 命令的语法异常判断 处理
 *
 * 判断 命令语法异常
 *
 * @param msg Message
 * @param uiCmdNo 命令ID
 * @return Error ID
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int JudgeCmdConstruct(char* pCommand, unsigned int uiCmdNo)
{
	int iRetValue = ERR_CMD_PARSE_ERROR;

	if (uiCmdNo != 0) {
		/*语法异常判断*/
		iRetValue = g_staCmdTable[uiCmdNo].check(pCommand);
	}

	return iRetValue;
}

/**
 * @brief 数值的判断
 *
 * 判断是否为数值
 *
 * @param in_pStr 字符串
 * @return True:是数值  False:不是数值
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
bool IsDigital(char* in_pStr)
{
	bool bResult = true;

	if (*in_pStr == '\0') {
		bResult = false;
	}
	else {
		if (*in_pStr == '-') {
			in_pStr++;
		}
		while (*in_pStr != '\0') {
			if (*in_pStr < '0' || *in_pStr > '9') {
				bResult = false;
				break;
			}
			in_pStr++;
		}
	}

	return bResult;
}

/**
 * @brief Float判断处理
 *
 * 判断是否是浮点数
 *
 * @param in_pStr 字符串
 * @return True:是浮点数 False:不是浮点数
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
bool IsFloatDigital(char* in_pStr)
{
	bool bResult = true;
	bool bDot = false;

	if (*in_pStr == '\0') {
		bResult = false;
	}
	else {
		if (*in_pStr == '-') {
			in_pStr++;
		}
		while (*in_pStr != '\0') {
			if (*in_pStr == '.' && !bDot) {
				bDot = true;
			}
			else if (*in_pStr < '0' || *in_pStr > '9') {
				bResult = false;
				break;
			}
			in_pStr++;
		}

		if (*(in_pStr-1) == '.') {
			bResult = false;
		}
	}

	return bResult && bDot;
}


/**
 * @brief 将IP地址转换为整数
 *
 * 将IP地址转换为整数
 *
 * @param in_pIp 字符串
 * @return -1:转换失败
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int IpToInt(char* in_pIp)
{
	int iResult = -1;
	int iIp_1, iIp_2, iIp_3, iIp_4;
	int iCount = 0;
	char ipAddr[IP_LENGTH];

	strncpy(ipAddr, in_pIp, IP_LENGTH);
	char *p = ipAddr;
	char *pIp = ipAddr;

	while (*pIp != '\0') {
		while (*pIp != '.' && *pIp != '\0') {
			pIp++;
		}
		*pIp = '\0';
		if (!IsDigital(p)) {
			iResult = -1;
			break;
		}
		else {
			++iCount;
			if (iCount == 1) {
				iIp_1 = atoi(p);
			}
			else if (iCount == 2) {
				iIp_2 = atoi(p);
			}
			else if (iCount == 3) {
				iIp_3 = atoi(p);
			}
			else if (iCount == 4) {
				iIp_4 = atoi(p);
			}
			else {
				iResult = -1;
				break;
			}
		}
		p = ++pIp;
	}

	if (iIp_1 > 255 || iIp_2 > 255 || iIp_3 > 255 || iIp_4 > 255) {
		iResult = -1;
	}
	else if (iIp_1 < 0 || iIp_2 < 0 || iIp_3 < 0 || iIp_4 < 0) {
		iResult = -1;
	}
	else if (iCount != 4) {
		iResult = -1;
	}
	else {
		iIp_1 = (iIp_1 << 24) & 0xFF000000;
		iIp_2 = (iIp_2 << 16) & 0x00FF0000;
		iIp_3 = (iIp_3 << 8) & 0x0000FF00;
		iIp_4 = iIp_4 & 0x000000FF;
		iResult = iIp_1 | iIp_2 | iIp_3 | iIp_4;
	}

	return iResult;
}

/**
 * @brief 将整数转换为IP地址
 *
 * 将整数转换为IP地址
 *
 * @param iIp 整数
 * @param out_pIp 转换结果（IP地址）
 * @return True:成功 False:失败
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
bool IntToIp(int iIp, char* out_pIp)
{
	bool bResult = true;

	int iIp_1, iIp_2, iIp_3, iIp_4;

	iIp_1 = (iIp >> 24) & 0x000000FF;
	iIp_2 = (iIp >> 16) & 0x000000FF;
	iIp_3 = (iIp >> 8) & 0x000000FF;
	iIp_4 = iIp & 0x000000FF;

	if (iIp_1 > 255 || iIp_2 > 255 || iIp_3 > 255 || iIp_4 > 255) {
		bResult = false;
	}
	else {
		sprintf(out_pIp, "%d.%d.%d.%d", iIp_1, iIp_2, iIp_3, iIp_4);
	}

	return bResult;
}

/**
 * @brief 子网掩码的判断
 *
 * 判断是否是子网掩码
 *
 * @param in_pMask 字符串
 * @return True:成功 False:失败
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
bool IsNetMask(char* in_pMask)
{
	bool bResult = true;
	int iNet_1, iNet_2, iNet_3, iNet_4;
	int iCount = 0;
	int i = -1;
	char netMask[IP_LENGTH];

	strncpy(netMask, in_pMask, IP_LENGTH);
	char *p = netMask;
	char *pNetMask = netMask;

	while (*pNetMask != '\0') {
		while (*pNetMask != '.' && *pNetMask != '\0') {
			pNetMask++;
		}
		*pNetMask = '\0';
		if (!IsDigital(p)) {
			bResult = false;
			break;
		}
		else {
			++iCount;
			if (iCount == 1) {
				iNet_1 = atoi(p);
			}
			else if (iCount == 2) {
				iNet_2 = atoi(p);
			}
			else if (iCount == 3) {
				iNet_3 = atoi(p);
			}
			else if (iCount == 4) {
				iNet_4 = atoi(p);
			}
			else {
				bResult = false;
				break;
			}
		}
		p = ++pNetMask;
	}

	if (iCount != 4) {
		bResult = false;
	}

	i = CheckMaskValue(iNet_1);
	if(i == -1 || (i == 0 && iNet_2 != 0)) {
		bResult = false;
	}

	i = CheckMaskValue(iNet_2);
	if(i == -1 || (i == 0 && iNet_3 != 0)) {
		bResult = false;
	}

	i = CheckMaskValue(iNet_3);
	if(i == -1 || (i == 0 && iNet_4 != 0)) {
		bResult = false;
	}

	i = CheckMaskValue(iNet_4);
	if(i == -1) {
		bResult = false;
	}

	return bResult;
}

/**
 * @brief 掩码值正确性的判断
 *
 * 判断是否是正确的掩码
 *
 * @param in_Value 字符串
 * @return -1:不一致 0:正确 1:255
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int CheckMaskValue(int in_Value)
{
	int iResult = 0;
	switch(in_Value)
	{
	case 255:
		iResult = 1;
		break;
	case 254:
	case 252:
	case 248:
	case 240:
	case 224:
	case 192:
	case 128:
	case 0:
		iResult = 0;
		break;
	default:
		iResult = -1;
		break;
	}

	return iResult;
}

/**
 * @brief 日期的判断
 *
 * 判断日期是不是正确的。YYYYMMDDHHmmSS
 *
 * @param pStr 字符串
 * @return True:正确 False:不正确
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
bool IsDate(char* in_pDate, int iLength)
{
	bool bResult = true;
	char sDate[DATE_HYPHEN_LENGHT + 1];
	char *p = sDate;
	int i = 1;
	int iYear, iMonth, iDay, iHour, iMinute, iSecond;
	bool bLeapYear = false;

	if (iLength != DATE_LENGHT) {
		bResult = false;
	}
	else {
		while (*in_pDate != '\0') {
			*p++ = *in_pDate++;

			if (i == 4 || i == 6 || i == 8 || i == 10 || i == 12) {
				/*YYYY埵抲4傑偱丄MM埵抲6傑偱丄DD埵抲8傑偱丄HH埵抲10傑偱丄mm埵抲12傑偱*/
				*p++ = HYPHEN;
			}
			i++;
		}
		*p = '\0';
		p = sDate;
		sscanf(p,"%d-%d-%d-%d-%d-%d",&iYear, &iMonth, &iDay, &iHour, &iMinute, &iSecond);

		if (iYear < MIN_YEAR || iYear > MAX_YEAR) bResult = false;
		if (iMonth < 1 || iMonth > 12) bResult = false;					/*寧偺斖埻1乣12*/
		if (iHour < 0 || iHour > 23) bResult = false;					/*帪偺斖埻0乣23*/
		if (iMinute < 0 || iMinute > 59) bResult = false;				/*暘偺斖埻0乣59*/
		if (iSecond < 0 || iSecond > 59) bResult = false;				/*昩偺斖埻0乣59*/

		if (bResult) {
			if((iYear % 4 == 0 && iYear % 100 != 0) || (iYear % 400 == 0)) {	/*塠擭傪寁嶼偡傞*/
				bLeapYear = true;
			}
			switch(iMonth) {
			case JANUARARY:
			case MARCH:
			case MAY:
			case JULY:
			case AUGUST:
			case OCTOBER:
			case DECEMBER:
				if (iDay < 1 || iDay > BIG_MONTH_DAY) bResult = false;
				break;
			case APRIL:
			case JUNE:
			case SEPTEMBER:
			case NOVEMBER:
				if (iDay < 1 || iDay > SMALL_MONTH_DAY) bResult = false;
				break;
			case FEBRUARY:
				if (!bLeapYear) {
					if (iDay < 1 || iDay > FEB_NO_LEAP) bResult = false;
				}
				else {
					if (iDay < 1 || iDay > FEB_LEAP) bResult = false;
				}
			} // switch
		}// bResult
	}

	if (bResult) {
		g_stSystemClock.iYear = iYear;
		g_stSystemClock.iMonth = iMonth;
		g_stSystemClock.iDay = iDay;
		g_stSystemClock.iHour = iHour;
		g_stSystemClock.iMinute = iMinute;
		g_stSystemClock.iSecond = iSecond;
	}

	return bResult;
}


/**
 * @brief 时间差的计算
 *
 * 计算时间差
 *
 * @param t1 结束时间
 * @param t2 开始时间
 * @return 时间差
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
int diffMs(struct timeval t1, struct timeval t2)
{
    return (((t1.tv_sec - t2.tv_sec) * 1000000) +
            (t1.tv_usec - t2.tv_usec));
}


/**
 * @brief 生成JSON对象(for image)
 *
 * 生成JSON对象
 *
 * @param in_pUserID 用户ID
 * @param in_IpAddr  设备IP
 * @param in_iCategory  类别
 * @return
 * @author wangwei
 * @date 2016/06/21 wangwei 初始版本
 */
const char* CreateImageJSONObj(char* in_pUserID, char * in_pMachine, char* in_IpAddr)
{
	const char *json = NULL;
	char dirName[FILENAME_LENGTH];
	char dirUserName[FILENAME_LENGTH];
	char dirOperName[FILENAME_LENGTH];
	char linkName[LINK_LENGTH];
	char linkTitle[LINK_LENGTH];
	char linkDeviceTitle[LINK_LENGTH];
	DIR *d, *md;
	struct dirent *file;

	json_object *jLinkStr;
	json_object *jArray = json_object_new_array();
	json_object * jobj = json_object_new_object();

	strcpy(dirName, APACHE_FOLDER);
	strcat(dirName, PHOTO_FOLDER);

	// dirName: 设备照片视频存放目录	dirUserName：移动设备视频照片存放目录
	strncpy(dirUserName, dirName, FILENAME_LENGTH);
	strncat(dirName, in_pMachine, FILENAME_LENGTH);
//	strncat(dirName, originAddFolder, FILENAME_LENGTH);
	strncat(dirUserName, in_pUserID, FILENAME_LENGTH);
//	strncat(dirUserName, originAddFolder, FILENAME_LENGTH);

	/*Creating a json string*/
	json_object *jMachineStr = json_object_new_string(in_pMachine);
	json_object *jDescStr = json_object_new_string(in_pUserID);
	json_object *jGenerateStr = json_object_new_string("http://www.ubky.com.cn");

	json_object_object_add(jobj, "device", jMachineStr);
	json_object_object_add(jobj, "user", jDescStr);
	json_object_object_add(jobj, "generator", jGenerateStr);

	strcpy(linkName, "http://");
	strcat(linkName, in_IpAddr);
	strcat(linkName, "/html/");
	strcat(linkName, PHOTO_FOLDER);

	strcpy(linkDeviceTitle, linkName);
	strcat(linkDeviceTitle, in_pMachine);
	strcat(linkDeviceTitle, "/");

	strcat(linkName, in_pUserID);
	strcat(linkName, "/");
	strcpy(linkTitle, linkName);

	// Device dir
	// origin
	strncpy(dirOperName, dirName, COMMAND_LENGTH);
	strncat(dirOperName, ORIGIN_FOLDER, COMMAND_LENGTH);
	if(!(md = opendir(dirOperName))) {
		g_TongueCtrlLog.E("Error opendir %s for json.\n", dirName);
	}
	else {
		while((file = readdir(md)) != NULL) {
			if(strncmp(file->d_name, ".", 1) == 0) {
				continue;
			}
			strcpy(linkName, linkDeviceTitle);
			strncat(linkName, ORI_DIR_NOSLASH, LINK_LENGTH);
			strcat(linkName, file->d_name);
			jLinkStr = json_object_new_string(linkName);
			json_object_array_add(jArray, jLinkStr);
		}
		closedir(md);
	}
	// tongue
	strncpy(dirOperName, dirName, COMMAND_LENGTH);
	if ((g_photoCategory == ORIGIN_TONGUE) || (g_photoCategory == ORIGIN_TONGUE_TONGUE_LIP)) {
		strncat(dirOperName, TONGUE_FOLDER, COMMAND_LENGTH);
		if(!(md = opendir(dirOperName))) {
			g_TongueCtrlLog.E("Error opendir %s for json.\n", dirName);
		}
		else {
			while((file = readdir(md)) != NULL) {
				if(strncmp(file->d_name, ".", 1) == 0) {
					continue;
				}
				strcpy(linkName, linkDeviceTitle);
				strncat(linkName, TON_DIR_NOSLASH, LINK_LENGTH);
				strcat(linkName, file->d_name);
				jLinkStr = json_object_new_string(linkName);
				json_object_array_add(jArray, jLinkStr);
			}
			closedir(md);
		}
	}
	// tongue and lip
	strncpy(dirOperName, dirName, COMMAND_LENGTH);
	if ((g_photoCategory == ORIGIN_TONGUE_LIP) || (g_photoCategory == ORIGIN_TONGUE_TONGUE_LIP)) {
		strncat(dirOperName, TONGUE_LIP_FOLDER, COMMAND_LENGTH);
		if(!(md = opendir(dirOperName))) {
			g_TongueCtrlLog.E("Error opendir %s for json.\n", dirName);
		}
		else {
			while((file = readdir(md)) != NULL) {
				if(strncmp(file->d_name, ".", 1) == 0) {
					continue;
				}
				strcpy(linkName, linkDeviceTitle);
				strncat(linkName, TON_LIP_DIR_NOSLASH, LINK_LENGTH);
				strcat(linkName, file->d_name);
				jLinkStr = json_object_new_string(linkName);
				json_object_array_add(jArray, jLinkStr);
			}
			closedir(md);
		}
	}


	// Mobile dir
	// origin
	strncpy(dirOperName, dirUserName, COMMAND_LENGTH);
	strncat(dirOperName, ORIGIN_FOLDER, COMMAND_LENGTH);
	if(!(d = opendir(dirOperName))) {
		g_TongueCtrlLog.E("Error opendir %s for json.\n", dirUserName);
	}
	else {
		while((file = readdir(d)) != NULL) {
			if(strncmp(file->d_name, ".", 1) == 0) {
				continue;
			}
			strcpy(linkName, linkTitle);
			strncat(linkName, ORI_DIR_NOSLASH, LINK_LENGTH);
			strcat(linkName, file->d_name);
			jLinkStr = json_object_new_string(linkName);
			json_object_array_add(jArray, jLinkStr);
		}
		closedir(md);
	}

	// tongue
	strncpy(dirOperName, dirUserName, COMMAND_LENGTH);
	if ((g_photoCategory == ORIGIN_TONGUE) || (g_photoCategory == ORIGIN_TONGUE_TONGUE_LIP)) {
		strncat(dirOperName, TONGUE_FOLDER, COMMAND_LENGTH);
		if(!(d = opendir(dirOperName))) {
			g_TongueCtrlLog.E("Error opendir %s for tongue json.\n", dirUserName);
		}
		else {
			while((file = readdir(d)) != NULL) {
				if(strncmp(file->d_name, ".", 1) == 0) {
					continue;
				}
				strcpy(linkName, linkTitle);
				strncat(linkName, TON_DIR_NOSLASH, LINK_LENGTH);
				strcat(linkName, file->d_name);
				jLinkStr = json_object_new_string(linkName);
				json_object_array_add(jArray, jLinkStr);
			}
			closedir(md);
		}
	}
	// tongue and lip
	strncpy(dirOperName, dirUserName, COMMAND_LENGTH);
	if ((g_photoCategory == ORIGIN_TONGUE_LIP) || (g_photoCategory == ORIGIN_TONGUE_TONGUE_LIP)) {
		strncat(dirOperName, TONGUE_LIP_FOLDER, COMMAND_LENGTH);
		if(!(d = opendir(dirOperName))) {
			g_TongueCtrlLog.E("Error opendir %s for tongue lip json.\n", dirUserName);
		}
		else {
			while((file = readdir(d)) != NULL) {
				if(strncmp(file->d_name, ".", 1) == 0) {
					continue;
				}
				strcpy(linkName, linkTitle);
				strncat(linkName, TON_LIP_DIR_NOSLASH, LINK_LENGTH);
				strcat(linkName, file->d_name);
				jLinkStr = json_object_new_string(linkName);
				json_object_array_add(jArray, jLinkStr);
			}
			closedir(md);
		}
	}

	json_object_object_add(jobj, "items", jArray);
	json = json_object_to_json_string(jobj);


	return json;
}

/**
 * @brief 生成JSON对象(for video)
 *
 * 生成JSON对象
 *
 * @param in_pUserID 用户ID
 * @param in_IpAddr  设备IP
 * @param in_iCategory  类别
 * @return
 * @author wangwei
 * @date 2017/08/29 wangwei 初始版本
 */
const char* CreateVideoJSONObj(char* in_pUserID, char * in_pMachine, char* in_IpAddr)
{
	const char *json = NULL;
	char dirName[FILENAME_LENGTH];
	char dirUserName[FILENAME_LENGTH];
	char linkName[LINK_LENGTH];
	char linkTitle[LINK_LENGTH];
	char linkDeviceTitle[LINK_LENGTH];
	DIR *d, *md;
	struct dirent *file;

	json_object *jLinkStr;
	json_object *jArray = json_object_new_array();;
	json_object * jobj = json_object_new_object();



	strcpy(dirName, APACHE_FOLDER);
	strcat(dirName, VIDEO_FOLDER);


	strcpy(dirUserName, dirName);
	strcat(dirName, in_pMachine);
	strcat(dirUserName, in_pUserID);

	/*Creating a json string*/
	json_object *jMachineStr = json_object_new_string(in_pMachine);
	json_object *jDescStr = json_object_new_string(in_pUserID);
	json_object *jGenerateStr = json_object_new_string("http://www.ubky.com.cn");

	json_object_object_add(jobj, "device", jMachineStr);
	json_object_object_add(jobj, "user", jDescStr);
	json_object_object_add(jobj, "generator", jGenerateStr);

	strcpy(linkName, "http://");
	strcat(linkName, in_IpAddr);
	strcat(linkName, "/html/");
	strcat(linkName, VIDEO_FOLDER);

	strcpy(linkDeviceTitle, linkName);
	strcat(linkDeviceTitle, in_pMachine);
	strcat(linkDeviceTitle, "/");

	strcat(linkName, in_pUserID);
	strcat(linkName, "/");
	strcpy(linkTitle, linkName);


	// Device dir
	if(!(md = opendir(dirName))) {
		g_TongueCtrlLog.E("Error opendir %s for json.\n", dirName);
	}
	else {
		while((file = readdir(md)) != NULL) {
			if(strncmp(file->d_name, ".", 1) == 0) {
				continue;
			}
			strcpy(linkName, linkDeviceTitle);
			strcat(linkName, file->d_name);
			jLinkStr = json_object_new_string(linkName);
			json_object_array_add(jArray, jLinkStr);
		}
	}

	// Mobile dir
	if(!(d = opendir(dirUserName))) {
		g_TongueCtrlLog.E("Error opendir %s for json.\n", dirUserName);
	}
	else {
		while((file = readdir(d)) != NULL) {
			if(strncmp(file->d_name, ".", 1) == 0) {
				continue;
			}
			strcpy(linkName, linkTitle);
			strcat(linkName, file->d_name);
			jLinkStr = json_object_new_string(linkName);
			json_object_array_add(jArray, jLinkStr);
		}
	}


	json_object_object_add(jobj, "items", jArray);
	json = json_object_to_json_string(jobj);


	return json;
}

