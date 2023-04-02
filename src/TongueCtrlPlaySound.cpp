/*****************************************************************************/
/**
* @file TongueCtrlMsg.cpp
* @brief ��̦������Ϣ����
*
* ��̦������Ϣ����
*
* @author wangwei
* @date 2016/07/07 wangwei ��ʼ�汾
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>

#include "TongueCtrl.h"
#include "TongueCtrlCommand.h"
#include "TongueCtrlErrProc.h"
#include "TongueCtrlLogThd.h"
#include "TongueCtrlUtility.h"
#include "TongueCtrlGPIO.h"


/**
 * @brief �������ſ��ƺ���
 *
 *  �������ſ��ƺ���
 *
 * @param
 * @return
 * @author wangwei
 * @date 2017/05/19 wangwei ��ʼ�汾
 */
void* CtrlSoundFunc(void* ptr)
{

	while (1) {
		// �ȴ���Ϣ�����ź����ͷ�
		sem_wait(&g_semPlaySound);
		if (g_iPlaySound == PICTURE_START_SOUND) {
			PlaySound(PHOTO_SHUTTER_SOUND);
		}
		else if (g_iPlaySound == VIDEO_START_SOUND) {
			PlaySound(VIDEO_START_SHUTTER_SOUND);
		}
		else if (g_iPlaySound == VIDEO_END_SOUND) {
			PlaySound(VIDEO_END_SHUTTER_SOUND);
		}
		else if (g_iPlaySound == FAILED_SOUND) {
			PlaySound(PHOTO_VIDEO_FAILED_SOUND);
		}
	}

	return (void*)0;
}

