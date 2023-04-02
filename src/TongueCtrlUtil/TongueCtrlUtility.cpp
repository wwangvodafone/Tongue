/*****************************************************************************/
/**
* @file TongueCtrlCommon.h
* @brief 舌苔控制Utility涵 数
*
* 舌苔控制Utility涵 数
*
* @author wangwei
* @date 2016/06/12 wangwei 初始版本
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <errno.h>
#include <sys/reboot.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <net/if.h>
#include <string.h>
#include <TongueCtrlUtil/TongueCtrlUtility.h>
#include <fstream>
#include <iostream>
#include <alsa/asoundlib.h>
#include <stdio.h>
#include <sndfile.h>

#include "TongueCtrlCommand.h"
#include "TongueCtrlErrProc.h"
#include "TongueCtrlLogThd.h"
#include "TongueCtrl.h"
#include "TongueCtrlDetector.h"

using namespace std;
void CopyFile(const char *in_srcFile, const char *in_dstFile);

/**
 * @brief ShutDown命令
 *
 *  ShutDown命令
 *
 * @param
 * @return
 * @author wangwei
 * @date 2016/06/12 wangwei 初始版本
 */
void ShutDown()
{
	//
//    sigset_t set;
//    sigfillset(&set);
//    sigprocmask(SIG_BLOCK, &set, NULL);
//
//    //
//    kill(-1, SIGTERM);
//    sync();
//    sleep(3);
//
//    kill(-1, SIGKILL);
//    sync();
//    sleep(3);
//
//    //
//    sleep(2);
	system("sudo halt");
}


/**
 * @brief Reboot命令
 *
 *  Reboot命令
 *
 * @param
 * @return
 * @author wangwei
 * @date 2016/06/12 wangwei 初始版本
 */
void Reboot(void)
{
	system("sudo reboot");
}

/**
 * @brief Reset命令
 *
 *  Reset命令
 *
 * @param
 * @return
 * @author wangwei
 * @date 2016/06/12 wangwei 初始版本
 */
void Reset(void)
{
	char commandReset[COMMAND_LENGTH];
	char command[COMMAND_LENGTH];
	char sType[5];
	unsigned int i = 0;

	strncpy(commandReset, SHELL_FOLDER, COMMAND_LENGTH);
	strncat(commandReset, "ResetSys ", COMMAND_LENGTH);
	for (i = 0; i < g_stCommand.unCommand.stCmdReset.uiNum; i++) {
		strncpy(command, commandReset, COMMAND_LENGTH);
		sprintf(sType, "%d", g_stCommand.unCommand.stCmdReset.uiCategory[i]);
		strncat(command, sType, COMMAND_LENGTH);
		printf("%s\n", command);
		system(command);
		g_TongueCtrlLog.D("%s\n", command);
	}
}

/**
 * @brief IP Change命令
 *
 *  IP Change命令
 *
 * @param
 * @return
 * @author wangwei
 * @date 2016/06/12 wangwei 初始版本
 */
void IpChg()
{
	char command[IPCHG_COMMAND_MAX] = "./IPChg.sh ";
	strcat(command, g_stCommand.unCommand.stCmdIpChg.ipaddr);
	strcat(command, " ");
	strcat(command, g_stCommand.unCommand.stCmdIpChg.netmask);

	system(command);
}


/**
 * @brief move命令
 *
 *  move命令
 *
 * @param
 * @return
 * @author wangwei
 * @date 2016/06/21 wangwei 初始版本
 */
void Move(char* in_fileName, char const * in_folderName)
{
	char command[COMMAND_LENGTH];

	strcpy(command, "mv ");
	strcat(command, in_fileName);
	strcat(command, " ");
	strcat(command, in_folderName);

	system(command);
}

/**
 * @brief rm命令
 *
 *  rm命令
 *
 * @param
 * @return
 * @author wangwei
 * @date 2016/06/29 wangwei 初始版本
 */
