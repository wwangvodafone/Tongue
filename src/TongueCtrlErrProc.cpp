/*****************************************************************************/
/**
* @file TongueCtrlErrProg.cpp
* @brief 舌苔控制错误处理定义
*
* 舌苔控制错误处理定义
*
* @author wangwei
* @date 2016/06/06 wangwei 初始版本
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "TongueCtrlErrProc.h"

Error_St g_staCmdError[ERR_COUNT] = {
		{"ERR_CMD_SUCCESS", 				0,		"Command Succeed."},
		{"ERR_CMD_FAILED", 					1,		"Command Failed."},
		{"ERR_CMD_PARSE_ERROR", 			300,	"Command Format Error."},
		{"ERR_CMD_CANNOT_EXECUTE", 			310,	"Command can not be executed."},
		{"ERR_RECEPTION_COND", 				320,	"Command reception condition error occurred."},
		{"ERR_RESPONSE_FAILED", 			330,	"Command response failed."},
		{"ERR_NO_USER", 					340,	"No such user."},
		{"ERR_ALREADY_VIDEO",				350,    "Video has already started."},
		{"ERR_VIDEO_NOTBEGIN",				355,	"Video has not started."},
		{"ERR_ALREADY_PHOTO",				360,    "Photo has already started."},
		{"ERR_WIFI_CONNECTED", 				400,	"Wifi cannot be connected."},
		{"ERR_IP_OBTAINED", 				410,	"Can not get own ip."},
		{"ERR_PHOTO_CAPTURE", 				500,	"Capture photo failed."},
		{"ERR_PHOTO_TONGUE_FAILED", 		510,	"Photo Tongue convert failed."},
		{"ERR_PHOTO_FACE_FAILED", 			520,	"Photo Face convert failed.."},
		{"ERR_VIDEO_CAPTURE", 				600,	"Capture video failed."},
		{"ERR_IMAGE_SYNC", 					700,	"Image synchronization failed."},
		{"ERR_VIDEO_SYNC", 					750,	"Video synchronization failed."},
		{"ERR_IMAGE_UPLOAD", 				800,	"Image upload failed."},
		{"ERR_VIDEO_UPLOAD", 				850,	"Video upload failed."},
		{"ERR_FILE_DELETE", 				900,	"Can not delete file."},
		{"ERR_SET_VALUE", 					1000,	"Error to set value."},
		{"ERR_FILE_ALREADY_HAD", 			1100,	"File already had."},
		{"ERR_FILE_NOT_EXIST", 				1110,	"File not exist."},
		{"ERR_FILE_IS_NULL", 				1115,	"File is null."},
		{"ERR_FOLDER_NOT_EXIST", 			1120,	"Folder not exist."},
};

/**********************************************************
 * @brief 获得Error名
 *
 *  获得Error名
 *
 * @param iEnumNum Error Enum编号
 * @return Error名
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
**********************************************************/
const char* getErrName(int iEnumNum)
{
	return ERROR_NAME(iEnumNum);
}

/**
 * @brief 获得Error ID
 *
 *  获得Error ID
 *
 * @param iEnumNum Error Enum编号
 * @return Error ID
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 **/
int 	getErrId(int iEnumNum)
{
	return ERROR_ID(iEnumNum);
}

/**
 * @brief 获得Error Detail
 *
 *  获得Error Detail
 *
 * @param iEnumNum Error Enum编号
 * @return Error Detail
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 **/
const char* 	getErrDetail(int iEnumNum)
{
	return ERROR_DETAIL(iEnumNum);
}



