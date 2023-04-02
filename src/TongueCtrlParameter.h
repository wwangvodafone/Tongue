/*****************************************************************************/
/**
* @file TongueCtrlParameter.h
* @brief ��̦���Ʋ�������
*
* ��̦���Ʋ�������
*
* @author wangwei
* @date 2016/06/04 wangwei ��ʼ�汾
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved. */
/*****************************************************************************/

#ifndef TONGUECTRLPARAMETER_H_
#define TONGUECTRLPARAMETER_H_

#define CHAMBER_NUM	16
#define	V_SCOPE_NO			0		/*ֵ�޷�Χ*/
#define V_SCOPE_VALUE		1		/*ֵ�ķ�Χ������ֵ����*/
#define V_SCOPE_SCOPE		2		/*ֵ�з�Χ*/
#define V_SCOPE_MIN			3		/*��MINֵ��*/
#define V_SCOPE_MULVALUE	4		/*��������ֵ*/
#define V_SCOPE_EXCLUDE		5		/*ָ����ֵ�����ֵ����Ч��*/

typedef enum _SystemParam_En_tag {
	DUMMY_SYSTEM_PARAM = 0,
	RESOLUTION_W,
	RESOLUTION_H,
	ROI_START_X,
	ROI_START_Y,
	ROI_LEN_X,
	ROI_LEN_Y,
	CROP_X,
	CROP_Y,
	CROP_WIDTH,
	CROP_HEIGHT,
	PREVIEW,
	SENSOR_MODE,
	RUN_TYPE,
	PORT_MMI,
	PORT_MASTER,
	PORT_TOOL,
	DUTY_1,
	START_X = DUTY_1 + CHAMBER_NUM,
	START_Y,
	END_X,
	END_Y,
	DIV_NUMBER,
	BINARY_THRESH_VALUE,
	EDGE_INTERVAL,
	NOTCH_EXCLUSION_NUMBER,
	LIGHTADJUST_THRESH_VALUE,
	X_REF,
	Y_REF,
	RADIUS_ACCURACY,
	PRESENCE_THRESH_VALUE,
	X_DISSOLUTION,
	Y_DISSOLUTION,
	CALIB_GAIN,
	CALIB_OFFSET,
	SYS_PARAM_NUM
} SystemParam_En;

/* ���ݸ�ʽ���� */
typedef union _NumericType_Un_tag {
	int		iValue;
	float	fValue;
} NumericType;

/*DataType*/
typedef enum _DATATYPE_En_tag {
	NO_TYPE = 0,
	INT_TYPE,
	FLOAT_TYPE,
	IP_TYPE,
	DATE_TYPE
} DATATYPE_En;

/*ϵͳ������Map*/
typedef struct _SystemParam_Map_St_tag {
	SystemParam_En	enParamId;											/*Parameter ID*/
	DATATYPE_En		enDataType;											/*��������*/
	float			fMinValue;											/*��Сֵ*/
	float			fMaxValue;											/*���ֵ*/
	int			iMinValue;												/*��Сֵ*/
	int			iMaxValue;												/*���ֵ*/
	unsigned int	uiScopeType;										/*ֵ�ķ�Χ����*/
	int			(*set)(unsigned int param_id, NumericType value);		/*�����趨����*/
	NumericType		(*request)(unsigned int param_id);					/*����������*/
	int				(*store)(unsigned int param_id);					/*�����洢����*/
} SystemParam_Map;

/*ϵͳ��ͨ����*/
typedef struct _CommonSysParam_St_tag {
	unsigned int	uiPortMMI;
	unsigned int	uiPortMaster;
	unsigned int	uiPortTool;
} CommonSysParam_St;

/*Chamber���Ʋ���*/
typedef struct _ChamberCtrlParam_St_tag {
	float	fDuty[CHAMBER_NUM];
} ChamberCtrlParam_St;

/*ͼ�������*/
typedef struct _ImageCtrlParam_St_tag {
	int		iResolutionX;							/*�ֱ���X*/
	int 	iResolutionY;							/*�ֱ���Y*/
	float	fRoiStartX;
	float 	fRoiStartY;
	float 	fRoiLenX;
	float 	fRoiLenY;
	int		iCropX;
	int		iCropY;
	int		iCropWidth;
	int		iCropHeight;
	int 	iPreview;
	int		iSensorMode;
	int		iRunType;

	int		iStartX;								/*ͼ����ȡ����Ŀ�ʼX Pixel��*/
	int 	iStartY;								/*ͼ����ȡ����Ŀ�ʼY Pixel��*/
	int		iEndX;									/*ͼ����ȡ������ս�X Pixel��*/
	int		iEndY;									/*ͼ����ȡ������ս�Y Pixel��*/
	int		iDivNumber;								/*Sub Pixel�ָ���*/
	int		iBinaryThreadValue;						/*��ֵ����������ֵ*/
	int		iInterval;								/*Edge������ʱ�ļ����*/
	int		iExclusionNumber;						/*Notch��ȥ����ʱ�ĳ�����*/
	int		iLightAdjustThreshValue;				/*���������Զ������õ��ж���ֵ*/
	int		iXRef;									/*Well�������괦��TeachingֵX*/
	int		iYRef;									/*Well�������괦��TeachingֵY*/
	int		iRadiusAccuracy;						/*��ֵ����ֵ�Զ������õ��ж���ֵ[um]*/
	int		iPresenceThreshValue;					/*Well�����ж��õ���ֵ*/
	float	fXdissolution;							/*Pixel��X�ֽ�[um]*/
	float 	fYdissolution;							/*Pixel��Y�ֽ�[um]*/
	float	fCalibGain;
	float 	fCalibOffset;

} ImageCtrlParam_St;

int SetSystemParam(unsigned int in_paramId, NumericType in_value);
NumericType RqSystemParam(unsigned int in_paramId);
int StoreSystemParam(unsigned int in_paramId);
extern unsigned int GetParamIdForSetting(unsigned int in_uiItemNo);

#endif /* TONGUECTRLPARAMETER_H_ */
