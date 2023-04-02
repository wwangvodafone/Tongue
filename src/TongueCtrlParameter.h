/*****************************************************************************/
/**
* @file TongueCtrlParameter.h
* @brief 舌苔控制参数定义
*
* 舌苔控制参数定义
*
* @author wangwei
* @date 2016/06/04 wangwei 初始版本
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved. */
/*****************************************************************************/

#ifndef TONGUECTRLPARAMETER_H_
#define TONGUECTRLPARAMETER_H_

#define CHAMBER_NUM	16
#define	V_SCOPE_NO			0		/*值无范围*/
#define V_SCOPE_VALUE		1		/*值的范围由两个值定义*/
#define V_SCOPE_SCOPE		2		/*值有范围*/
#define V_SCOPE_MIN			3		/*比MIN值大*/
#define V_SCOPE_MULVALUE	4		/*可以设多个值*/
#define V_SCOPE_EXCLUDE		5		/*指定的值以外的值是有效的*/

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

/* 数据格式定义 */
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

/*系统参数的Map*/
typedef struct _SystemParam_Map_St_tag {
	SystemParam_En	enParamId;											/*Parameter ID*/
	DATATYPE_En		enDataType;											/*数据类型*/
	float			fMinValue;											/*最小值*/
	float			fMaxValue;											/*最大值*/
	int			iMinValue;												/*最小值*/
	int			iMaxValue;												/*最大值*/
	unsigned int	uiScopeType;										/*值的范围类型*/
	int			(*set)(unsigned int param_id, NumericType value);		/*参数设定函数*/
	NumericType		(*request)(unsigned int param_id);					/*参数请求函数*/
	int				(*store)(unsigned int param_id);					/*参数存储函数*/
} SystemParam_Map;

/*系统共通参数*/
typedef struct _CommonSysParam_St_tag {
	unsigned int	uiPortMMI;
	unsigned int	uiPortMaster;
	unsigned int	uiPortTool;
} CommonSysParam_St;

/*Chamber控制参数*/
typedef struct _ChamberCtrlParam_St_tag {
	float	fDuty[CHAMBER_NUM];
} ChamberCtrlParam_St;

/*图像处理参数*/
typedef struct _ImageCtrlParam_St_tag {
	int		iResolutionX;							/*分辨率X*/
	int 	iResolutionY;							/*分辨率Y*/
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

	int		iStartX;								/*图像切取处理的开始X Pixel号*/
	int 	iStartY;								/*图像切取处理的开始Y Pixel号*/
	int		iEndX;									/*图像切取处理的终结X Pixel号*/
	int		iEndY;									/*图像切取处理的终结Y Pixel号*/
	int		iDivNumber;								/*Sub Pixel分割数*/
	int		iBinaryThreadValue;						/*二值化处理用阈值*/
	int		iInterval;								/*Edge坐标抽出时的间隔数*/
	int		iExclusionNumber;						/*Notch除去处理时的除外数*/
	int		iLightAdjustThreshValue;				/*照明亮度自动调整用的判断阈值*/
	int		iXRef;									/*Well中心坐标处理Teaching值X*/
	int		iYRef;									/*Well中心坐标处理Teaching值Y*/
	int		iRadiusAccuracy;						/*二值化阈值自动调整用的判断阈值[um]*/
	int		iPresenceThreshValue;					/*Well有无判断用的阈值*/
	float	fXdissolution;							/*Pixel的X分解[um]*/
	float 	fYdissolution;							/*Pixel的Y分解[um]*/
	float	fCalibGain;
	float 	fCalibOffset;

} ImageCtrlParam_St;

int SetSystemParam(unsigned int in_paramId, NumericType in_value);
NumericType RqSystemParam(unsigned int in_paramId);
int StoreSystemParam(unsigned int in_paramId);
extern unsigned int GetParamIdForSetting(unsigned int in_uiItemNo);

#endif /* TONGUECTRLPARAMETER_H_ */
