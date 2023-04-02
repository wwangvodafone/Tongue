/*****************************************************************************/
/**
* @file TongueCtrlCmdRun.cpp
* @brief 日志线程
*
* 日志线程
*
* @author wangwei
* @date 2016/06/06 wangwei 初始版本
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <queue>

#include <time.h>
//#include <semaphore.h>
#include <pthread.h>

#include "sysexits.h"

#include "TongueCtrlLogThd.h"
#include "TongueCtrlCommand.h"
#include "TongueCtrl.h"

char	g_cFileName[LOG_FILE_NAME_LENGTH];					// Log文件名
int		g_nLogItemCnt = LOG_ITEM_COUNT_MAX;					// 日志行数
std::queue<LogItem_St> LogMsgQueue;
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t g_cond = PTHREAD_COND_INITIALIZER;

/** @brief TongueCtrlLog */
TongueCtrlLog_St	g_TongueCtrlLog = {
		/** @brief information日志*/
		TongueCtrlInformationLog,
		/** @brief Debug日志*/
		TongueCtrlDebugLog,
		/** @brief Error日志*/
		TongueCtrlErrorLog
};

/**
 * @brief TongueCtrl的information日志项目登录处理
 *
 * TongueCtrl的information日志项目登录处理
 *
 * @param in_ulLogLevel 日志Level
 * @param in_ulLogCode 日志Code
 * @param in_pLogStr 日志字符串
 * @return 无
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
void TongueCtrlInformationLog(const char *in_pLogSt)
{
	char* buf =const_cast<char*>(in_pLogSt);

	// Information日志登录
	SaveTongueCtrlLogItem(LOG_TYPE_INFO, buf);
	return;
}

/**
 * @brief TongueCtrl的Debut日志项目登录处理
 *
 * TongueCtrl的Debug日志项目登录处理
 *
 * @param in_ulLogLevel 日志Level
 * @param in_ulLogCode 日志Code
 * @param in_pLogStr 日志字符串
 * @return 无
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
void TongueCtrlDebugLog(const char *fmt, ...)
{

	va_list ap;
	char buf[LOG_TEXT_LENGTH];

	va_start(ap, fmt);
	vsnprintf(buf, LOG_TEXT_LENGTH, fmt, ap);
	va_end(ap);

	// Debug儘僌傪搊榐偡傞丅
	SaveTongueCtrlLogItem(LOG_TYPE_DBG, buf);

	return;
}

/**
 * @brief TongueCtrl的Error日志项目登录处理
 *
 * TongueCtrl的Error日志项目登录处理
 *
 * @param in_ulLogLevel 日志Level
 * @param in_ulLogCode 日志Code
 * @param in_pLogStr 日志字符串
 * @return 无
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
void TongueCtrlErrorLog(const char *fmt, ...)
{

	va_list ap;
	char buf[LOG_TEXT_LENGTH];

	va_start(ap, fmt);
	vsnprintf(buf, LOG_TEXT_LENGTH, fmt, ap);
	va_end(ap);

	// Error儘僌傪搊榐偡傞丅
	SaveTongueCtrlLogItem(LOG_TYPE_ERR, buf);

	return;
}

/**
 * @brief TongueCtrl的日志项目登录处理
 *
 * TongueCtrl的日志项目登录处理
 *
 * @param e_ulLogType 日志种类
 * @param in_ulLogLevel 日志Level
 * @param in_ulLogCode 日志Code
 * @param in_pLogStr 日志字符串
 * @return 无
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
void SaveTongueCtrlLogItem(LogType_En e_ulLogType, char *in_pLogStr)
{
	int iStrlen;
    struct tm *p_sTime;
    struct timeval tv;
    LogItem_St stLogMsg;


    gettimeofday(&tv,NULL);

    p_sTime = localtime(&tv.tv_sec); /* 时间转换*/

	// 时间设定
    stLogMsg.stDatetime.ulYear = p_sTime->tm_year + 1900;
	stLogMsg.stDatetime.ulMonth = p_sTime->tm_mon + 1;
	stLogMsg.stDatetime.ulDay = p_sTime->tm_mday;
	stLogMsg.stDatetime.ulHour = p_sTime->tm_hour;
	stLogMsg.stDatetime.ulMinute = p_sTime->tm_min;
	stLogMsg.stDatetime.ulSecond = p_sTime->tm_sec;
	stLogMsg.stDatetime.ulMSecond = (tv.tv_usec/1000);

	// 日志种类
	stLogMsg.ulLogType = e_ulLogType;

	// 日志字符串
	iStrlen = strlen(in_pLogStr);
	if(iStrlen >= LOG_TEXT_LENGTH)
	{
		iStrlen = LOG_TEXT_LENGTH;
	}
	strncpy(stLogMsg.pLogMsg, in_pLogStr, iStrlen);

	// 不输出串口通信终了的Code
	if(iStrlen > 0 && (stLogMsg.pLogMsg[iStrlen-1] == '\r' || stLogMsg.pLogMsg[iStrlen-1] == '\n' )){
		iStrlen--;
	}
	stLogMsg.pLogMsg[iStrlen] = '\0';

	pthread_mutex_lock(&g_mutex);

	LogMsgQueue.push(stLogMsg);

	pthread_mutex_unlock(&g_mutex);
	pthread_cond_signal(&g_cond);

	return;
}

