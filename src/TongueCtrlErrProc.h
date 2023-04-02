/*****************************************************************************/
/**
* @file TongueCtrlErrProg.h
* @brief 舌苔控制错误处理定义
*
* 舌苔控制错误处理定义
*
* @author wangwei
* @date 2016/06/06 wangwei 初始版本
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/

#ifndef TONGUECTRLERRPROC_H_
#define TONGUECTRLERRPROC_H_

typedef struct _Error_St_tags_
{
	/** @brief Error名*/
	const char  *name;
	/** @brief Error编号 */
	unsigned int	uiErrId;
	/** @brief Error  */
	const char  *aErrDetail;
} Error_St;

enum {
	ERR_CMD_SUCCESS = 0,
	ERR_CMD_FAILED,
	ERR_CMD_PARSE_ERROR,
	ERR_CMD_CANNOT_EXECUTE,
	ERR_RECEPTION_COND,
	ERR_RESPONSE_FAILED,
	ERR_NO_USER,
	ERR_ALREADY_VIDEO,
	ERR_VIDEO_NOTBEGIN,
	ERR_ALREADY_PHOTO,
	ERR_WIFI_CONNECTED,
	ERR_IP_OBTAINED,
	ERR_PHOTO_CAPTURE,
	ERR_PHOTO_TONGUE_FAILED,
	ERR_PHOTO_FACE_FAILED,
	ERR_VIDEO_CAPTURE,
	ERR_IMAGE_SYNC,
	ERR_VIDEO_SYNC,
	ERR_IMAGE_UPLOAD,
	ERR_VIDEO_UPLOAD,
	ERR_FILE_DELETE,
	ERR_SET_VALUE,
	ERR_FILE_ALREADY_HAD,
	ERR_FILE_NOT_EXIST,
	ERR_FILE_IS_NULL,
	ERR_FOLDER_NOT_EXIST,
	ERR_COUNT
};

extern Error_St g_staCmdError[ERR_COUNT];

#define ERROR_NAME(kw) (g_staCmdError[kw].name)
#define ERROR_ID(kw) (g_staCmdError[kw].uiErrId)
#define ERROR_DETAIL(kw) (g_staCmdError[kw].aErrDetail)

#define ERRCODE(symbol, errid, errdetail) ERR_##symbol,


const char* getErrName(int iEnumNum);
int 	getErrId(int iEnumNum);
const char*	getErrDetail(int iEnumNum);


#endif /* TONGUECTRLERRPROC_H_ */
