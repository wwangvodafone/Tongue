/*****************************************************************************/
/**
* @file TongueCtrlSysMonitorThd.cpp
* @brief 舌苔控制系统监控线程
*
* 舌苔控制系统监控线程
*
* @author wangwei
* @date 2016/06/06 wangwei 初始版本
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>

#include "TongueCtrlSysMonitorThd.h"
#include "TongueCtrlGPIO.h"
#include "TongueCtrl.h"
#include "TongueCtrlMsgThd.h"

static void DevMonitorIsr(int);
static int iVideoStartTime = 0;
/**
 * @brief 系统监控线程函灵敏
 *
 * 系统监控线程函灵敏
 *
 * @param
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 **/
void* SysMonitorFunc(void *ptr)
{
	printf("SysMonitor Thread start.\n");
	SysMonitorTimerSet();

	while (1) {
		sem_wait(&g_semSysMonitor);
//		printf("g_semSysMonitor\n");
		if ((g_camStatus == DEV_CAM_VIDEO_START) && (g_CurrentPattern == MOVIEING)) {	// Device video ing...
			iVideoStartTime += CHECK_GPIO_TIME;
			printf("iVideoStartTime=%d\n", iVideoStartTime);
			if (iVideoStartTime >= VIDEO_TIMEOUT_TIME * 1000) {
				CamVideoIsr(0);
				iVideoStartTime = 0;
			}
		}
		else {
			iVideoStartTime = 0;
		}
		if (g_stReceptionCond.uiCurSysCmdStatus == SYSTEM_READY) {
			// GPIO检查
//			printf("CheckGPIO.\n");
			CheckGPIO();
		}
	}

	return (void*)0;
}


/**
 * @brief 计时器设置
 *
 * 计时器设置
 *
 * @param
 * @author wangwei
 * @date 2017/05/15 wangwei 初始版本
 **/
void SysMonitorTimerSet(void)
{
	int res = 0;

	printf("SysMonitorTimerSet.....\n");
	// Register the
	signal(SIGALRM, DevMonitorIsr);
	struct itimerval tick;

	// Initialize struct
	memset(&tick, 0, sizeof(tick));
	tick.it_value.tv_sec = 0;
	tick.it_value.tv_usec = CHECK_GPIO_TIME;

	// Interval time to run function
	tick.it_interval.tv_sec = 0;
	tick.it_interval.tv_usec = CHECK_GPIO_TIME;

	res = setitimer(ITIMER_REAL, &tick, NULL);
	if (res) {
		printf("Set timer failed!!/n");
	}

}

/**
 * @brief 设备监控计时处理
 *
 * 设备监控计时处理
 *
 * @param
 * @author wangwei
 * @date 2016/06/06 wangwei 初始版本
 **/
void DevMonitorIsr(int i)
{
	// System Monitor信号量释放
	sem_post(&g_semSysMonitor);
}
