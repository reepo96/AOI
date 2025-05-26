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
    long HorBlankVal;        //������ֵ
	long VerBlankVal;        //������
    long SnapMode;           //ͼ��ɼ�ģʽ
    long TriggerPolarity;    //�ⴥ���źŵļ���
    long StrobePolarity;     //������źŵļ���
    long SnapSpeed;          //ͼ��ɼ��ٶ�
    long EnableVideoMirror;  //�Ƿ�ֱ������
    long CamAdcBits;         //ģ��/����ת������
    BOOL BlkLevCal;          //�ڵ�ƽ������������
    long BlkLevCalRedVal;    //��ɫͨ���ڵ�ƽ
	long BlkLevCalGreen1Val; //��ɫͨ��1�ڵ�ƽ
	long BlkLevCalGreen2Val; //��ɫͨ��2�ڵ�ƽ
	long BlkLevCalBlueVal;   //��ɫͨ���ڵ�ƽ
	long BrightNessVal;      //����
	long GainRedVal;         //��ͨ������
	long GainGreen1Val;      //��ͨ��1����
	long GainGreen2Val;      //��ͨ��2����
	long GainBlueVal;        //��ͨ����
	long GainCameraVal;	    //���������
    long Exposure;           //�ع�ϵ��
    long ShutterSpeed;      //�����ٶ�
    long ShuterUnit;        //�����ٶȵ�λ

public:
    bool GetData();
    bool SaveData();
};