/**
 * @brief 日志的输出处理
 *
 * 日志的输出处理
 *
 * @param in_pstLogItem 日志项目
 * @return 无
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
void LogOut(LogItem_St *in_pstLogItem)
{
	FILE *fp;
	char cLog[DEBUG_LOG_LENGTH];
	char cLogType;

	// 是否超过了日志的最大保存数
	if (g_nLogItemCnt >= LOG_ITEM_COUNT_MAX)
	{
		if(SetLogFileName(&(in_pstLogItem->stDatetime)))
		{
			// 如果超过了则生成新的文件
			fp = fopen(g_cFileName, "a");
		}
		else
		{
			// 如果没超过则追加
		    fp = fopen(g_cFileName, "w");
		}
	}
	else
	{
		// 没超过则追加
	    fp = fopen(g_cFileName, "a");
	}

	// 设定日志各类
	switch(in_pstLogItem->ulLogType)
	{
	//information日志
	case LOG_TYPE_INFO:
		cLogType = 'I';
		break;
	//Error日志
	case LOG_TYPE_ERR:
		cLogType = 'E';
		break;
	//Debug日志
	case LOG_TYPE_DBG:
		cLogType = 'D';
		break;
	default:
		break;
	}

	// 设置日志内容
	sprintf(cLog,"%c %d/%02d/%02d %02d:%02d:%02d:%03d %s\n",
			cLogType,
			in_pstLogItem->stDatetime.ulYear,
			in_pstLogItem->stDatetime.ulMonth,
			in_pstLogItem->stDatetime.ulDay,
			in_pstLogItem->stDatetime.ulHour,
			in_pstLogItem->stDatetime.ulMinute,
			in_pstLogItem->stDatetime.ulSecond,
			in_pstLogItem->stDatetime.ulMSecond,
			in_pstLogItem->pLogMsg
			);

	fputs(cLog, fp);

	// 移动到下一个写入的位置
	g_nLogItemCnt++;

	fclose(fp);

	return ;
}

/**
 * @brief 检测日志文件是否存在
 *
 * 检测日志文件是否存在
 *
 * @param  in_pstDatetime 日期时间结构体
 * @return 无
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
bool SetLogFileName(Datetime_St* in_pstDatetime)
{
	FILE* fp;
	struct stat buf;
	int iFileCount = 0;
	char c[DEBUG_LOG_LENGTH];
	char strFolderName[FILE_MAX_LENGTH];
	char strFileName[FILE_MAX_LENGTH];

	strncpy(strFolderName, APACHE_FOLDER, FILE_MAX_LENGTH);
	strncat(strFolderName, LOG_FOLDER, FILE_MAX_LENGTH);

	// 检测文件是否存在
	if(stat(strFolderName, &buf) != 0)
	{
		// 不存在就创建
		mkdir(strFolderName, 0775);
	}

	while(true)
	{
		strncpy(g_cFileName, strFolderName, FILE_MAX_LENGTH);
		// 设定日志文件名
		sprintf(strFileName,"%d%02d%02d_%04d.log",
				in_pstDatetime->ulYear,
				in_pstDatetime->ulMonth,
				in_pstDatetime->ulDay,
				iFileCount);
		strncat(g_cFileName, strFileName, FILE_MAX_LENGTH);
		fp = fopen(g_cFileName, "r");
		if(fp == NULL)
		{
			break;
		}

		fclose(fp);
		iFileCount++;
	}

	if(iFileCount == 0)
	{
		return false;
	}

	iFileCount--;
	strncpy(g_cFileName, strFolderName, FILE_MAX_LENGTH);
	sprintf(strFileName,"%d%02d%02d_%04d.log",
					in_pstDatetime->ulYear,
					in_pstDatetime->ulMonth,
					in_pstDatetime->ulDay,
					iFileCount);

	g_nLogItemCnt = 0;
	strncat(g_cFileName, strFileName, FILE_MAX_LENGTH);
	fp = fopen(g_cFileName, "r");
	while(((c[0] = fgetc(fp)) != EOF) && ((c[0] = fgetc(fp)) != 255))
	{
		fgets(c, DEBUG_LOG_LENGTH, fp);
		g_nLogItemCnt++;
	}

	fclose(fp);

	if (g_nLogItemCnt >= LOG_ITEM_COUNT_MAX)
	{
		iFileCount++;
		strncpy(g_cFileName, strFolderName, FILE_MAX_LENGTH);
		sprintf(strFileName,"%d%02d%02d_%04d.log",
						in_pstDatetime->ulYear,
						in_pstDatetime->ulMonth,
						in_pstDatetime->ulDay,
						iFileCount);
		strncat(g_cFileName, strFileName, FILE_MAX_LENGTH);
	}

	return true;
}

/**
 * @brief 日志文件保存
 *
 * 日志文件保存
 *
 * @param
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 */
void* LogFileSaveFunc(void* ptr)
{
	while(true)
	{
		pthread_mutex_lock(&g_mutex);
		if(LogMsgQueue.empty() == true)
		{
			pthread_cond_wait(&g_cond, &g_mutex);
		}

		while(true)
		{
			// 到得最开头的元素
			LogOut(&(LogMsgQueue.front()));

			// 删除开头的元素
			LogMsgQueue.pop();

			if(LogMsgQueue.empty() == true)
			{
				break;
			}
		}

		pthread_mutex_unlock(&g_mutex);
	}

	return (void*)0;
}

