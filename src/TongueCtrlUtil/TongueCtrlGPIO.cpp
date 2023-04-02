/*****************************************************************************/
/**
* @file TongueCtrlGPIO.cpp
* @brief ��̦����GPIO
*
* ��̦����GPIO
*
* @author wangwei
* @date 2016/06/07 wangwei ��ʼ�汾
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
 * @brief GPIO��ʼ��
 *
 * GPIO��ʼ��
 *
 * @param
 * @return 0:���� 0���⣺�쳣
 * @author Tengzhiwei
 * @date 2016/09/29 Tengzhiwei ��ʼ�汾
 */
void InitGPIO()
{
	g_CurrentPattern = PHOTO;
	wiringPiSetup();
	pinMode (GPIO1,OUTPUT);		//��ƵLED
	pinMode (GPIO2,INPUT);		//����/��Ƶ�л�����
	pinMode (GPIO3,OUTPUT);		//����LED
	pinMode (GPIO4,INPUT);		//����/��Ƶ��������
	pinMode (GPIO0,INPUT);		//APģʽ�л�
	pinMode (GPIO11,OUTPUT);	//������
	pinMode (GPIO7,OUTPUT);		//������
	pinMode (GPIO10,OUTPUT);	//������
	SwitchLightOnOff(ILLUME_LIGHT,0);
	SwitchLightOnOff(IMAGE_LIGHT,ENABLE_ON);

}

/**
 * @brief GPIO���
 *
 * GPIO���
 *
 * @param
 * @author Tengzhiwei
 * @date 2016/09/29 Tengzhiwei ��ʼ�汾
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
						SwitchLightOnOff(VIDEO_LIGHT,ENABLE_ON);		//����LED��
					}
					else if(g_CurrentPattern == MOVIE) {
						SwitchLightOnOff(IMAGE_LIGHT,ENABLE_ON);		//����LED��
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
						DoGPIOCmd(CMD_PICTURE, 0);	//����
					}
					else if(g_CurrentPattern == MOVIE) {
						g_MovieState = START;
						DoGPIOCmd(CMD_MOVIE, 0);	//����ʼ
					}
				}
			}
		}
		else if (g_camStatus == DEV_CAM_VIDEO_START) {	// �豸����
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
						DoGPIOCmd(CMD_MOVIE, 1);	//����ֹͣ
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
 * @brief GPIO����ִ��
 *
 * GPIO����ִ��
 *
 * @param
 * @author Tengzhiwei
 * @date 2016/06/27 Tengzhiwei ��ʼ�汾
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
 * @brief ����/����led���л����
 *
 * ����/����led���л����
 *
 * @param
 * @author Tengzhiwei
 * @date 2016/07/29 Tengzhiwei ��ʼ�汾
 * GPIOΪLOWʱ����LED
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
	else if(in_category==ILLUME_LIGHT&&in_onOff==0) {	// Ϩ������
		digitalWrite(GPIO7,LOW);
	}
	else if(in_category==ILLUME_LIGHT&&in_onOff==1) {	// ��������
		digitalWrite(GPIO7,HIGH);
	}
	return iResult;
}

/**
 * @brief �豸��������ִ������
 *
 *  �豸��������ִ������
 *
 * @param iSocket ����Socket
 * @return
 * @author wangwei
 * @date 2016/10/19 wangwei ��ʼ�汾
 */
void StartDevCmdRun()
{
	// �ͷ�����ִ���ź���
	g_clientFlag = CT_FROM_UDP;
	sem_post(&g_semCmdRun);

}
