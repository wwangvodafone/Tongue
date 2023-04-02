/*****************************************************************************/
/**
* @file TongueCtrlCamThd.h
* @brief 舌苔控制相机线程定义
*
* 舌苔控制相机线程定义
*
* @author wangwei
* @date 2016/06/08 wangwei 初始版本
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/

#ifndef TONGUECTRLCAMTHD_H_
#define TONGUECTRLCAMTHD_H_



extern RaspiVidBuffer_St g_raspiVidBuffer;

void* CamProcFunc(void* ptr);

#endif /* TONGUECTRLCAMTHD_H_ */
