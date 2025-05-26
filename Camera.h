#pragma once

#include "./CameraInc/HVDAILT.h"
#include "./CameraInc/HVDef.h"
#include "./CameraInc/HVUtil.h"
#include "./CameraInc/Raw2Rgb.h"

//摄像头类
class CCamera
{
public:
    CCamera(void);
public:
    ~CCamera(void);

protected:
    HHV     m_hCameraHandle;
    HVTYPE  m_cameraType;       //摄像机类型

	BITMAPINFO *m_pBmpInfo;		//BITMAPINFO 结构指针，显示图像时使用
	char m_chBmpBuf[2048];		//BIMTAPINFO 存储缓冲区，m_pBmpInfo即指向此缓冲区

	//颜色查找表
	BYTE m_pLutR[256] ;
	BYTE m_pLutG[256] ;
	BYTE m_pLutB[256] ;	

	bool m_bIsSnapSpeedSprted;   //本摄像机是否支持设置采集速度
	bool m_bIsBlkSizeSprted;   //本摄像机是否支持设置取得消隐取值范围

	//消隐的边界值
	int m_iHMinBlkSize;
	int m_iHMaxBlkSize;
	int m_iVMinBlkSize;
	int m_iVMaxBlkSize;

public:
    HVTYPE GetCameraType()
    {
        return m_cameraType;
    }

    int GetHMinBlkSize()
    {
        return m_iHMinBlkSize;
    }

    int GetHMaxBlkSize()
    {
        return m_iHMaxBlkSize;
    }

    int GetVMinBlkSize()
    {
        return m_iVMinBlkSize;
    }

    int GetVMaxBlkSize()
    {
        return m_iVMaxBlkSize;
    }

    //取摄像头分辨率
    HV_RESOLUTION GetResolutionMode();

    void SetResolutionMode(HV_RESOLUTION mode);

    //消隐
    void SetBlanking(int iHorBlankVal,int iVerBlankVal);

    //图像采集模式
    void SetSetSnapMode(HV_SNAP_MODE mode);

    //外触发信号极性
    void SetTriggerPolarity(HV_POLARITY polarity);

    //闪光灯信号的极性
    void SetStrobePolarity(HV_POLARITY polarity);

    //图像采集速度
    void SetSnapSpeed(HV_SNAP_SPEED speed);

    //镜像方向及是否允许
    void EnableVideoMirror(HV_MIRROR_DIRECTION dir,BOOL bEnable);

    void ADCControl(BYTE byParam, long lValue);

    void AGCControl(BYTE byParam, long lValue);

    void AECControl(BYTE byParam, long lValue);

    bool GetImage(CImage image[],int num,int iOutWinWidth,int iOutWinHeight);

    bool SetOutputWindow(int nXStart, int nYStart, int nWidth, int nHeight);
};
