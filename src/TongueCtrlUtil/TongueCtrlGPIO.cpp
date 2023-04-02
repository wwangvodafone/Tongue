/*****************************************************************************/
/**
* @file TongueCtrlGPIO.cpp
* @brief 舌苔控制GPIO
*
* 舌苔控制GPIO
*
* @author wangwei
* @date 2016/06/07 wangwei 初始版本
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved. */
/*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdarg.h>
#include <wiringPi.h>
#include <softTone.h>
#include "TongueCtrl.h"
#include "TongueCtrlGPIO.h"
#include "TongueCtrlErrProc.h"
#include "TongueCtrlCommand.h"
#include "TongueCtrlUtility.h"
#include "TongueCtrlLogThd.h"

static void StartDevCmdRun();

int key_press_num = 0;
int SonudType = 0;

/**
 * @brief GPIO初始化
 *
 * GPIO初始化
 *
 * @param
 * @return 0:正常 0以外：异常
 * @author Tengzhiwei
 * @date 2016/09/29 Tengzhiwei 初始版本
 */
void InitGPIO()
{
	g_CurrentPattern = PHOTO;
	wiringPiSetup();
	pinMode (GPIO1,OUTPUT);		//视频LED
	pinMode (GPIO2,INPUT);		//拍照/视频切换按键
	pinMode (GPIO3,OUTPUT);		//拍照LED
	pinMode (GPIO4,INPUT);		//拍照/视频启动按键
	pinMode (GPIO0,INPUT);		//AP模式切换
	pinMode (GPIO11,OUTPUT);	//照明灯
	pinMode (GPIO7,OUTPUT);		//照明灯
	pinMode (GPIO10,OUTPUT);	//照明灯
	SwitchLightOnOff(ILLUME_LIGHT,0);
	SwitchLightOnOff(IMAGE_LIGHT,ENABLE_ON);

}

/**
 * @brief GPIO检测
 *
 * GPIO检测
 *
 * @param
 * @author Tengzhiwei
 * @date 2016/09/29 Tengzhiwei 初始版本
 */

void CheckGPIO()
{

	unsigned long lastDebounceTime;
	unsigned long currentTime;

	if (digitalRead(GPIO0)) {	// Reset
		printf("Check GPIO0.\n");
		lastDebounceTime = millis();
		delay(10);
		if (digitalRead(GPIO0)) {
			g_TongueCtrlLog.I("CheckGPIO GPIO0 ON.");
			while (digitalRead(GPIO0));
			currentTime = millis();
			printf("%ld, %ld\n", currentTime, lastDebounceTime);
			if ((currentTime - lastDebounceTime ) > RESET_TIMEOUT) {
				g_stReceptionCond.uiCurDevStatus = DEVICE_EXEC;
				g_stReceptionCond.uiCurSysCmdStatus = DEVICE_EXEC;
				printf("CheckGPIO GPIO0 ON.\n");
				DoGPIOCmd(CMD_SETWIFI, 0);
			}

		}
	}

	if (g_stReceptionCond.uiCurSysCmdStatus == SYSTEM_READY) {
		if ((g_camStatus != CAM_VIDEO_START) && (g_camStatus != DEV_CAM_VIDEO_START)
				&& (g_camStatus != CAM_PHOTO)) { // System ready and video or Photo not start
			if(digitalRead(GPIO2)) {
				delay(10);
				if(digitalRead(GPIO2)) {
					while(digitalRead(GPIO2));
					if(g_CurrentPattern == PHOTO) {
						SwitchLightOnOff(VIDEO_LIGHT,ENABLE_ON);		//摄像LED亮
					}
					else if(g_CurrentPattern == MOVIE) {
						SwitchLightOnOff(IMAGE_LIGHT,ENABLE_ON);		//拍照LED亮
					}
				}
			}

			if (!digitalRead(GPIO4)) {
				printf("Check GPIO4.\n");
				delay(10);
				if (!digitalRead(GPIO4)) {
					g_stReceptionCond.uiCurDevStatus = DEVICE_EXEC;
					g_stReceptionCond.uiCurSysCmdStatus = DEVICE_EXEC;
					while (!digitalRead(GPIO4));
					SwitchLightOnOff(ILLUME_LIGHT, 1);
					delay(2000);
					if(g_CurrentPattern == PHOTO) {
						DoGPIOCmd(CMD_PICTURE, 0);	//拍照
					}
					else if(g_CurrentPattern == MOVIE) {
						g_MovieState = START;
						DoGPIOCmd(CMD_MOVIE, 0);	//摄像开始
					}
				}
			}
		}
		else if (g_camStatus == DEV_CAM_VIDEO_START) {	// 设备摄像
			if (!digitalRead(GPIO4)) {
				printf("Check GPIO4.\n");
				delay(10);
				if (!digitalRead(GPIO4)) {
					g_stReceptionCond.uiCurDevStatus = DEVICE_EXEC;
					g_stReceptionCond.uiCurSysCmdStatus = DEVICE_EXEC;
					while (!digitalRead(GPIO4));
					SwitchLightOnOff(ILLUME_LIGHT, 1);
					delay(2000);
					if(g_CurrentPattern == MOVIEING) {
						g_MovieState = STOP;
						DoGPIOCmd(CMD_MOVIE, 1);	//摄像停止
						SwitchLightOnOff(VIDEO_LIGHT,ENABLE_ON);
					}
				}
			}
		}

		if(g_MovieState == START) {
			SwitchLightOnOff(VIDEO_LIGHT,ENABLE_DONE);
		}

//		g_stReceptionCond.uiCurSysCmdStatus = SYSTEM_READY;
	}
}
/**
 * @brief GPIO命令执行
 *
 * GPIO命令执行
 *
 * @param
 * @author Tengzhiwei
 * @date 2016/06/27 Tengzhiwei 初始版本
 */
