/*****************************************************************************/
/**
* @file TongueCtrlUdpCommThd.cpp
* @brief 舌苔控制系统UDP线程
*
* 舌苔控制系统UDP线程
*
* @author wangwei
* @date 2016/07/25 wangwei 初始版本
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "TongueCtrl.h"
#include "TongueCtrlLogThd.h"
#include "TongueCtrlErrProc.h"

UdpComm_St g_udpComm;;

static void DoReceivedUdpMessage(char* in_buffer, int in_udpSocket, struct sockaddr_in in_sin);

void* UdpCommFxn(void* ptr)
{
	unsigned int            i,tmp;
	char buffer[UDPPACKETSIZE];

	g_udpComm.socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	 if (g_udpComm.socket < 0)
	 {
		 g_TongueCtrlLog.E("Failed UDP socket create\n");
		 return (void*)0;
	 }

	bzero(&g_udpComm.sin, sizeof(struct sockaddr_in));

	g_udpComm.sin.sin_family      = AF_INET;
	g_udpComm.sin.sin_addr.s_addr = htonl(INADDR_ANY);
	g_udpComm.sin.sin_port        = htons(UDP_BROADCAST_PORT);
	if( bind(g_udpComm.socket, (struct sockaddr*) &g_udpComm.sin, sizeof(g_udpComm.sin) ) < 0)
	{
		printf("UDP bind failed. %s\n", strerror(errno));
		g_TongueCtrlLog.E("failed UDP bind\n");
		return (void*)0;
	}

	for(;;) {
		tmp = sizeof(g_udpComm.sin);
		i = (int)recvfrom(g_udpComm.socket, buffer, UDPPACKETSIZE, 0,(struct sockaddr*)&g_udpComm.sin, &tmp);
		if( i >= 0 ) {
			DoReceivedUdpMessage(buffer, g_udpComm.socket, g_udpComm.sin);
		}
		// 等待Network信号量释放
		sem_wait(&g_semUdpComm);
	}

    close(g_udpComm.socket);

    return (void*)0;
}

/**
 * @brief 接收UDP包
 *
 * 接收UDP包
 *
 * @param in_buffer 接收Buffer
 * @param in_udpSocket	Socket
 * @param in_sin	sockaddr_in
 * @author wangwei
 * @date 2016/07/25 wangwei 初始版本
 */
void DoReceivedUdpMessage(char* in_buffer, int in_udpSocket, struct sockaddr_in in_sin)
{
	//
	if (!CutNetDelimiter(in_buffer)) {

	}
	memset(g_sInputCommand, '\0', sizeof(g_sInputCommand));
	memcpy(g_sInputCommand, in_buffer, strlen(in_buffer));
	g_sInputCommand[strlen(in_buffer)] = '\0';

	fprintf(stderr, "UDP input command: %s\n", g_sInputCommand);
	g_TongueCtrlLog.D("UDP input command: %s", g_sInputCommand);
	// Command
	// 释放命令执行信号量
	g_clientFlag = CT_FROM_UDP;
	sem_post(&g_semCmdRun);
}


