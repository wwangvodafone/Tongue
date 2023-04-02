/*****************************************************************************/
/**
* @file TongueCtrlCmdRun.h
* @brief 日志线程定义
*
* 日志线程定义
*
* @author wangwei
* @date 2016/06/06 wangwei 初始版本
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/

#ifndef TongueCTRLLOG_H_
#define TongueCTRLLOG_H_

#define 	LOG_TEXT_LENGTH   		64										// 日志字符串部分长度
#define 	LOG_TIME_LENGTH			17										// 日志时刻部分长度
#define 	DEBUG_LOG_LENGTH		(9+LOG_TIME_LENGTH+LOG_TEXT_LENGTH)	    // 日志长度
#define		LOG_ITEM_COUNT_MAX		5000									// 文件中日志最大数目
#define 	LOG_FILE_NAME_LENGTH	(512+LOG_TIME_LENGTH)					// 日志文件名的长度

/** @brief 儘僌庬椶楍嫇宆 */
typedef enum _LogType_En_tag_
{
	/** @brief 日志种类:Information */
	LOG_TYPE_INFO = 0,
	/** @brief 日志种类:Error */
	LOG_TYPE_ERR,
	/** @brief 日志种类:Debug */
	LOG_TYPE_DBG,
	/** @brief 日志种别数 */
	LOG_TYPE_NUM
} LogType_En;

/** @brief TongueCtrlLog */
typedef struct _TongueCtrlLog_St_tag_
{
	/** @brief information日志*/
	void (*I)(const char *in_pLogStr);
	/** @brief Debug日志 */
	void (*D)(const char *fmt, ...);
	/** @brief Error日志 */
	void (*E)(const char *fmt, ...);
} TongueCtrlLog_St;

/** @brief 日期时间结构体 */
typedef struct _Datatime_St_tag
{
	/** @brief 年(1900~9999) */
	int	ulYear;
	/** @brief 月(1~12) */
	int	ulMonth;
	/** @brief 日(1~31) */
	int	ulDay;
	/** @brief 时(0~23) */
	int	ulHour;
	/** @brief 分(0~59) */
	int	ulMinute;
	/** @brief 秒(0~59) */
	int	ulSecond;
	/** @brief 毫秒(0~999) */
	int	ulMSecond;
} Datetime_St;

/** @brief 日志项目结构体 */
typedef struct _LogItem_St_tag
{
	/** @brief 日期时间 */
	Datetime_St	stDatetime;

	/** @brief 日志种类(LOG_TYPE_XXX) */
	LogType_En	ulLogType;

	/** @brief 日志字符串*/
	char		pLogMsg[LOG_TEXT_LENGTH];
} LogItem_St;

/** @brief TongueCtrl的information日志项目登录处理 */
extern void TongueCtrlInformationLog(const char *in_pLogSt);

/** @brief TongueCtrl的Debug日志项目登录处理*/
extern void TongueCtrlDebugLog(const char *fmt, ...);

/** @brief TongueCtrl的Error日志项目登录处理 */
extern void TongueCtrlErrorLog(const char *fmt, ...);

/**
 * @brief TongueCtrl日志项目登录处理 */
extern void SaveTongueCtrlLogItem(LogType_En e_ulLogType, char *in_pLogStr);

/** @brief 日志输出处理*/
extern void LogOut(LogItem_St *in_pstLogItem);

/** @brief 日志文件名设定*/
extern bool SetLogFileName(Datetime_St* in_pstDatetime);

/** @brief 日志文件保存 */
extern void* LogFileSaveFunc(void* ptr);

/** @brief TongueCtrlLog */
extern TongueCtrlLog_St	g_TongueCtrlLog;

#endif /* TongueCTRLLOG_H_ */
