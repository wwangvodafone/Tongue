/*****************************************************************************/
/**
* @file TongueCtrlParameter.h
* @brief 舌苔控制参数
*
* 舌苔控制参数定义
*
* @author wangwei
* @date 2016/06/04 wangwei 初始版本
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved. */
/*****************************************************************************/
#include "TongueCtrlParameter.h"
#include "TongueCtrlErrProc.h"
#include "TongueCtrl.h"
#include "TongueCtrlCommon.h"
#include "TongueCtrlLogThd.h"

int CheckParams(unsigned int in_paramId, NumericType in_value);

SystemParam_Map 	g_systemParamMap[] = {
		{DUMMY_SYSTEM_PARAM, 		NO_TYPE, 		0.0f, 		0.0f, 		0, 		0, 			V_SCOPE_NO, 			SetSystemParam,				RqSystemParam, 				StoreSystemParam},
		{PORT_MMI,					INT_TYPE, 		0.0f, 		0.0f, 		1, 		100000,  	V_SCOPE_SCOPE, 			SetSystemParam, 			RqSystemParam,				StoreSystemParam},
		{PORT_MASTER,				INT_TYPE, 		0.0f, 		0.0f, 		1, 		100000,  	V_SCOPE_SCOPE, 			SetSystemParam, 			RqSystemParam,				StoreSystemParam},
		{PORT_TOOL,					INT_TYPE,	 	0.0f, 		0.0f, 		1, 		100000,  	V_SCOPE_SCOPE, 			SetSystemParam, 			RqSystemParam, 				StoreSystemParam},
		{DUTY_1,					FLOAT_TYPE, 	0.1f,       100.0f, 	0, 		0, 			V_SCOPE_SCOPE, 			SetSystemParam, 			RqSystemParam, 				StoreSystemParam},
		{START_X,					INT_TYPE, 		0.0f, 		0.0f, 		1, 		2592,  		V_SCOPE_SCOPE, 			SetSystemParam, 			RqSystemParam, 				StoreSystemParam},
		{START_Y,					INT_TYPE, 		0.0f, 		0.0f, 		1, 		1944,  		V_SCOPE_SCOPE, 			SetSystemParam, 			RqSystemParam, 				StoreSystemParam},
		{END_X,						INT_TYPE, 		0.0f, 		0.0f, 		1, 		2592,  		V_SCOPE_SCOPE, 			SetSystemParam, 			RqSystemParam, 				StoreSystemParam},
		{END_Y,						INT_TYPE, 		0.0f, 		0.0f, 		1, 		1944,  		V_SCOPE_SCOPE, 			SetSystemParam, 			RqSystemParam, 				StoreSystemParam},
		{DIV_NUMBER,				INT_TYPE, 		0.0f, 		0.0f, 		0, 		5, 	 		V_SCOPE_SCOPE, 			SetSystemParam, 			RqSystemParam, 				StoreSystemParam},
		{BINARY_THRESH_VALUE,		INT_TYPE, 		0.0f, 		0.0f, 		0, 		255,  		V_SCOPE_SCOPE, 			SetSystemParam, 			RqSystemParam, 				StoreSystemParam},
		{EDGE_INTERVAL,				INT_TYPE, 		0.0f, 		0.0f, 		0, 		20,  		V_SCOPE_SCOPE, 			SetSystemParam, 			RqSystemParam, 				StoreSystemParam},
		{NOTCH_EXCLUSION_NUMBER,	INT_TYPE, 		0.0f, 		0.0f, 		0, 		50,  		V_SCOPE_SCOPE, 			SetSystemParam, 			RqSystemParam, 				StoreSystemParam},
		{LIGHTADJUST_THRESH_VALUE,	INT_TYPE, 		0.0f, 		0.0f, 		0, 		255,  		V_SCOPE_SCOPE, 			SetSystemParam, 			RqSystemParam, 				StoreSystemParam},
		{X_REF,						INT_TYPE, 		0.0f, 		0.0f, 		1, 		2592,  		V_SCOPE_SCOPE, 			SetSystemParam, 			RqSystemParam, 				StoreSystemParam},
		{Y_REF,						INT_TYPE, 		0.0f, 		0.0f, 		1, 		1944,  		V_SCOPE_SCOPE, 			SetSystemParam, 			RqSystemParam, 				StoreSystemParam},
		{RADIUS_ACCURACY,			INT_TYPE, 		0.0f, 		0.0f, 		1, 		1000,  		V_SCOPE_SCOPE, 			SetSystemParam,				RqSystemParam, 				StoreSystemParam},
		{PRESENCE_THRESH_VALUE,		INT_TYPE, 		0.0f, 		0.0f, 		0, 		255,  		V_SCOPE_SCOPE, 			SetSystemParam, 			RqSystemParam, 				StoreSystemParam},
		{X_DISSOLUTION,				FLOAT_TYPE, 	0.0f, 		1000.0f, 	0, 		0,  		V_SCOPE_SCOPE, 			SetSystemParam, 			RqSystemParam, 				StoreSystemParam},
		{Y_DISSOLUTION,				FLOAT_TYPE, 	0.0f, 		1000.0f, 	0, 		0,  		V_SCOPE_SCOPE, 			SetSystemParam, 			RqSystemParam, 				StoreSystemParam},
		{CALIB_GAIN,				FLOAT_TYPE, 	0.0f, 		1000.0f, 	0, 		0,  		V_SCOPE_NO, 			SetSystemParam, 			RqSystemParam, 				StoreSystemParam},
		{CALIB_OFFSET,				FLOAT_TYPE, 	0.0f, 		1000.0f, 	0, 		0,  		V_SCOPE_NO, 			SetSystemParam, 			RqSystemParam, 				StoreSystemParam},

};