int Remove(char *in_fileName)
{
	int iResult = ERR_FILE_NOT_EXIST;
	char command[COMMAND_LENGTH];

	// 判断文件是否存在
	if (access(in_fileName, F_OK) == 0) {
		strcpy(command, "rm -f ");
		strcat(command, in_fileName);

		system(command);
		g_TongueCtrlLog.D("Delete %s\n", in_fileName);
		iResult = ERR_CMD_SUCCESS;
	}

	return iResult;
}


/**
 * @brief rm -r命令
 *
 *  rm -r命令
 *
 * @param
 * @return
 * @author wangwei
 * @date 2016/06/29 wangwei 初始版本
 */
int RemoveAll(char *in_fileName)
{
	int iResult = ERR_FILE_NOT_EXIST;
	char command[COMMAND_LENGTH];

	// 判断文件是否存在
	if (access(in_fileName, F_OK) == 0) {
		strcpy(command, "rm -rf ");
		strcat(command, in_fileName);

		system(command);

		iResult = ERR_CMD_SUCCESS;
	}

	return iResult;
}

/**
 * @brief 创建Log压缩文件
 *
 *  创建Log压缩文件
 *
 * @param 目录名
 * @return
 * @author wangwei
 * @date 2016/08/03 wangwei 初始版本
 */
void CreateLogFile(char *in_folderName, char* out_tarFileName)
{
	char fileFullName[FILE_MAX_LENGTH];
	char command[COMMAND_LENGTH];

	strncpy(fileFullName, in_folderName, FILE_MAX_LENGTH);
	strncat(fileFullName, out_tarFileName, FILE_MAX_LENGTH);

	g_TongueCtrlLog.D("The log file is: %s", fileFullName);
	if (access(fileFullName, F_OK) == 0) {
		strcpy(command, "rm -f ");
		strncat(command, fileFullName, COMMAND_LENGTH);
		g_TongueCtrlLog.D(command);

		system(command);
	}

	CompressFiles(in_folderName, out_tarFileName);

	return;
}

/**
 * @brief 压缩一个目录中全部文件
 *
 *  压缩一个目录中全部文件
 *
 * @param 目录名
 * @return
 * @author wangwei
 * @date 2016/07/22 wangwei 初始版本
 */
int CompressFiles(char *in_folderName, char* out_tarFileName)
{
	int iResult = ERR_FILE_NOT_EXIST;
	char command[COMMAND_LENGTH];

	if (access(out_tarFileName, F_OK) != 0) {	// if file exists
		strcpy(command, "rm -f ");
		strncat(command, in_folderName, COMMAND_LENGTH);
		strncat(command, out_tarFileName, COMMAND_LENGTH);
		printf("%s\n", command);
		system(command);
	}
	sleep(1);
	if (access(in_folderName, F_OK) == 0) {
		strcpy(command, "tar zvcf ");
		strncat(command, in_folderName, COMMAND_LENGTH);
		strncat(command, out_tarFileName, COMMAND_LENGTH);
		strncat(command, " ", COMMAND_LENGTH);
		strncat(command, in_folderName, COMMAND_LENGTH);

		g_TongueCtrlLog.D(command);
		printf("%s\n", command);
		system(command);
		iResult = ERR_CMD_SUCCESS;
	}

	return iResult;
}

/**
 * @brief 启动进程
 *
 *  启动进程
 *
 * @param
 * @return
 * @author wangwei
 * @date 2016/07/06 wangwei 初始版本
 */
