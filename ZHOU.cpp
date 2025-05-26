#include "StdAfx.h"
#include "ZHOU.h"
#include "gep.h"
//#include "windows.h" 
//#include<winbase.h>




ZHOU::ZHOU(void)
{
}

ZHOU::~ZHOU(void)
{
}
void ZHOU::AxisInitial(short axis,unsigned short limit,unsigned short home,unsigned short enc)
{
	GT_ClrSts(axis);      //������״̬�Ĵ�����λ
	GT_DrvRst(axis);      //���ָ����������ŷ������ź�

	//GT_AlarmOff(axis);    //���������������ź���Ч
	GT_AlarmOn(axis);   //���������������ź���Ч

	GT_LmtsOn(axis);      //��������λ�ź���Ч
	//GT_LmtsOff(axis);     //��������λ�ź���Ч
	GT_EncOn(axis);       //�򿪿�����ı���������
	//GT_EncOff(axis);      //�رտ�����ı���������
	//GT_CaptHome(axis);  //������������ΪHome �źŲ���
	GT_CaptIndex(axis);   // ������������ΪIndex �źŲ���
	//
	GT_LmtSns(limit);     //������λ���ش�����ƽ
	GT_EncSns(enc);       //���ÿ�����������ļ�������
	GT_HomeSns(home);     //����Home �źŵĴ�����
	//GT_IndexSns(home); //����Index �źŵĴ�����
	//
	GT_StepPulse(axis);   //������������������ź�
	GT_AxisOn(axis);      //����ʹ��
	Sleep(200);   //�����ʵ���ʱ�ȴ��������ŷ�����
}

void ZHOU::AxisRunT(unsigned short axis,long pos,double vel,double acc)
{
	GT_PrflT(axis); //����Ϊ�������߼Ӽ���
	GT_SetPos(axis,pos); //���ÿ�����Ŀ��λ��
	GT_SetVel(axis,vel); //���ÿ������ٶ�
	GT_SetAcc(axis,acc); //���ÿ�������ٶ�
	GT_Update(axis); //ˢ�¿��������
}
void ZHOU::AxisRunS(unsigned short axis,long pos,double vel,double macc,double jerk)
{
	GT_PrflS(axis); //����ΪS ���߼Ӽ���
	GT_SetPos(axis,pos); //���ÿ�����Ŀ��λ��
	GT_SetVel(axis,vel); //���ÿ������ٶ�
	GT_SetMAcc(axis,macc); //���ÿ�������ٶ�
	GT_SetJerk(axis,jerk); //���ÿ�����Ӽ��ٶ�
	GT_Update(axis); //ˢ�¿��������
}
/*
short Home(unsigned short axis,long pos,long offset,
double vel_high,double vel_low)                    //home+index��ʽ
{
unsigned long status;
GT_ClrSts(axis); //���ԭ����״̬
GT_CaptHome(axis); //����ԭ��������Ϊԭ�㲶��ʽ
GT_SetPos(axis, pos);
GT_SetVel(axis,vel_high); //���ٲ���ԭ��
GT_SetAcc(axis,0.01);
GT_Update(axis);
do
{
GT_GetSts(axis,&status); //��ȡ��ԭ����״̬
if(!(status&0x400))
{ //��ԭ�����Ѿ�ֹͣ
return 1; //Home �ź���Ȼû�д���
}
}while(!(status&0x8)); //�ȴ�Home ����
GT_SmthStp(axis); //ƽ��ֹͣ��ԭ����
do
{
GT_GetSts(axis,&status);
}while(status&0x400); //�ȴ��˶����
GT_GetCapt(axis,&pos); //��ȡHome ����λ��
GT_SetPos(axis,pos); //�����˶���ԭ�㲶��λ��
GT_SetVel(axis,vel_low);
GT_Update(axis);
do
{
GT_GetSts(axis,&status);
}while(status&0x400); //�ȴ��˶����
GT_ClrSts(axis); //���Home ���񴥷���־
GT_CaptIndex(axis); //����ԭ��������Ϊ
//Index ����
GT_SetPos(axis,pos+11000); //�����˶�һȦ��һ��
GT_Update(axis);
do
{
GT_GetSts(axis,&status);
if(!(status&0x400))
{ //��ԭ�����Ѿ�ֹͣ
return 2; //Index �ź���Ȼû�д���
}
}while(!(status&0x8)); //�ȴ�Index �źŴ���
GT_SmthStp(axis); //ƽ��ֹͣ��ԭ����
do
{
GT_GetSts(axis,&status);
}while(status&0x400); //�ȴ��˶����
GT_GetCapt(axis,&pos); //��ȡIndex ����λ��
GT_SetPos(axis,pos); //�˶���Index ����λ��
GT_SetVel(axis,vel_low);
GT_Update(axis);
do
{
GT_GetSts(axis,&status);
}while(status&0x400); //�ȴ��˶����
GT_SetPos(axis,pos+offset); //�˶���Index ����λ��
GT_Update(axis);
do
{
GT_GetSts(axis,&status);
}while(status&0x400); //�ȴ��˶����
Sleep(1000); //�����ʵ���ʱ���ȴ�����̨ͣ��
GT_ZeroPos(axis); //����̨λ�����㽨����������ϵ
return 0;
}
*/
void ZHOU::SetAtlPos(unsigned short axis,long pos)
{
	GT_SetAtlPos(axis,pos);
}


