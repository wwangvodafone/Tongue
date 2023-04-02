/*****************************************************************************/
/**
* @file TongueCtrlCmdRun.cpp
* @brief ��־�߳�
*
* ��־�߳�
*
* @author wangwei
* @date 2016/06/06 wangwei ��ʼ�汾
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

char	g_cFileName[LOG_FILE_NAME_LENGTH];					// Log�ļ���
int		g_nLogItemCnt = LOG_ITEM_COUNT_MAX;					// ��־����
std::queue<LogItem_St> LogMsgQueue;
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t g_cond = PTHREAD_COND_INITIALIZER;

/** @brief TongueCtrlLog */
TongueCtrlLog_St	g_TongueCtrlLog = {
		/** @brief information��־*/
		TongueCtrlInformationLog,
		/** @brief Debug��־*/
		TongueCtrlDebugLog,
		/** @brief Error��־*/
		TongueCtrlErrorLog
};

/**
 * @brief TongueCtrl��information��־��Ŀ��¼����
 *
 * TongueCtrl��information��־��Ŀ��¼����
 *
 * @param in_ulLogLevel ��־Level
 * @param in_ulLogCode ��־Code
 * @param in_pLogStr ��־�ַ���
 * @return ��
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
void TongueCtrlInformationLog(const char *in_pLogSt)
{
	char* buf =const_cast<char*>(in_pLogSt);

	// Information��־��¼
	SaveTongueCtrlLogItem(LOG_TYPE_INFO, buf);
	return;
}

/**
 * @brief TongueCtrl��Debut��־��Ŀ��¼����
 *
 * TongueCtrl��Debug��־��Ŀ��¼����
 *
 * @param in_ulLogLevel ��־Level
 * @param in_ulLogCode ��־Code
 * @param in_pLogStr ��־�ַ���
 * @return ��
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
void TongueCtrlDebugLog(const char *fmt, ...)
{

	va_list ap;
	char buf[LOG_TEXT_LENGTH];

	va_start(ap, fmt);
	vsnprintf(buf, LOG_TEXT_LENGTH, fmt, ap);
	va_end(ap);

	// Debug���O��o�^����B
	SaveTongueCtrlLogItem(LOG_TYPE_DBG, buf);

	return;
}

/**
 * @brief TongueCtrl��Error��־��Ŀ��¼����
 *
 * TongueCtrl��Error��־��Ŀ��¼����
 *
 * @param in_ulLogLevel ��־Level
 * @param in_ulLogCode ��־Code
 * @param in_pLogStr ��־�ַ���
 * @return ��
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
void TongueCtrlErrorLog(const char *fmt, ...)
{

	va_list ap;
	char buf[LOG_TEXT_LENGTH];

	va_start(ap, fmt);
	vsnprintf(buf, LOG_TEXT_LENGTH, fmt, ap);
	va_end(ap);

	// Error���O��o�^����B
	SaveTongueCtrlLogItem(LOG_TYPE_ERR, buf);

	return;
}

/**
 * @brief TongueCtrl����־��Ŀ��¼����
 *
 * TongueCtrl����־��Ŀ��¼����
 *
 * @param e_ulLogType ��־����
 * @param in_ulLogLevel ��־Level
 * @param in_ulLogCode ��־Code
 * @param in_pLogStr ��־�ַ���
 * @return ��
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
void SaveTongueCtrlLogItem(LogType_En e_ulLogType, char *in_pLogStr)
{
	int iStrlen;
    struct tm *p_sTime;
    struct timeval tv;
    LogItem_St stLogMsg;


    gettimeofday(&tv,NULL);

    p_sTime = localtime(&tv.tv_sec); /* ʱ��ת��*/

	// ʱ���趨
    stLogMsg.stDatetime.ulYear = p_sTime->tm_year + 1900;
	stLogMsg.stDatetime.ulMonth = p_sTime->tm_mon + 1;
	stLogMsg.stDatetime.ulDay = p_sTime->tm_mday;
	stLogMsg.stDatetime.ulHour = p_sTime->tm_hour;
	stLogMsg.stDatetime.ulMinute = p_sTime->tm_min;
	stLogMsg.stDatetime.ulSecond = p_sTime->tm_sec;
	stLogMsg.stDatetime.ulMSecond = (tv.tv_usec/1000);

	// ��־����
	stLogMsg.ulLogType = e_ulLogType;

	// ��־�ַ���
	iStrlen = strlen(in_pLogStr);
	if(iStrlen >= LOG_TEXT_LENGTH)
	{
		iStrlen = LOG_TEXT_LENGTH;
	}
	strncpy(stLogMsg.pLogMsg, in_pLogStr, iStrlen);

	// ���������ͨ�����˵�Code
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
 * @brief ��־���������
 *
 * ��־���������
 *
 * @param in_pstLogItem ��־��Ŀ
 * @return ��
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
 */
void LogOut(LogItem_St *in_pstLogItem)
{
	FILE *fp;
	char cLog[DEBUG_LOG_LENGTH];
	char cLogType;

	// �Ƿ񳬹�����־����󱣴���
	if (g_nLogItemCnt >= LOG_ITEM_COUNT_MAX)
	{
		if(SetLogFileName(&(in_pstLogItem->stDatetime)))
		{
			// ����������������µ��ļ�
			fp = fopen(g_cFileName, "a");
		}
		else
		{
			// ���û������׷��
		    fp = fopen(g_cFileName, "w");
		}
	}
	else
	{
		// û������׷��
	    fp = fopen(g_cFileName, "a");
	}

	// �趨��־����
	switch(in_pstLogItem->ulLogType)
	{
	//information��־
	case LOG_TYPE_INFO:
		cLogType = 'I';
		break;
	//Error��־
	case LOG_TYPE_ERR:
		cLogType = 'E';
		break;
	//Debug��־
	case LOG_TYPE_DBG:
		cLogType = 'D';
		break;
	default:
		break;
	}

	// ������־����
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

	// �ƶ�����һ��д���λ��
	g_nLogItemCnt++;

	fclose(fp);

	return ;
}

/**
 * @brief �����־�ļ��Ƿ����
 *
 * �����־�ļ��Ƿ����
 *
 * @param  in_pstDatetime ����ʱ��ṹ��
 * @return ��
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
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

	// ����ļ��Ƿ����
	if(stat(strFolderName, &buf) != 0)
	{
		// �����ھʹ���
		mkdir(strFolderName, 0775);
	}

	while(true)
	{
		strncpy(g_cFileName, strFolderName, FILE_MAX_LENGTH);
		// �趨��־�ļ���
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
 * @brief ��־�ļ�����
 *
 * ��־�ļ�����
 *
 * @param
 * @author wangwei
 * @date 2016/06/06 wangwei ��ʼ�汾
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
			// �����ͷ��Ԫ��
			LogOut(&(LogMsgQueue.front()));

			// ɾ����ͷ��Ԫ��
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