NumericType	g_aSystemParams[SYS_PARAM_NUM];

/**
 * @brief 参数设定
 *
 * 参数设定
 *
 * @param in_paramId   参数ID
 * @param in_value	设定值
 * @return 0: 成功  0以外: 失败
 * @author wangwei
 * @date 2016/06/12 wangwei 初始版本
 */
int SetSystemParam(unsigned int in_paramId, NumericType in_value)
{
	int iResult = CheckParams(in_paramId, in_value);

	unsigned int uiMapNo = GetParamIdForSetting(in_paramId);
	DATATYPE_En enDataType = g_systemParamMap[uiMapNo].enDataType;


	if (iResult == ERR_CMD_SUCCESS) {
		if (enDataType == INT_TYPE) {
			g_aSystemParams[in_paramId].iValue = in_value.iValue;
		}
		else if (enDataType == FLOAT_TYPE) {
			g_aSystemParams[in_paramId].fValue = in_value.fValue;
		}
	}

	return iResult;
}

/**
 * @brief 参数请求
 *
 * 参数请求
 *
 * @param in_paramId 参数ID
 * @return 参数值
 * @author wangwei
 * @date 2016/06/12 wangwei 初始版本
 */
NumericType RqSystemParam(unsigned int in_paramId)
{
	NumericType value;
	value.iValue = 0;
	value.fValue = 0.0f;

	unsigned int uiMapNo = GetParamIdForSetting(in_paramId);
	DATATYPE_En enDataType = g_systemParamMap[uiMapNo].enDataType;

	if (enDataType == INT_TYPE) {
		value.iValue = g_aSystemParams[in_paramId].iValue;
	}
	else if (enDataType == FLOAT_TYPE) {
		value.fValue = g_aSystemParams[in_paramId].fValue;
	}

	return value;
}

/**
 * @brief 参数Store
 *
 * 参数Store
 *
 * @param in_paramId 参数ID
 * @return
 * @author wangwei
 * @date 2016/06/12 wangwei 初始版本
 */