int StartProcess(PROCESS_TYPE processType, void* param)
{
	int iResult = ERR_CMD_SUCCESS;
	char command[COMMAND_LENGTH];
	char buffer[FIFO_BUF_SIZE];
	int pipeFd, ansFd, res;
	char strSrcFile[FILE_MAX_LENGTH];
	char strDstFile[FILE_MAX_LENGTH];
	TonguePhotoParam_En tonguePhotoParam;
	TongueVidParam_En tongueVidParam;

	struct stat buf;

	tonguePhotoParam.sFileName = ((TonguePhotoParam_En*)param)->sFileName;
	tonguePhotoParam.sFolderName = ((TonguePhotoParam_En*)param)->sFolderName;
	if (access(IMG_VID_FIFO, F_OK) == -1) {
		printf("Can not access fifo for image and video.\n");
		g_TongueCtrlLog.E("Can not access fifo for image and video.");
		iResult = ERR_CMD_FAILED;
	}
	if (access(ANS_FIFO, F_OK) == -1) {
		g_TongueCtrlLog.E("Can not access fifo for image and video answer.");
		printf("%s\n", strerror(errno));
		iResult = ERR_CMD_FAILED;
	}
	if (iResult == ERR_CMD_SUCCESS) {
		pipeFd = open(IMG_VID_FIFO, O_WRONLY);
		ansFd = open(ANS_FIFO, O_RDWR);
		if (pipeFd == -1) {
			g_TongueCtrlLog.E("Can not open fifo for image and video.");
			printf("%s\n", strerror(errno));
			iResult = ERR_CMD_FAILED;
		}
		if (ansFd == -1) {
			g_TongueCtrlLog.E("Can not open fifo for image and video answer.");
			printf("%s\n", strerror(errno));
			iResult = ERR_CMD_FAILED;
		}
		if (iResult == ERR_CMD_SUCCESS) {
			if (processType == TONGUE_PHOTO) {
				strncpy(buffer, RPI_PIC, FIFO_BUF_SIZE);
				res = write(pipeFd, buffer, FIFO_BUF_SIZE);
				close(pipeFd);
				sleep(1);
				// 判断photo文件是否存在
				if (access(RPI_PHOTO, F_OK) == 0) {
					iResult = stat(RPI_PHOTO, &buf );
					if (buf.st_size > 0) {
						strncpy(strSrcFile, RPI_FOLDER, COMMAND_LENGTH);
						strncat(strSrcFile, RPI_WEB_IMAGE, COMMAND_LENGTH);
						strncpy(strDstFile, tonguePhotoParam.sFolderName, COMMAND_LENGTH);
						strncat(strDstFile, "/", COMMAND_LENGTH);
						strncat(strDstFile, tonguePhotoParam.sFileName, COMMAND_LENGTH);
						CopyFile(strSrcFile, strDstFile);

						if (GetFileSize(strDstFile) <= 0) {
							iResult = ERR_FILE_IS_NULL;
						}

					}
					else {
						iResult = ERR_FILE_NOT_EXIST;
					}
				}
				else {
					iResult = ERR_FILE_NOT_EXIST;
				}
			}
			else if (processType == TONGUE_VIDEO) {
				strncpy(buffer, RPI_VIDEO_START, FIFO_BUF_SIZE);
				res = write(pipeFd, buffer, FIFO_BUF_SIZE);
				close(pipeFd);
				close(ansFd);
			}
			else if (processType == TONGUE_VIDEO_END) {
				strncpy(buffer, RPI_VIDEO_END, FIFO_BUF_SIZE);
				res = write(pipeFd, buffer, FIFO_BUF_SIZE);
				close(pipeFd);
				sleep(1);

				tongueVidParam.sFileName = ((TongueVidParam_En*)param)->sFileName;
				tongueVidParam.sFolderName = ((TongueVidParam_En*)param)->sFolderName;

				// 判断video文件是否存在
				if (access(RPI_VIDEO, F_OK) == 0) {
					strncpy(strSrcFile, RPI_FOLDER, COMMAND_LENGTH);
					strncat(strSrcFile, RPI_WEB_VIDEO, COMMAND_LENGTH);
					strncpy(strDstFile, tongueVidParam.sFolderName, COMMAND_LENGTH);
					strncat(strDstFile, "/", COMMAND_LENGTH);
					strncat(strDstFile, tongueVidParam.sFileName, COMMAND_LENGTH);
					CopyFile(strSrcFile, strDstFile);
					strncpy(command, SHELL_FOLDER, COMMAND_LENGTH);
					strncat(command, "CreateVidPic", COMMAND_LENGTH);
					printf("%s\n", command);

					system(command);
				}
				else {
					iResult = ERR_FILE_NOT_EXIST;
				}
			}
			else if (processType == TONGUE_VIDEO_QUIT) {
				printf("Video quit.\n");
				strncpy(buffer, RPI_VIDEO_END, FIFO_BUF_SIZE);
				res = write(pipeFd, buffer, FIFO_BUF_SIZE);
				close(pipeFd);
				sleep(2);
				strncpy(command, "rm ", COMMAND_LENGTH);
				strncat(command, RPI_FOLDER, COMMAND_LENGTH);
				strncat(command, "*", COMMAND_LENGTH);
				printf("%s\n", command);
				system(command);
			}
		}
	}

	return iResult;
}

