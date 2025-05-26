#pragma once

#include "./CameraInc/HVDAILT.h"
#include "./CameraInc/HVDef.h"
#include "./CameraInc/HVUtil.h"
#include "./CameraInc/Raw2Rgb.h"

//����ͷ��
class CCamera
{
public:
    CCamera(void);
public:
    ~CCamera(void);

protected:
    HHV     m_hCameraHandle;
    HVTYPE  m_cameraType;       //���������

	BITMAPINFO *m_pBmpInfo;		//BITMAPINFO �ṹָ�룬��ʾͼ��ʱʹ��
	char m_chBmpBuf[2048];		//BIMTAPINFO �洢��������m_pBmpInfo��ָ��˻�����

	//��ɫ���ұ�
	BYTE m_pLutR[256] ;
	BYTE m_pLutG[256] ;
	BYTE m_pLutB[256] ;	

	bool m_bIsSnapSpeedSprted;   //��������Ƿ�֧�����òɼ��ٶ�
	bool m_bIsBlkSizeSprted;   //��������Ƿ�֧������ȡ������ȡֵ��Χ

	//�����ı߽�ֵ
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

    //ȡ����ͷ�ֱ���
    HV_RESOLUTION GetResolutionMode();

    void SetResolutionMode(HV_RESOLUTION mode);

    //����
    void SetBlanking(int iHorBlankVal,int iVerBlankVal);

    //ͼ��ɼ�ģʽ
    void SetSetSnapMode(HV_SNAP_MODE mode);

    //�ⴥ���źż���
    void SetTriggerPolarity(HV_POLARITY polarity);

    //������źŵļ���
    void SetStrobePolarity(HV_POLARITY polarity);

    //ͼ��ɼ��ٶ�
    void SetSnapSpeed(HV_SNAP_SPEED speed);

    //�������Ƿ�����
    void EnableVideoMirror(HV_MIRROR_DIRECTION dir,BOOL bEnable);

    void ADCControl(BYTE byParam, long lValue);

    void AGCControl(BYTE byParam, long lValue);

    void AECControl(BYTE byParam, long lValue);

    bool GetImage(CImage image[],int num,int iOutWinWidth,int iOutWinHeight);

    bool SetOutputWindow(int nXStart, int nYStart, int nWidth, int nHeight);
};
