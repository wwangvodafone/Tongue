/*****************************************************************************/
/**
* @file TongueCtrlCmdRun.h
* @brief ��־�̶߳���
*
* ��־�̶߳���
*
* @author wangwei
* @date 2016/06/06 wangwei ��ʼ�汾
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/

#ifndef TongueCTRLLOG_H_
#define TongueCTRLLOG_H_

#define 	LOG_TEXT_LENGTH   		64										// ��־�ַ������ֳ���
#define 	LOG_TIME_LENGTH			17										// ��־ʱ�̲��ֳ���
#define 	DEBUG_LOG_LENGTH		(9+LOG_TIME_LENGTH+LOG_TEXT_LENGTH)	    // ��־����
#define		LOG_ITEM_COUNT_MAX		5000									// �ļ�����־�����Ŀ
#define 	LOG_FILE_NAME_LENGTH	(512+LOG_TIME_LENGTH)					// ��־�ļ����ĳ���

/** @brief ���O��ޗ񋓌^ */
typedef enum _LogType_En_tag_
{
	/** @brief ��־����:Information */
	LOG_TYPE_INFO = 0,
	/** @brief ��־����:Error */
	LOG_TYPE_ERR,
	/** @brief ��־����:Debug */
	LOG_TYPE_DBG,
	/** @brief ��־�ֱ��� */
	LOG_TYPE_NUM
} LogType_En;

/** @brief TongueCtrlLog */
typedef struct _TongueCtrlLog_St_tag_
{
	/** @brief information��־*/
	void (*I)(const char *in_pLogStr);
	/** @brief Debug��־ */
	void (*D)(const char *fmt, ...);
	/** @brief Error��־ */
	void (*E)(const char *fmt, ...);
} TongueCtrlLog_St;

/** @brief ����ʱ��ṹ�� */
typedef struct _Datatime_St_tag
{
	/** @brief ��(1900~9999) */
	int	ulYear;
	/** @brief ��(1~12) */
	int	ulMonth;
	/** @brief ��(1~31) */
	int	ulDay;
	/** @brief ʱ(0~23) */
	int	ulHour;
	/** @brief ��(0~59) */
	int	ulMinute;
	/** @brief ��(0~59) */
	int	ulSecond;
	/** @brief ����(0~999) */
	int	ulMSecond;
} Datetime_St;

/** @brief ��־��Ŀ�ṹ�� */
typedef struct _LogItem_St_tag
{
	/** @brief ����ʱ�� */
	Datetime_St	stDatetime;

	/** @brief ��־����(LOG_TYPE_XXX) */
	LogType_En	ulLogType;

	/** @brief ��־�ַ���*/
	char		pLogMsg[LOG_TEXT_LENGTH];
} LogItem_St;

/** @brief TongueCtrl��information��־��Ŀ��¼���� */
extern void TongueCtrlInformationLog(const char *in_pLogSt);

/** @brief TongueCtrl��Debug��־��Ŀ��¼����*/
extern void TongueCtrlDebugLog(const char *fmt, ...);

/** @brief TongueCtrl��Error��־��Ŀ��¼���� */
extern void TongueCtrlErrorLog(const char *fmt, ...);

/**
 * @brief TongueCtrl��־��Ŀ��¼���� */
extern void SaveTongueCtrlLogItem(LogType_En e_ulLogType, char *in_pLogStr);

/** @brief ��־�������*/
extern void LogOut(LogItem_St *in_pstLogItem);

/** @brief ��־�ļ����趨*/
extern bool SetLogFileName(Datetime_St* in_pstDatetime);

/** @brief ��־�ļ����� */
extern void* LogFileSaveFunc(void* ptr);

/** @brief TongueCtrlLog */
extern TongueCtrlLog_St	g_TongueCtrlLog;

#endif /* TongueCTRLLOG_H_ */