/**
 * @brief 得到mac地址
 *
 *  得到mac地址
 *
 * @param
 * @return mac地址
 * @author wangwei
 * @date 2016/07/14 wangwei 初始版本
 */
int GetMacAddr(char *out_mac)
{
	int iResult = -1;
	struct ifreq ifreq;
	int sock;

	if((sock=socket(AF_INET,SOCK_STREAM,0))<0) {
			perror("socket");
			return iResult;
	}
	strcpy(ifreq.ifr_name, "wlan0");
	if(ioctl(sock,SIOCGIFHWADDR,&ifreq)<0) {
			perror("ioctl");
			return iResult;
	}
	sprintf(out_mac, "%02X%02X%02X%02X%02X%02X",
			(unsigned char)ifreq.ifr_hwaddr.sa_data[0],
			(unsigned char)ifreq.ifr_hwaddr.sa_data[1],
			(unsigned char)ifreq.ifr_hwaddr.sa_data[2],
			(unsigned char)ifreq.ifr_hwaddr.sa_data[3],
			(unsigned char)ifreq.ifr_hwaddr.sa_data[4],
			(unsigned char)ifreq.ifr_hwaddr.sa_data[5]);

	iResult = ERR_CMD_SUCCESS;
	return iResult;
}

/**
 * @brief Change to wifi hotspot
 *
 *  Change to wifi hotspot
 *
 * @param
 * @return mac地址
 * @author wangwei
 * @date 2016/07/27 wangwei 初始版本
 */
void ChangeToHotspot()
{
	char command[COMMAND_LENGTH];

	strncpy(command, SHELL_FOLDER, COMMAND_LENGTH);
	strncat(command, "Chg2HS ", COMMAND_LENGTH);
	system(command);

	return;
}

/**
 * @brief Change to wifi station
 *
 *  Change to wifi station
 *
 * @param
 * @return mac地址
 * @author wangwei
 * @date 2016/07/27 wangwei 初始版本
 */
void ChangeToStation(char *in_ssid, char *in_pass)
{
	char command[COMMAND_LENGTH];

	strncpy(command, SHELL_FOLDER, COMMAND_LENGTH);
	strncat(command, "Chg2Station ", COMMAND_LENGTH);
	strncat(command, in_ssid, COMMAND_LENGTH);
	strncat(command, " ", COMMAND_LENGTH);
	strncat(command, in_pass, COMMAND_LENGTH);

	system(command);

	return;
}

/**
 * @brief Check the wifi station
 *
 *  Check the wifi station
 *
 * @param
 * @return mac地址
 * @author wangwei
 * @date 2016/07/28 wangwei 初始版本
 */
void CheckWifiStatus(void)
{
	char wifiFile1[COMMAND_LENGTH];
	char wifiFile2[COMMAND_LENGTH];

	strncpy(wifiFile1, SHELL_FOLDER, COMMAND_LENGTH);
	strncat(wifiFile1, ".wifi_1", COMMAND_LENGTH);
	strncpy(wifiFile2, SHELL_FOLDER, COMMAND_LENGTH);
	strncat(wifiFile2, ".wifi_2", COMMAND_LENGTH);

	if (access(wifiFile1, F_OK) == 0) {
		g_iWifiStatus = WIFI_HOTSPOT;
	}
	else if (access(wifiFile2, F_OK) == 0) {
		g_iWifiStatus = WIFI_STATION;
	}

	return;
}

