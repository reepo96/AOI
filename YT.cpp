#include "StdAfx.h"
#include   "stdio.h" 
#include "YT.h"
#include "gep.h"


YT::YT(void)
{
}

YT::~YT(void)
{
}

void YT::error(short rtn)
{
	CString sError;
	switch(rtn)
	{
	case -1:
		sError="\a\nCommunication Error !"; break;
	case 0:
		
		break;
	case 1:
		sError="\a\nCommand Error !"; break;
	case 2:
		sError="\a\nRadius or chord is 0 !"; break;
	case 3:
		sError="\a\nLength is 0 or overflow !"; break;
	case 4:
		sError="\a\nVelocity or acceleration is less then 0 !";
		break;
	case 5:
		sError="\a\nChord is greater than diameter !"; break;
	case 7:
		sError="\a\nParameter error !"; break;
	default:
		sError.Format("%d",rtn);
		sError="\a\nError Code ="+sError; break;
	}
	::AfxMessageBox(sError);
}

void YT::YTInitiel(void)
{
	//GT_HookCommand(CommandHandle);
	GT_Open();
	GT_Reset();
	//AxisInitial(1,0,10);           //轴1
	//AxisRunT();                     //按照梯形曲线运动
}
