/*****************************************************************************/
/**
* @file TongueCtrlMsg.h
* @brief 舌苔控制消息队列定义
*
* 舌苔控制消息队列定义
*
* @author wangwei
* @date 2016/07/07 wangwei 初始版本
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/


#ifndef TONGUECTRLMSGTHD_H_
#define TONGUECTRLMSGTHD_H_

extern void* CtrlMsgFunc(void* ptr);
extern void TimerSet(char* pNameAddr, int sec);
extern void CamVideoIsr(int i);

#endif /* TONGUECTRLMSGTHD_H_ */