/**
 * @brief Set hostname
 *
 *  Set hostname
 *
 * @param
 * @return
 * @author wangwei
 * @date 2016/08/03 wangwei 初始版本
 */
void SetHostName(void)
{
	char command[COMMAND_LENGTH];

	strncpy(command, SHELL_FOLDER, COMMAND_LENGTH);
	strncat(command, "SetHostname ", COMMAND_LENGTH);
	system(command);

	return;
}


/**
 * @brief 设定设备用储存视频照片目录
 *
 *  设定设备用储存视频照片目录
 *
 * @param
 * @return
 * @author wangwei
 * @date 2016/08/03 wangwei 初始版本
 */
void SetDeviceFolder(void)
{
	char command[COMMAND_LENGTH];

	strncpy(command, SHELL_FOLDER, COMMAND_LENGTH);
	strncat(command, "SetDeviceFolder ", COMMAND_LENGTH);

	system(command);

	return;
}

/**
 * @brief 得到固件的版本号
 *
 * 得到固件的版本号
 *
 * @param out_version 版本号
 * @return 0:正常  0以外:异常
 * @author wangwei
 * @date 2015/08/08 wangwei 初始版本
 */
int GetFirmareVersion(char* out_version)
{
	int iResult = ERR_CMD_FAILED;
	FILE *fp;
	char input[MAX_LINE_LENGTH];
	int len = 0;

	fp = fopen(VERSION_FILE, "r");

	if (fp != NULL) {
		while (fgets(input, MAX_LINE_LENGTH, fp)) {
			if (input[0] == '#') {
				continue;
			}
	        len = strlen(input);
	        if (len < 4) {
	            continue;
	        }
	        if (input[len-1] == '\n') {
	        	input[len-1] = '\0';
	        }
	        iResult = ERR_CMD_SUCCESS;
	        strncpy(out_version, input, VERSION_LENGTH);
	        break;
		}
	}

	fclose(fp);

	return iResult;
}

/**
 * @brief 转换成mp4
 *
 * 转换成mp4
 *
 * @param in_fileName 文件名
 * @return
 * @author wangwei
 * @date 2015/08/10 wangwei 初始版本
 */
void Change2Mp4(char* in_fileName, char* out_fileName)
{
	char command[COMMAND_LENGTH];
	strncpy(command, "ffmpeg -r 30 -i ", COMMAND_LENGTH);
	strncat(command, in_fileName, COMMAND_LENGTH);
	strncat(command, " -vcodec copy ", COMMAND_LENGTH);
	strncat(command, out_fileName, COMMAND_LENGTH);

	system(command);

	// Delete the h264
	strncpy(command, "rm -f ", COMMAND_LENGTH);
	strncat(command, in_fileName, COMMAND_LENGTH);

	system(command);

	return;
}

/**
 * @brief 版本升级
 *
 * 版本升级
 *
 * @param
 * @return
 * @author wangwei
 * @date 2015/11/06 wangwei 初始版本
 */
void StartUpgrade(void)
{
	int iStatus;
	char command[COMMAND_LENGTH];

	strncpy(command, SHELL_FOLDER, COMMAND_LENGTH);
	strncat(command, "Upgrade", COMMAND_LENGTH);
	printf("%s\n", command);
	g_TongueCtrlLog.D("StartUpgrade %s", command);
	iStatus = system(command);
	if (iStatus < 0) {
		g_TongueCtrlLog.D("StartUpgrade %s", strerror(errno));
	}

	return;
}

/**
 * @brief Play the sound
 *
 * Play the sound
 *
 * @param
 * @return
 * @author wangwei
 * @date 2017/05/17 wangwei 初始版本
 */
