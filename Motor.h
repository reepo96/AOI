#pragma once

//���������
class CMotor
{
public:
	typedef enum dir{NONE,LEFT,RIGHT,UP,DOWN} DIR; //�˶�����
protected:
	DIR	m_MoveDirX; //x���˶�����
	DIR	m_MoveDirY; //y���˶�����

public:
    CMotor(void);
public:
    virtual ~CMotor(void);
public:

    /*
    *��ʼ����
    *axis:��������1��2��3
    *limit:��λ���ش�����ƽ
    *home:Home �źŵĴ�����
    *enc:��������ļ�������
    */
    void AxisInitial(short axis,unsigned short limit,unsigned short home,unsigned short enc);

    /*
    *�����������߼Ӽ����˶���ĳһλ��
    *axis:��������1��2��3
    *pos:��Ŀ��λ��
    *vel:���ٶ�
    *acc:����ٶ�
    */
    void AxisRunT(unsigned short axis,long pos,double vel,double acc);

    /*
    *����S���߼Ӽ����˶���ĳһλ��
    *axis:��������1��2��3
    *pos:��Ŀ��λ��
    *vel:���ٶ�
    *macc:����ٶ�
    *jerk:��Ӽ��ٶ�
    */
    void AxisRunS(unsigned short axis,long pos,double vel,double macc,double jerk);

    /*
    *���ԭ��
    *axis:��������1��2��3
    *vel_high:���ٶ�
    *vel_low:���ٶ�    
    */
    short GoHome(unsigned short axis,double vel_high,double vel_low);

	void WaitStop(unsigned short axis);

    /*
    *���������ȵ���ĸ߶�
    *iPcbHeight:��ߣ���λmm
    */
    void AdjustTrackerHeight(int iPcbHeight);

	/*��ȡ���λ��*/
	short GetPos(unsigned short axis,long *pos);

	/*
	*����X����Y��ʵ��Ŀ��λ�ã�����X����Y����ڽǶ�ƫ���Ҫ��������Ŀ��λ�õó�ʵ��λ�ã�
	*/
	void ComputerFactPos(const long lSrcPosX,const long lSrcPosY,long *pDestPosX,long *pDestPosY);
};