void DoGPIOCmd(int in_cmd, ...)
{
	va_list arg_ptr;
	int j = 0;
	char param[10];
	char mac[NAME_LENGTH];

	va_start(arg_ptr, in_cmd);
	j = va_arg(arg_ptr, int);
	va_end(arg_ptr);

	GetMacAddr(mac);	// Get mac address

	if (g_stReceptionCond.uiCurSysCmdStatus == DEVICE_EXEC) {
		switch (in_cmd) {
		case CMD_PICTURE:
			strncpy(g_sInputCommand, "PICTURE ", COMMAND_LENGTH);
			strncat(g_sInputCommand, mac, COMMAND_LENGTH);
			strncat(g_sInputCommand, " ", COMMAND_LENGTH);
			sprintf(param, "%d", j);
			strncat(g_sInputCommand, param, COMMAND_LENGTH);
			printf("Device:%s\n",g_sInputCommand);
			break;
		case CMD_MOVIE:
			strncpy(g_sInputCommand, "MOVIE ", COMMAND_LENGTH);
			strncat(g_sInputCommand, mac, COMMAND_LENGTH);
			strncat(g_sInputCommand, " ", COMMAND_LENGTH);
			sprintf(param, "%d", j);
			strncat(g_sInputCommand, param, COMMAND_LENGTH);
			printf("Device:%s\n",g_sInputCommand);
			break;
		case CMD_SETWIFI:
			strncpy(g_sInputCommand, "SETWIFI 0 ", COMMAND_LENGTH);
			printf("Device:%s\n",g_sInputCommand);
			break;
		default:
			break;
		}
		StartDevCmdRun();
	}

}
/**
 * @brief 拍照/摄像led灯切换点灯
 *
 * 拍照/摄像led灯切换点灯
 *
 * @param
 * @author Tengzhiwei
 * @date 2016/07/29 Tengzhiwei 初始版本
 * GPIO为LOW时点亮LED
 */
int SwitchLightOnOff(int in_category, int in_onOff)
{
	int iResult = ERR_CMD_SUCCESS;

	if(in_category==IMAGE_LIGHT&&in_onOff==ENABLE_ON) {
		printf("ImageLight on.\n");
		g_CurrentPattern = PHOTO;
		digitalWrite(GPIO3,HIGH);
		digitalWrite(GPIO1,LOW);
	}
	else if(in_category==VIDEO_LIGHT&&in_onOff==ENABLE_ON) {
		printf("VideoLight on.\n");
		g_CurrentPattern = MOVIE;
		g_MovieState = STOP;
		digitalWrite(GPIO1,HIGH);
		digitalWrite(GPIO3,LOW);
	}
	else if(in_category==VIDEO_LIGHT&&in_onOff==ENABLE_DONE) {
		printf("VideoLight blink.\n");
		g_CurrentPattern = MOVIEING;
		g_MovieState = START;
		digitalWrite(GPIO3,LOW);
		digitalWrite(GPIO1,HIGH);delay(200);
        digitalWrite(GPIO1,LOW);delay(200);
	}
	else if(in_category==ILLUME_LIGHT&&in_onOff==0) {	// 熄照明灯
		digitalWrite(GPIO7,LOW);
	}
	else if(in_category==ILLUME_LIGHT&&in_onOff==1) {	// 点照明灯
		digitalWrite(GPIO7,HIGH);
	}
	return iResult;
}

/**
 * @brief 设备发送命令执行启动
 *
 *  设备发送命令执行启动
 *
 * @param iSocket 接收Socket
 * @return
 * @author wangwei
 * @date 2016/10/19 wangwei 初始版本
 */
void StartDevCmdRun()
{
	// 释放命令执行信号量
	g_clientFlag = CT_FROM_UDP;
	sem_post(&g_semCmdRun);

}
