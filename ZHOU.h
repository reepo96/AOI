#pragma once


class ZHOU 
{
public:
	ZHOU(void);
	void AxisRunT(unsigned short axis,long pos,double vel,double acc);  //梯形曲线
	void AxisRunS(unsigned short axis,long pos,double vel,double macc,double jerk);   //S型曲线
	void AxisInitial(short axis,unsigned short limit,unsigned short home,unsigned short enc);//初始化
	short Home(unsigned short axis,long pos,long offset,double vel_high,double vel_low);//返回原点
	//short HomeIndex(unsigned short axis,long pos,long offset,double vel_high,double vel_low);//返回原点
	void SetAtlPos(unsigned short axis,long pos);  //指定控制轴的实际位置、目标位置、规划位置
public:
	~ZHOU(void);
private:


};
