/*****************************************************************************/
/**
* @file TongueCtrlCamThd.h
* @brief 舌苔控制相机线程
*
* 舌苔控制相机线程
*
* @author wangwei
* @date 2016/06/08 wangwei 初始版本
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/

#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <sysexits.h>

#include "TongueCtrl.h"
#include "TongueCtrlCamThd.h"
#include "TongueCtrlCommon.h"
#include "TongueCtrlLogThd.h"
#include "TongueCtrlUtility.h"

#include <semaphore.h>
#include <CMath>

/**
 * @brief 相机线程函数
 *
 * 相机线程函数
 *
 * @param
 * @return 0:正常  -1:异常
 * @author wangwei
 * @date 2016/06/08 wangwei 初始版本
 */
void* CamProcFunc(void* ptr)
{
	g_TongueCtrlLog.I("CamProcFunc Start");

   while (1) {
	   sem_wait(&g_semCamera);
	   if ((g_camStatus == CAM_VIDEO_START) || (g_camStatus == DEV_CAM_VIDEO_START)) {
		   if (g_camStatus == DEV_CAM_VIDEO_START) {
			   PlaySound(VIDEO_START_SHUTTER_SOUND);
//			   g_iPlaySound = VIDEO_START_SOUND;
//			   sem_post(&g_semPlaySound);
		   }

		   StartProcess(TONGUE_VIDEO, &g_tongueVidParam);
	   }
   }
   g_TongueCtrlLog.I("CamProcFunc End");
   return (void*) 0;
}