int StoreSystemParam(unsigned int in_paramId)
{
	g_TongueCtrlLog.D("StoreSystemParam Start(in_paramId = %d).", in_paramId);

	int iResult = ERR_CMD_SUCCESS;
	iResult = WriteParameter(SYS_PARAM_NUM, g_aSystemParams);

	return iResult;
}


/**
 * @brief 取得参数ID
 *
 * 取得参数ID
 *
 * @param in_paramId 参数ID
 * @return g_systemParamMap的下标
 * @author wangwei
 * @date 2016/06/12 wangwei 初始版本
 */
unsigned int GetParamIdForSetting(unsigned int in_uiItemNo)
{
	g_TongueCtrlLog.D("GetParamIdForSetting Start(in_uiItemNo = %d).", in_uiItemNo);
	unsigned int uiResultNo = 0;
	if (in_uiItemNo < DUTY_1) {
		uiResultNo = in_uiItemNo;
	}
	else if (in_uiItemNo >= DUTY_1 && in_uiItemNo < START_X) {
		int iDuty = (in_uiItemNo - DUTY_1) % CHAMBER_NUM;
		uiResultNo = in_uiItemNo - iDuty;
	}
	else {
		uiResultNo = in_uiItemNo - CHAMBER_NUM + 1;
	}

	return uiResultNo;
}

/**
 * @brief 参数的check
 *
 * 参数的check
 *
 * @param in_paramId 参数ID
 * @param in_value	设定值
 * @return 0: 成功  0以外: 失败
 * @author wangwei
 * @date 2016/06/12 wangwei 初始版本
 */
int CheckParams(unsigned int in_paramId, NumericType in_value)
{
	g_TongueCtrlLog.D("CheckParams Start(in_paramId = %d, in_value = %d).", in_paramId, in_value);

	int iResult = ERR_CMD_SUCCESS;
	float fValue = in_value.fValue;
	int iValue = in_value.iValue;

	// g_systemParamMap攝楍偺揧偊帤傪寁嶼
	unsigned int uiMapNo = GetParamIdForSetting(in_paramId);

	unsigned int uiScopeType = g_systemParamMap[uiMapNo].uiScopeType;
	DATATYPE_En	enType = g_systemParamMap[uiMapNo].enDataType;

	if (uiScopeType == V_SCOPE_VALUE) {
		if (enType == INT_TYPE) {
			if ((iValue != (int)g_systemParamMap[uiMapNo].fMinValue)
				&& (iValue != (int)g_systemParamMap[uiMapNo].fMaxValue)) {
				iResult = ERR_SET_VALUE;
			}
		}
		else {
			if (fValue != g_systemParamMap[uiMapNo].fMinValue
					&& fValue != g_systemParamMap[uiMapNo].fMaxValue) {
				iResult = ERR_SET_VALUE;
			}
		}
	}
	else if (uiScopeType == V_SCOPE_SCOPE) {
		if (enType == FLOAT_TYPE) {
			if (fValue < g_systemParamMap[uiMapNo].fMinValue
					|| fValue > g_systemParamMap[uiMapNo].fMaxValue) {
				iResult = ERR_SET_VALUE;
			}
		}
		else if (enType == INT_TYPE) {
			if (iValue < g_systemParamMap[uiMapNo].iMinValue
					|| iValue > g_systemParamMap[uiMapNo].iMaxValue) {
				iResult = ERR_SET_VALUE;
			}
		}
	}
	else if (uiScopeType == V_SCOPE_MIN) {
		if (fValue < g_systemParamMap[uiMapNo].fMinValue) {
			iResult = ERR_SET_VALUE;
		}
	}
	else if (uiScopeType == V_SCOPE_EXCLUDE) {
		if (fValue == g_systemParamMap[uiMapNo].fMinValue) {
			iResult = ERR_SET_VALUE;
		}
	}

	g_TongueCtrlLog.D("CheckParams End(return = %d).", iResult);

	return iResult;
}
