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
	GT_ClrSts(axis);      //控制轴状态寄存器复位
	GT_DrvRst(axis);      //清除指定控制轴的伺服报警信号

	//GT_AlarmOff(axis);    //控制轴驱动报警信号无效
	GT_AlarmOn(axis);   //控制轴驱动报警信号有效

	GT_LmtsOn(axis);      //控制轴限位信号有效
	//GT_LmtsOff(axis);     //控制轴限位信号无效
	GT_EncOn(axis);       //打开控制轴的编码器输入
	//GT_EncOff(axis);      //关闭控制轴的编码器输入
	//GT_CaptHome(axis);  //将控制轴设置为Home 信号捕获
	GT_CaptIndex(axis);   // 将控制轴设置为Index 信号捕获
	//
	GT_LmtSns(limit);     //设置限位开关触发电平
	GT_EncSns(enc);       //设置控制轴编码器的记数方向
	GT_HomeSns(home);     //设置Home 信号的触发沿
	//GT_IndexSns(home); //设置Index 信号的触发沿
	//
	GT_StepPulse(axis);   //设置轴输出正负脉冲信号
	GT_AxisOn(axis);      //驱动使能
	Sleep(200);   //插入适当延时等待驱动器伺服就绪
}

void ZHOU::AxisRunT(unsigned short axis,long pos,double vel,double acc)
{
	GT_PrflT(axis); //设置为梯形曲线加减速
	GT_SetPos(axis,pos); //设置控制轴目标位置
	GT_SetVel(axis,vel); //设置控制轴速度
	GT_SetAcc(axis,acc); //设置控制轴加速度
	GT_Update(axis); //刷新控制轴参数
}
void ZHOU::AxisRunS(unsigned short axis,long pos,double vel,double macc,double jerk)
{
	GT_PrflS(axis); //设置为S 曲线加减速
	GT_SetPos(axis,pos); //设置控制轴目标位置
	GT_SetVel(axis,vel); //设置控制轴速度
	GT_SetMAcc(axis,macc); //设置控制轴加速度
	GT_SetJerk(axis,jerk); //设置控制轴加加速度
	GT_Update(axis); //刷新控制轴参数
}
/*
short Home(unsigned short axis,long pos,long offset,
double vel_high,double vel_low)                    //home+index方式
{
unsigned long status;
GT_ClrSts(axis); //清回原点轴状态
GT_CaptHome(axis); //将回原点轴设置为原点捕获方式
GT_SetPos(axis, pos);
GT_SetVel(axis,vel_high); //高速查找原点
GT_SetAcc(axis,0.01);
GT_Update(axis);
do
{
GT_GetSts(axis,&status); //读取回原点轴状态
if(!(status&0x400))
{ //回原点轴已经停止
return 1; //Home 信号仍然没有触发
}
}while(!(status&0x8)); //等待Home 捕获
GT_SmthStp(axis); //平滑停止回原点轴
do
{
GT_GetSts(axis,&status);
}while(status&0x400); //等待运动完成
GT_GetCapt(axis,&pos); //读取Home 捕获位置
GT_SetPos(axis,pos); //低速运动到原点捕获位置
GT_SetVel(axis,vel_low);
GT_Update(axis);
do
{
GT_GetSts(axis,&status);
}while(status&0x400); //等待运动完成
GT_ClrSts(axis); //清除Home 捕获触发标志
GT_CaptIndex(axis); //将回原点轴设置为
//Index 捕获
GT_SetPos(axis,pos+11000); //继续运动一圈多一点
GT_Update(axis);
do
{
GT_GetSts(axis,&status);
if(!(status&0x400))
{ //回原点轴已经停止
return 2; //Index 信号仍然没有触发
}
}while(!(status&0x8)); //等待Index 信号触发
GT_SmthStp(axis); //平滑停止回原点轴
do
{
GT_GetSts(axis,&status);
}while(status&0x400); //等待运动完成
GT_GetCapt(axis,&pos); //读取Index 捕获位置
GT_SetPos(axis,pos); //运动到Index 捕获位置
GT_SetVel(axis,vel_low);
GT_Update(axis);
do
{
GT_GetSts(axis,&status);
}while(status&0x400); //等待运动完成
GT_SetPos(axis,pos+offset); //运动到Index 捕获位置
GT_Update(axis);
do
{
GT_GetSts(axis,&status);
}while(status&0x400); //等待运动完成
Sleep(1000); //插入适当延时，等待工作台停稳
GT_ZeroPos(axis); //工作台位置清零建立机床坐标系
return 0;
}
*/
void ZHOU::SetAtlPos(unsigned short axis,long pos)
{
	GT_SetAtlPos(axis,pos);
}


