/*****************************************************************************/
/**
* @file TongueCtrlMsg.h
* @brief ��̦������Ϣ���ж���
*
* ��̦������Ϣ���ж���
*
* @author wangwei
* @date 2016/07/07 wangwei ��ʼ�汾
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/


#ifndef TONGUECTRLMSGTHD_H_
#define TONGUECTRLMSGTHD_H_

extern void* CtrlMsgFunc(void* ptr);
extern void TimerSet(char* pNameAddr, int sec);
extern void CamVideoIsr(int i);

#endif /* TONGUECTRLMSGTHD_H_ */