int PlaySound(const char* in_sound)
{
//	char command[COMMAND_LENGTH];
//	strncpy(command, SHELL_FOLDER, COMMAND_LENGTH);
//	strncat(command, "PlaySound ", COMMAND_LENGTH);
//	strncat(command, in_sound, COMMAND_LENGTH);
//	printf("%s\n", command);
//	g_TongueCtrlLog.D(command);
//	system(command);
//	char command[COMMAND_LENGTH];
//	int pid;
//
//	strncpy(command, SHELL_FOLDER, COMMAND_LENGTH);
//	strncat(command, SOUND_FOLDER, COMMAND_LENGTH);
//	strncat(command, in_sound, COMMAND_LENGTH);
//
//	pid=fork();
//	if(pid==0)
//	{
//		//printf("I am the child\n");
//		execlp("/usr/bin/omxplayer", " ", command, NULL);		//Execute file: file, arguments (1 or more strings followed by NULL)
//		_exit(0);
//	}
//	else
//	{
//		//printf("I am the parent\n");
//		wait();
//	}
	int iResult = ERR_CMD_SUCCESS;
    snd_pcm_t *pcm_handle;
    snd_pcm_hw_params_t *params;
    snd_pcm_uframes_t frames;
    int dir, pcmrc;
    short* buf = NULL;
    short readcount;

    SF_INFO sfinfo;
    SNDFILE *infile = NULL;

	char sound[COMMAND_LENGTH];
	strncpy(sound, SHELL_FOLDER, COMMAND_LENGTH);
	strncat(sound, SOUND_FOLDER, COMMAND_LENGTH);
	strncat(sound, in_sound, COMMAND_LENGTH);

    infile = sf_open(sound, SFM_READ, &sfinfo);
    fprintf(stderr,"Channels: %d\n", sfinfo.channels);
    fprintf(stderr,"Sample rate: %d\n", sfinfo.samplerate);
    fprintf(stderr,"Sections: %d\n", sfinfo.sections);
    fprintf(stderr,"Format: %d\n", sfinfo.format);

    /* Open the PCM device in playback mode */
    snd_pcm_open(&pcm_handle, PCM_DEVICE, SND_PCM_STREAM_PLAYBACK, 0);

    /* Allocate parameters object and fill it with default values*/
    snd_pcm_hw_params_alloca(&params);
    snd_pcm_hw_params_any(pcm_handle, params);
    /* Set parameters */
    snd_pcm_hw_params_set_access(pcm_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(pcm_handle, params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels(pcm_handle, params, sfinfo.channels);
    snd_pcm_hw_params_set_rate(pcm_handle, params, sfinfo.samplerate, 0);

    /* Write parameters */
    snd_pcm_hw_params(pcm_handle, params);

    /* Allocate buffer to hold single period */
    snd_pcm_hw_params_get_period_size(params, &frames, &dir);
    fprintf(stderr,"Starting read/write loop\n");
    buf = (short*)malloc(frames * sfinfo.channels * sizeof(short));
    while ((readcount = sf_readf_short(infile, buf, frames))>0) {

        pcmrc = snd_pcm_writei(pcm_handle, buf, readcount);
        if (pcmrc == -EPIPE) {
        	g_TongueCtrlLog.E("Underrun!");
        	iResult = ERR_CMD_FAILED;
            snd_pcm_prepare(pcm_handle);
        }
        else if (pcmrc < 0) {
        	g_TongueCtrlLog.E("Error writing to PCM device: %s", snd_strerror(pcmrc));
        	iResult = ERR_CMD_FAILED;
        }
        else if (pcmrc != readcount) {
        	g_TongueCtrlLog.E("PCM write difffers from PCM read.");
        	iResult = ERR_CMD_FAILED;
        }

    }
    fprintf(stderr,"End read/write loop\n");

    snd_pcm_drain(pcm_handle);
    snd_pcm_close(pcm_handle);
    free(buf);

    return iResult;
}


/**
 * @brief 转换图像
 *
 * 转换图像
 *
 * @param
 * @return 0:success other:failed
 * @author wangwei
 * @date 2017/07/21 wangwei 初始版本
 */
int CreatePhoto(void* param, int in_iCategory)
{
	TonguePhotoParam_En tonguePhotoParam;

	char command[COMMAND_LENGTH];
	char strFileName[FILE_MAX_LENGTH];
	char strTongueFileName[FILE_MAX_LENGTH];
	char strTongueLipFileName[FILE_MAX_LENGTH];

	int iResult = ERR_CMD_SUCCESS;

	tonguePhotoParam.sFileName = ((TonguePhotoParam_En*)param)->sFileName;
	tonguePhotoParam.sFolderName = ((TonguePhotoParam_En*)param)->sFolderName;
	tonguePhotoParam.sTongueFileName = ((TonguePhotoParam_En*)param)->sTongueFileName;
	tonguePhotoParam.sTongueFolderName = ((TonguePhotoParam_En*)param)->sTongueFolderName;
	tonguePhotoParam.sTongueLipFileName = ((TonguePhotoParam_En*)param)->sTongueLipFileName;
	tonguePhotoParam.sTongueLipFolderName = ((TonguePhotoParam_En*)param)->sTongueLipFolderName;

	strncpy(strFileName, tonguePhotoParam.sFolderName, FILE_MAX_LENGTH);
	strncat(strFileName, tonguePhotoParam.sFileName, FILE_MAX_LENGTH);
	strncpy(strTongueFileName, tonguePhotoParam.sTongueFolderName, FILE_MAX_LENGTH);
	strncat(strTongueFileName, tonguePhotoParam.sTongueFileName, FILE_MAX_LENGTH);
	strncpy(strTongueLipFileName, tonguePhotoParam.sTongueLipFolderName, FILE_MAX_LENGTH);
	strncat(strTongueLipFileName, tonguePhotoParam.sTongueLipFileName, FILE_MAX_LENGTH);

	printf("in_iCategory=%d\n", in_iCategory);
	if (in_iCategory == ORIGIN) {
		// Do nothing
	}
	else if (in_iCategory == ORIGIN_TONGUE) {
		CopyFile(strFileName, strTongueFileName);
		g_TongueCtrlLog.D(command);
		tongueDetection(strTongueFileName, strTongueFileName);
	}
	else if (in_iCategory == ORIGIN_TONGUE_LIP) {
		CopyFile(strFileName, strTongueLipFileName);
		g_TongueCtrlLog.D(command);
		if (!skinDetection(strTongueLipFileName, strTongueLipFileName)) { // Face convert failed.
			iResult = ERR_PHOTO_FACE_FAILED;
		}
	}
	else if (in_iCategory == ORIGIN_TONGUE_TONGUE_LIP) {
		CopyFile(strFileName, strTongueFileName);
		CopyFile(strFileName, strTongueLipFileName);
		g_TongueCtrlLog.D(command);
		tongueDetection(strTongueFileName, strTongueFileName);
		if (!skinDetection(strTongueLipFileName, strTongueLipFileName)) { // Tongue convert failed.
			iResult = ERR_PHOTO_TONGUE_FAILED;
		}
	}

	return iResult;
}

/**
 * @brief 文件拷贝
 *
 * 文件拷贝
 *
 * @param
 * @return 0:success other:failed
 * @author wangwei
 * @date 2017/08/27 wangwei 初始版本
 */
void CopyFile(const char *in_srcFile, const char *in_dstFile)
{
	std::ifstream input(in_srcFile,std::ios::binary);
	std::ofstream output(in_dstFile,std::ios::binary);

	output << input.rdbuf();
}


/**
 * @brief 判断文件的类型 0:原始文件，1:舌头文件，2：舌头和嘴唇文件
 *
 * 判断文件的类型 0:原始文件，1:舌头文件，2：舌头和嘴唇文件
 *
 * @param in_fileName 文件名
 * @param in_dotPos 文件名中.的倒数位置(0开始计数，便：a.jpg的.位置为3)
 * @return 0:原始文件，1:舌头文件，2：舌头和嘴唇文件
 * @author wangwei
 * @date 2017/08/28 wangwei 初始版本
 */
int JudgeFileCategory(char* in_fileName, int in_dotPos)
{
	char* p;
	char* q;
	int iResult = 0;
	int iUnderBar = '_';

	p = in_fileName;
	q = p + strlen(in_fileName);

	if (*(q - in_dotPos - 3) == iUnderBar) {
		iResult = *(q - in_dotPos - 2) - 48;
	}

	return iResult;
}


/**
 * @brief Rpm Start
 *
 *  Rpm Start
 *
 * @param
 * @return
 * @author wangwei
 * @date 2017/09/14 wangwei 初始版本
 */
void RpmStart(void)
{
	char command[COMMAND_LENGTH];

	strncpy(command, SHELL_FOLDER, COMMAND_LENGTH);
	strncat(command, "RpmStart ", COMMAND_LENGTH);
	g_TongueCtrlLog.D(command);
	printf("%s\n", command);
	system(command);
}


/**
 * @brief Get file size
 *
 *  Get file size
 *
 * @param
 * @return
 * @author wangwei
 * @date 2017/09/27 wangwei 初始版本
 */
unsigned long GetFileSize(const char* in_path)
{
    unsigned long filesize = -1;
    struct stat statbuff;
    if(stat(in_path, &statbuff) < 0){
        return filesize;
    }else{
        filesize = statbuff.st_size;
    }
    return filesize;

}


/**
 * @brief 时间设置
 *
 *  时间设置
 *
 * @param
 * @return
 * @author wangwei
 * @date 2017/10/10 wangwei 初始版本
 */
void SetTime(unsigned long in_lTime)
{
	char command[COMMAND_LENGTH];
	char sTime[UTC_LENGTH];

	sprintf(sTime, "%ld", in_lTime);
	strncpy(command, SHELL_FOLDER, COMMAND_LENGTH);
	strncat(command, "SetTime ", COMMAND_LENGTH);
	strncat(command, sTime, COMMAND_LENGTH);
	g_TongueCtrlLog.D(command);
	printf("%s\n", command);
	system(command);
}


/**
 * @brief 得到拍摄文件种类
 *
 * 得到拍摄文件种类（0，1，2，3）
 *
 * @param out_iNumber 种类号
 * @return 0:正常  0以外:异常
 * @author wangwei
 * @date 2017/10/10 wangwei 初始版本
 */
int GetPhotoCategory(char* out_number)
{
	int iResult = ERR_CMD_FAILED;
	FILE *fp;
	char input[MAX_LINE_LENGTH];
	int len = 0;

	fp = fopen(PHOTO_CATE_FILE, "r");

	if (fp != NULL) {
		while (fgets(input, MAX_LINE_LENGTH, fp)) {
			if (input[0] == '#') {
				continue;
			}
	        len = strlen(input);
	        if (input[len-1] == '\n') {
	        	input[len-1] = '\0';
	        }
	        iResult = ERR_CMD_SUCCESS;
	        strncpy(out_number, input, VERSION_LENGTH);
	        break;
		}
	}

	fclose(fp);

	return iResult;
}

/**
 * @brief 设置拍摄文件种类
 *
 * 高寒拍摄文件种类（0，1，2，3）
 *
 * @param in_number 种类号
 * @return 0:正常  0以外:异常
 * @author wangwei
 * @date 2017/10/10 wangwei 初始版本
 */
int SetPhotoCategory(char *in_number)
{
	int iResult = ERR_CMD_FAILED;
	FILE *fp;
	char input[MAX_LINE_LENGTH];
	int len = 0;

	fp = fopen(PHOTO_CATE_FILE, "wb");

	if (fp != NULL) {
		fwrite(in_number, sizeof(in_number), 1, fp);
	}

	fclose(fp);

	return iResult;
}