short ZHOU::Home(unsigned short axis,long pos,long offset,double vel_high,double vel_low)   //home��ʽ
{
	//double prf_pos[4];
	long TempPos;
	unsigned long status;
	GT_ClrSts(axis); //���ԭ����״̬
	GT_CaptHome(axis); //����ԭ��������Ϊ
	//ԭ�㲶��ʽ
	//GT_SetVel(axis,vel_high); //���û�ԭ����ٶ�
	//GT_SetAcc(axis,0.01); //���û�ԭ��ļ��ٶ�
	//GT_GetPrfPos(axis,prf_pos);//��ȡ����Ĺ滮λ��GT_SetAtlPos();
	//GT_GetPrfPnt(prf_pos); //��ȡ����Ĺ滮λ��
	//prf_pos[axis-1] = pos; //���û�ԭ�����Ŀ��λ��
	//GT_LnXYZ(prf_pos[0],prf_pos[1],prf_pos[2]);
	GT_PrflS(axis); //����ΪS ���߼Ӽ���
	GT_SetPos(axis,pos); //���ÿ�����Ŀ��λ��
	GT_SetVel(axis,vel_high); //���ÿ������ٶ�
	GT_SetMAcc(axis,1); //���ÿ�������ٶ�
	GT_SetJerk(axis,0.001); //���ÿ�����Ӽ��ٶ�
	GT_Update(axis); //ˢ�¿��������
	do
	{
		GT_GetSts(axis,&status);//��ȡ������״̬
		//GT_GetCrdSts(&crd_status); //��ȡ����ϵ״̬
		//if(crd_status&1) //����˶��Ѿ����
		if(!(status&0x400))//����˶��Ѿ����
		{ //��Ȼû�д���Home �ź�
			return 1; //���ش������1
		}
		//GT_GetSts(axis,&status); //��ȡ������״̬
	}while(!(status&0x8)); //�ȴ�Home �źŴ���
	GT_SmthStp(axis);//GT_StpMtn(); //ƽ��ֹͣ
	do
	{
		GT_GetSts(axis,&status);//��ȡ������״̬
		//GT_GetCrdSts(&crd_status); //��ȡ����ϵ״̬
	}while(status&0x400);//(!(crd_status&1)); //�ȴ��˶����
	GT_GetCapt(axis,&TempPos); //��ȡ����λ��
	//GT_SetSynVel(vel_low); //�����˶���ԭ��
	//prf_pos[axis-1]= pos;
	//rtn=GT_LnXYZ(prf_pos[0],prf_pos[1],prf_pos[2]);
	GT_SetPos(axis,TempPos); //���ÿ�����Ŀ��λ��
	GT_SetVel(axis,vel_low); //���ÿ������ٶȵ����˶���ԭ��
	GT_Update(axis); //ˢ�¿��������
	do
	{
		GT_GetSts(axis,&status);//��ȡ������״̬
		//GT_GetCrdSts(&crd_status); //��ȡ����ϵ״̬
	}while(status&0x400);//(!(crd_status&1)); //�ȴ��˶����
	//prf_pos[axis-1]+= offset; //�˶�һС�ξ����뿪Home ����
	//GT_LnXYZ(prf_pos[0],prf_pos[1],prf_pos[2]);
	GT_SetPos(axis,TempPos+offset); //���ÿ�����Ŀ��λ��
	GT_SetVel(axis,vel_low); //���ÿ������ٶȵ����˶���ԭ��
	GT_Update(axis); //ˢ�¿��������
	do
	{
		GT_GetSts(axis,&status);//��ȡ������״̬
		//GT_GetCrdSts(&crd_status); //��ȡ����ϵ״̬
	}while(status&0x400);//(!(crd_status&1)); //�ȴ��˶����
	Sleep(500); //�����ʵ���ʱ���ȴ�����̨ͣ��
	GT_ZeroPos(axis); //����̨λ�����㽨����������ϵ
	return 0;
}



