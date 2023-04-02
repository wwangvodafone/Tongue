/*****************************************************************************/
/**
* @file TongueCtrlCmdCheck.h
* @brief …‡Ã¶øÿ÷∆√¸¡Ó”Ô∑®ºÏ≤‚
*
* ºÏ≤‚…‡Ã¶øÿ÷∆√¸¡Ó”Ô∑®
*
* @author wangwei
* @date 2016/06/06 wangwei ≥ı º∞Ê±æ
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/

#ifndef TONGUECTRLCMD_ALIGNCTRLCMDCHECK_H_
#define TONGUECTRLCMD_ALIGNCTRLCMDCHECK_H_

extern int	CheckDummyCmd(char* in_pCommand);
extern int	CheckShutdownCmd(char* in_pCommand);
extern int	CheckResetCmd(char* in_pCommand);
extern int	CheckRebootCmd(char* in_pCommand);
extern int	CheckLightCmd(char* in_pCommand);
extern int	CheckPictureCmd(char* in_pCommand);
extern int	CheckMovieCmd(char* in_pCommand);
extern int	CheckLoadCmd(char* in_pCommand);
extern int	CheckSaveCmd(char* in_pCommand);
extern int	CheckUploadCmd(char* in_pCommand);
extern int	CheckSyncCmd(char* in_pCommand);
extern int	CheckDeleteCmd(char* in_pCommand);
extern int	CheckSetCmd(char* in_pCommand);
extern int	CheckRqCmd(char* in_pCommand);
extern int	CheckErrrstCmd(char* in_pCommand);
extern int	CheckIpChgCmd(char* in_pCommand);
extern int	CheckGetLogCmd(char* in_pCommand);
extern int	CheckUpgradeCmd(char* in_pCommand);
extern int	CheckSearchCmd(char* in_pCommand);
extern int	CheckInitMapsCmd(char* in_pCommand);
extern int	CheckSetWifiCmd(char* in_pCommand);
extern int	CheckObtInfoCmd(char* in_pCommand);
extern int	CheckDevPicCmd(char* in_pCommand);
extern int	CheckDevMovieCmd(char* in_pCommand);
extern int  CheckPhotoCatgCmd(char* in_pCommand);
extern int  CheckRpmStartCmd(char* in_pCommand);

#endif /* TONGUECTRLCMD_ALIGNCTRLCMDCHECK_H_ */
