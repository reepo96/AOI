#pragma once
#include "datamgr.h"

class CCameraSetDataMgr :
    public CDataMgr
{
public:
    CCameraSetDataMgr(void);
public:
    ~CCameraSetDataMgr(void);
protected:
	bool	m_bIsConnect;
public:
    long HorBlankVal;        //行消隐值
	long VerBlankVal;        //场消隐
    long SnapMode;           //图像采集模式
    long TriggerPolarity;    //外触发信号的极性
    long StrobePolarity;     //闪光灯信号的极性
    long SnapSpeed;          //图像采集速度
    long EnableVideoMirror;  //是否垂直方向镜像
    long CamAdcBits;         //模拟/数字转换级别
    BOOL BlkLevCal;          //黑电平参数调节允许
    long BlkLevCalRedVal;    //红色通道黑电平
	long BlkLevCalGreen1Val; //绿色通道1黑电平
	long BlkLevCalGreen2Val; //绿色通道2黑电平
	long BlkLevCalBlueVal;   //蓝色通道黑电平
	long BrightNessVal;      //亮度
	long GainRedVal;         //红通道增益
	long GainGreen1Val;      //绿通道1增益
	long GainGreen2Val;      //绿通道2增益
	long GainBlueVal;        //蓝通道增
	long GainCameraVal;	    //摄像机增益
    long Exposure;           //曝光系数
    long ShutterSpeed;      //快门速度
    long ShuterUnit;        //快门速度单位

public:
    bool GetData();
    bool SaveData();
};
