#pragma once

//电机控制类
class CMotor
{
public:
	typedef enum dir{NONE,LEFT,RIGHT,UP,DOWN} DIR; //运动方向
protected:
	DIR	m_MoveDirX; //x轴运动方向
	DIR	m_MoveDirY; //y轴运动方向

public:
    CMotor(void);
public:
    virtual ~CMotor(void);
public:

    /*
    *初始化轴
    *axis:轴索引：1、2、3
    *limit:限位开关触发电平
    *home:Home 信号的触发沿
    *enc:轴编码器的记数方向
    */
    void AxisInitial(short axis,unsigned short limit,unsigned short home,unsigned short enc);

    /*
    *轴做梯形曲线加减速运动到某一位置
    *axis:轴索引：1、2、3
    *pos:轴目标位置
    *vel:轴速度
    *acc:轴加速度
    */
    void AxisRunT(unsigned short axis,long pos,double vel,double acc);

    /*
    *轴做S曲线加减速运动到某一位置
    *axis:轴索引：1、2、3
    *pos:轴目标位置
    *vel:轴速度
    *macc:轴加速度
    *jerk:轴加加速度
    */
    void AxisRunS(unsigned short axis,long pos,double vel,double macc,double jerk);

    /*
    *轴回原点
    *axis:轴索引：1、2、3
    *vel_high:高速度
    *vel_low:低速度    
    */
    short GoHome(unsigned short axis,double vel_high,double vel_low);

	void WaitStop(unsigned short axis);

    /*
    *调整轨道宽度到板的高度
    *iPcbHeight:板高，单位mm
    */
    void AdjustTrackerHeight(int iPcbHeight);

	/*获取轴的位置*/
	short GetPos(unsigned short axis,long *pos);

	/*
	*计算X轴与Y轴实际目标位置（由于X轴与Y轴存在角度偏差，需要计算理论目标位置得出实际位置）
	*/
	void ComputerFactPos(const long lSrcPosX,const long lSrcPosY,long *pDestPosX,long *pDestPosY);
};
