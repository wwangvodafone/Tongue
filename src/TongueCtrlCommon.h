/*****************************************************************************/
/**
* @file TongueCtrlCommon.h
* @brief 舌苔控制命令共有变量涵 数定义
*
* 舌苔控制命令共有变量涵 数定义
*
* @author wangwei
* @date 2016/06/06 wangwei 初始版本
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/
#ifndef TONGUECTRLCOMMON_H_
#define TONGUECTRLCOMMON_H_

#include "TongueCtrlParameter.h"
#include "TongueCtrl.h"


/*CLOCK Info*/
typedef struct _SystemClock_St_tag {
	int iYear;
	int iMonth;
	int iDay;
	int iHour;
	int iMinute;
	int iSecond;
	int iMSecond;
} SystemClock_St;


extern int ReadParametersAll(int in_paramCnt, NumericType * in_pValue);
extern int WriteParametersAll(int in_paramCnt, NumericType * in_pValue);
extern int WriteParameter(int in_paramCnt, NumericType * in_pValue);
extern char* GetDateTimeString(void);
extern int GetOwnIP(char* in_ipAddr);

/*日期时间字符串*/
extern char g_strDateTime[];

#endif /* TONGUECTRLCOMMON_H_ */
