/*****************************************************************************/
/**
* @file TongueCtrlCommon.cpp
* @brief 舌苔控制命令共有变量涵 数
*
* 舌苔控制命令共有变量涵 数
*
* @author wangwei
* @date 2016/06/06 wangwei 初始版本
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/timeb.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <arpa/inet.h>

#include "TongueCtrl.h"
#include "TongueCtrlCommand.h"
#include "TongueCtrlParameter.h"
#include "TongueCtrlLogThd.h"
#include "TongueCtrlErrProc.h"

/*日期时间字符串*/
char g_strDateTime[DATETIME_LENGTH];


/**
 * @brief 从config文件读出全部的参数
 *
 * 从config文件读出全部的参数
 *
 * @param in_paramCnt 参数的最大数目
 * @param in_pValue 存储设定值的最开始地址
 * @return 0:正常  0以外:异常
 * @author wangwei
 * @date 2015/06/12 wangwei 初始版本
 */
int ReadParametersAll(int in_paramCnt, NumericType * in_pValue)
{

	g_TongueCtrlLog.D("ReadParametersAll Start(return = %d).", in_paramCnt);

	int iResult = -1;
	FILE *fp;
	char input[MAX_LINE_LENGTH];
	int len = 0;
	int i = 1;
	unsigned int iParam = i;

	in_pValue++;
	fp = fopen(STORE_FILE, "r");

	if (fp != NULL) {
		while (fgets(input, MAX_LINE_LENGTH, fp)) {
			if (input[0] == '#') {
				continue;
			}
	        len = strlen(input);
	        if (len < 2) {
	            continue;
	        }
	        if (input[len-1] == '\n') {
	        	input[len-1] = '\0';
	        }
	        char *p = input;
	        while ((*p != '=') && (*p != '\0')) {
	        	p++;
	        }
	        if (*p++ != '\0') {
//	        	*in_pValue = *p;

	        }
	    	// g_systemParamMap数组的下标计算
	    	unsigned int uiMapNo = GetParamIdForSetting(iParam);
			DATATYPE_En dataType = g_systemParamMap[uiMapNo].enDataType;
	        if (dataType == FLOAT_TYPE) {
	        	in_pValue->fValue = atof((const char*)p);
	        	g_TongueCtrlLog.D("dataType = FLOAT_TYPE Value = %d.", in_pValue->fValue);

	        }
	        else if (dataType == INT_TYPE) {
	        	in_pValue->iValue = atoi((const char*)p);
	        	g_TongueCtrlLog.D("dataType = INT_TYPE Value = %d.", in_pValue->iValue);
	        }

	        in_pValue++;
	        iParam = ++i;
		}

		fclose(fp);
		iResult = 0;
	}

	g_TongueCtrlLog.D("ReadParametersAll End(return = %d).", iResult);

	return iResult;
}

/**
 * @brief 向Config文件里写入全部的参数
 *
 * @param in_paramCnt 参数的最大数目
 * @param in_pValue 存储设定值的最开始地址
 * @return 0:正常  0以外:异常
 * @author wangwei
 * @date 2015/06/12 wangwei 初始版本
 */
int WriteParametersAll(int in_paramCnt, NumericType * in_pValue)
{
	g_TongueCtrlLog.D("WriteParametersAll Start(in_paramId = %d).", in_paramCnt);

	int iResult = -1;
	FILE *fp;
	unsigned int iParam;

	fp = fopen(STORE_FILE, "w");

	if (fp != NULL) {
		for (int i = 0; i < in_paramCnt; i++) {
			iParam = i;
			// g_systemParamMap攝楍偺揧偊帤傪寁嶼
			unsigned int uiMapNo = GetParamIdForSetting(iParam);
			DATATYPE_En dataType = g_systemParamMap[uiMapNo].enDataType;
			if (dataType == INT_TYPE) {
				fprintf(fp,"%d=%d\n", i, (in_pValue + i)->iValue);
	        	g_TongueCtrlLog.D("dataType = INT_TYPE Value = %d.", in_pValue->iValue);
			}
			else if (dataType == FLOAT_TYPE) {
				fprintf(fp,"%d=%f\n", i, (in_pValue + i)->fValue);
	        	g_TongueCtrlLog.D("dataType = FLOAT_TYPE Value = %d.", in_pValue->fValue);
			}
		}
		iResult = 0;
		fclose(fp);
	}

	g_TongueCtrlLog.D("WriteParametersAll End(return = %d).", iResult);

	return iResult;
}