//***********************************************************************

//                            ���Ե����

//***********************************************************************
/*
void error(short rtn)
{
switch(rtn)
{
case -1:
::AfxMessageBox("\a\nCommunication Error !"); break;
case 0:
break;
case 1:
::AfxMessageBox("\a\nCommand Error !"); break;
case 2:
::AfxMessageBox("\a\nRadius or chord is 0 !"); break;
case 3:
::AfxMessageBox("\a\nLength is 0 or overflow !"); break;
case 4:
::AfxMessageBox("\a\nVelocity or acceleration is less then 0 !");
break;
case 5:
::AfxMessageBox("\a\nChord is greater than diameter !"); break;
case 7:
::AfxMessageBox("\a\nParameter error !"); break;
default:
::AfxMessageBox("\a\nError Code = %d ",rtn); break;
}
}

void AxisInitial(short axis_num,unsigned short limit)
{ //�������ʼ������
short rtn;
rtn=GT_LmtSns(limit); //������λ���ش�����ƽ
error(rtn);
for(short i=1;i<=axis_num;++i)
{
rtn=GT_AlarmOff(i);   //ʹ����������Ч
error(rtn);
rtn=GT_StepPulse(i); //������������������ź�
error(rtn);
//rtn=GT_AxisOn(i); //����ʹ��
rtn=GT_AxisOff(i);
error(rtn);
rtn=GT_ClrSts(i); //������״̬�Ĵ�����λ
error(rtn);
Sleep(200); //�����ʵ���ʱ
//�ȴ��������ŷ�����
}
}
void AxisRunS(unsigned short axis,long pos,double vel,double macc,double
jerk)
{
GT_PrflS(axis); //����ΪS ���߼Ӽ���
GT_SetPos(axis,pos); //���ÿ�����Ŀ��λ��
GT_SetVel(axis,vel); //���ÿ������ٶ�
GT_SetMAcc(axis,macc); //���ÿ�������ٶ�
GT_SetJerk(axis,jerk); //���ÿ�����Ӽ��ٶ�
GT_Update(axis); //ˢ�¿��������
}
void CommandHandle(char *command,short error)
{
switch(error)
{
case -1:
::AfxMessageBox("\a\nCommunication Error !"); break;
case 0:
break;
case 1:
::AfxMessageBox("\a\nCommand Error !"); break;
case 7:
::AfxMessageBox("\a\nParameter Error !"); break;
default :
::AfxMessageBox("\a\nError Code =%d",error); break;
}
}

*/
