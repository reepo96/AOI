#pragma once


class ZHOU 
{
public:
	ZHOU(void);
	void AxisRunT(unsigned short axis,long pos,double vel,double acc);  //��������
	void AxisRunS(unsigned short axis,long pos,double vel,double macc,double jerk);   //S������
	void AxisInitial(short axis,unsigned short limit,unsigned short home,unsigned short enc);//��ʼ��
	short Home(unsigned short axis,long pos,long offset,double vel_high,double vel_low);//����ԭ��
	//short HomeIndex(unsigned short axis,long pos,long offset,double vel_high,double vel_low);//����ԭ��
	void SetAtlPos(unsigned short axis,long pos);  //ָ���������ʵ��λ�á�Ŀ��λ�á��滮λ��
public:
	~ZHOU(void);
private:


};