short ZHOU::Home(unsigned short axis,long pos,long offset,double vel_high,double vel_low)   //home方式
{
	//double prf_pos[4];
	long TempPos;
	unsigned long status;
	GT_ClrSts(axis); //清回原点轴状态
	GT_CaptHome(axis); //将回原点轴设置为
	//原点捕获方式
	//GT_SetVel(axis,vel_high); //设置回原点的速度
	//GT_SetAcc(axis,0.01); //设置回原点的加速度
	//GT_GetPrfPos(axis,prf_pos);//读取各轴的规划位置GT_SetAtlPos();
	//GT_GetPrfPnt(prf_pos); //读取各轴的规划位置
	//prf_pos[axis-1] = pos; //设置回原点轴的目标位置
	//GT_LnXYZ(prf_pos[0],prf_pos[1],prf_pos[2]);
	GT_PrflS(axis); //设置为S 曲线加减速
	GT_SetPos(axis,pos); //设置控制轴目标位置
	GT_SetVel(axis,vel_high); //设置控制轴速度
	GT_SetMAcc(axis,1); //设置控制轴加速度
	GT_SetJerk(axis,0.001); //设置控制轴加加速度
	GT_Update(axis); //刷新控制轴参数
	do
	{
		GT_GetSts(axis,&status);//读取控制轴状态
		//GT_GetCrdSts(&crd_status); //读取坐标系状态
		//if(crd_status&1) //如果运动已经完成
		if(!(status&0x400))//如果运动已经完成
		{ //仍然没有触发Home 信号
			return 1; //返回错误代码1
		}
		//GT_GetSts(axis,&status); //读取控制轴状态
	}while(!(status&0x8)); //等待Home 信号触发
	GT_SmthStp(axis);//GT_StpMtn(); //平滑停止
	do
	{
		GT_GetSts(axis,&status);//读取控制轴状态
		//GT_GetCrdSts(&crd_status); //读取坐标系状态
	}while(status&0x400);//(!(crd_status&1)); //等待运动完成
	GT_GetCapt(axis,&TempPos); //读取捕获位置
	//GT_SetSynVel(vel_low); //低速运动到原点
	//prf_pos[axis-1]= pos;
	//rtn=GT_LnXYZ(prf_pos[0],prf_pos[1],prf_pos[2]);
	GT_SetPos(axis,TempPos); //设置控制轴目标位置
	GT_SetVel(axis,vel_low); //设置控制轴速度低速运动到原点
	GT_Update(axis); //刷新控制轴参数
	do
	{
		GT_GetSts(axis,&status);//读取控制轴状态
		//GT_GetCrdSts(&crd_status); //读取坐标系状态
	}while(status&0x400);//(!(crd_status&1)); //等待运动完成
	//prf_pos[axis-1]+= offset; //运动一小段距离离开Home 开关
	//GT_LnXYZ(prf_pos[0],prf_pos[1],prf_pos[2]);
	GT_SetPos(axis,TempPos+offset); //设置控制轴目标位置
	GT_SetVel(axis,vel_low); //设置控制轴速度低速运动到原点
	GT_Update(axis); //刷新控制轴参数
	do
	{
		GT_GetSts(axis,&status);//读取控制轴状态
		//GT_GetCrdSts(&crd_status); //读取坐标系状态
	}while(status&0x400);//(!(crd_status&1)); //等待运动完成
	Sleep(500); //插入适当延时，等待工作台停稳
	GT_ZeroPos(axis); //工作台位置清零建立机床坐标系
	return 0;
}



//***********************************************************************

//                            测试电机用

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
{ //控制轴初始化函数
short rtn;
rtn=GT_LmtSns(limit); //设置限位开关触发电平
error(rtn);
for(short i=1;i<=axis_num;++i)
{
rtn=GT_AlarmOff(i);   //使驱动报警无效
error(rtn);
rtn=GT_StepPulse(i); //设置轴输出正负脉冲信号
error(rtn);
//rtn=GT_AxisOn(i); //驱动使能
rtn=GT_AxisOff(i);
error(rtn);
rtn=GT_ClrSts(i); //控制轴状态寄存器复位
error(rtn);
Sleep(200); //插入适当延时
//等待驱动器伺服就绪
}
}
void AxisRunS(unsigned short axis,long pos,double vel,double macc,double
jerk)
{
GT_PrflS(axis); //设置为S 曲线加减速
GT_SetPos(axis,pos); //设置控制轴目标位置
GT_SetVel(axis,vel); //设置控制轴速度
GT_SetMAcc(axis,macc); //设置控制轴加速度
GT_SetJerk(axis,jerk); //设置控制轴加加速度
GT_Update(axis); //刷新控制轴参数
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