/**
 * @brief 向Config文件写入参数
 *
 * 向Config文件写入参数
 *
 * @param in_paramCnt 参数的最大数目
 * @param in_pValue 存储设定值的最开始地址
 * @return 0:正常  0以外:异常
 * @author wangwei
 * @date 2015/06/12 wangwei 初始版本
 */
int WriteParameter(int in_paramCnt, NumericType * in_pValue)
{
	g_TongueCtrlLog.D("WriteParametersAll Start(in_paramId = %d).", in_paramCnt);

	int iResult = -1;
	FILE *fp;
	unsigned int iParam;

	fp = fopen(STORE_FILE, "w");

	if (fp != NULL) {
		for (int i = 0; i < in_paramCnt; i++) {
			iParam = i;
			// g_systemParamMap数组的下标计算
			unsigned int uiMapNo = GetParamIdForSetting(iParam);
			DATATYPE_En dataType = g_systemParamMap[uiMapNo].enDataType;
			if (dataType == INT_TYPE) {
				fprintf(fp,"%d=%d\n", i, (in_pValue + i)->iValue);
	        	g_TongueCtrlLog.D("dataType = INT_TYPE Value = %d.", in_pValue->iValue);
			}
			else if (dataType == FLOAT_TYPE) {
				fprintf(fp,"%d=%f\n", i, (in_pValue + i)->fValue);
	        	g_TongueCtrlLog.D("dataType = FLOAT_TYPE Value = %d.", in_pValue->fValue);
			}
		}
		iResult = 0;
		fclose(fp);
	}

	g_TongueCtrlLog.D("WriteParametersAll End(return = %d).", iResult);

	return iResult;
}


/**
 * @brief 得到日期时间字符串
 *
 * 得到日期时间字符串
 *
 * @param
 * @return
 * @author wangwei
 * @date 2015/06/16 wangwei 初始版本
 */
void GetDateTimeString(void)
{
	timeb fTimer;
	time_t timer;
	char year[5], month[3], day[3], hour[3], minute[3], second[3], miliSecond[4];
	struct tm *tblock;
//	timer=time(NULL);
//	tblock=localtime(&timer);

	ftime(&fTimer);
	timer = fTimer.time;
	tblock=localtime(&timer);

	sprintf(year, "%d", 1900+tblock->tm_year);

	if (tblock->tm_mon < 9) {
		sprintf(month, "0%d", 1+tblock->tm_mon);
	}
	else {
		sprintf(month, "%d", 1+tblock->tm_mon);
	}
	if (tblock->tm_mday < 10) {
		sprintf(day, "0%d", tblock->tm_mday);
	}
	else {
		sprintf(day, "%d", tblock->tm_mday);
	}
	if (tblock->tm_hour < 10) {
		sprintf(hour, "0%d", tblock->tm_hour);
	}
	else {
		sprintf(hour, "%d", tblock->tm_hour);
	}
	if (tblock->tm_min < 10) {
		sprintf(minute, "0%d", tblock->tm_min);
	}
	else {
		sprintf(minute, "%d", tblock->tm_min);
	}
	if (tblock->tm_sec < 10) {
		sprintf(second, "0%d", tblock->tm_sec);
	}
	else {
		sprintf(second, "%d", tblock->tm_sec);
	}

	if (fTimer.millitm < 10) {
		sprintf(miliSecond, "00%d", fTimer.millitm);
	}
	else if (fTimer.millitm < 100) {
		sprintf(miliSecond, "0%d", fTimer.millitm);
	}
	else {
		sprintf(miliSecond, "%d", fTimer.millitm);
	}

	sprintf(g_strDateTime,"%s%s%s%s%s%s%s", year, month, day, hour, minute, second, miliSecond);

	return;
}

/**
 * @brief 获得本机IP
 *
 * 获得本机IP
 *
 * @param  in_ipAddr: IP字符串
 * @return 0: 成功  0以外: 失败
 * @author wangwei
 * @date 2015/06/22 wangwei 初始版本
 */
int GetOwnIP(char* in_ipAddr)
{
	int iRslt = ERR_CMD_FAILED;
	int sock;
	struct sockaddr_in sin;
	struct ifreq ifr;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == -1) {
		g_TongueCtrlLog.E("GetOwnIP socket failed.");
	}
	else {
		strncpy(ifr.ifr_name, ETH_NAME, IFNAMSIZ);
		ifr.ifr_name[IFNAMSIZ - 1] = 0;

		if (ioctl(sock, SIOCGIFADDR, &ifr) < 0) {
			g_TongueCtrlLog.E("GetOwnIP ioctl failed");
		}
		else {
			memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
			strcpy(in_ipAddr, inet_ntoa(sin.sin_addr));
			iRslt = ERR_CMD_SUCCESS;
		}
	}

	return iRslt;
}
