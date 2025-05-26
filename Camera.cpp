#include "StdAfx.h"
#include "Camera.h"

#include "resource.h"

CCamera::CCamera(void)
{
#ifndef _MYUTEST
    m_hCameraHandle = NULL;
    HVSTATUS hStatus = BeginHVDevice(1,&m_hCameraHandle);

	if(m_hCameraHandle == NULL)
	{
		return;
	}

	memset(m_chBmpBuf,0,sizeof(m_chBmpBuf));
	m_pBmpInfo = (BITMAPINFO *)m_chBmpBuf;

	//	初始化BITMAPINFO 结构，此结构在保存bmp文件、显示采集图像时使用
	m_pBmpInfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	//	图像宽度，一般为输出窗口宽度
	m_pBmpInfo->bmiHeader.biWidth			= 2048;
	//	图像宽度，一般为输出窗口高度
	m_pBmpInfo->bmiHeader.biHeight			= 1536;
	
	/*
	*	以下设置一般相同，
	*	对于低于8位的位图，还应设置相应的位图调色板
	*/
	m_pBmpInfo->bmiHeader.biPlanes			= 1;
	m_pBmpInfo->bmiHeader.biBitCount		= 24;
	m_pBmpInfo->bmiHeader.biCompression		= BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage		= 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter	= 0;
	m_pBmpInfo->bmiHeader.biClrUsed			= 0;
	m_pBmpInfo->bmiHeader.biClrImportant	= 0;

	for(int i=0;i<256;i++)
	{
		m_pLutR[i] = i;
		m_pLutG[i] = i;
		m_pLutB[i] = i;
	}

	m_bIsSnapSpeedSprted = false;
	m_bIsBlkSizeSprted = false;

	int size = sizeof(HVTYPE);
	
	HVGetDeviceInfo(m_hCameraHandle,DESC_DEVICE_TYPE, &m_cameraType, &size);	
	if(m_cameraType == HV2001UCTYPE || m_cameraType == HV1301UCTYPE)
	{
		m_bIsSnapSpeedSprted = true;   //本摄像机是否支持设置采集速度
		m_bIsBlkSizeSprted = true;   //本摄像机是否支持设置取得消隐取值范围	
	}

	m_iHMinBlkSize = 0;
	m_iHMaxBlkSize = 0;
	m_iVMinBlkSize = 0;
	m_iVMaxBlkSize = 0;

	if(m_bIsBlkSizeSprted)
	{
		DWORD pBlankSize[4];
		int nBufSize = 0;
		HVGetDeviceInfo(m_hCameraHandle,DESC_DEVICE_BLANKSIZE,NULL,&nBufSize);	
		HVGetDeviceInfo(m_hCameraHandle,DESC_DEVICE_BLANKSIZE,pBlankSize,&nBufSize);

		m_iHMinBlkSize = (int)pBlankSize[0];
		m_iVMinBlkSize = (int)pBlankSize[1];
		m_iHMaxBlkSize = (int)pBlankSize[2];
		m_iVMaxBlkSize = (int)pBlankSize[3];	
	}
#endif
}

CCamera::~CCamera(void)
{
#ifndef _MYUTEST
    if(m_hCameraHandle != NULL)
    {
        EndHVDevice(m_hCameraHandle);
    }
#endif
}

HV_RESOLUTION CCamera::GetResolutionMode()
{
#ifndef _MYUTEST
    int		iBuferSize = 0;
	BYTE	*pbContext = NULL;
	DWORD	*pdwContext = NULL;

	//分辨率
	HVGetDeviceInfo(m_hCameraHandle,DESC_RESOLUTION,NULL,&iBuferSize);
	pbContext = new BYTE[iBuferSize];
	pdwContext = (DWORD*)pbContext;
	HVGetDeviceInfo(m_hCameraHandle,DESC_RESOLUTION,pdwContext,&iBuferSize);
	DWORD dwMaxWidth = *(pdwContext);
	DWORD dwMaxHeight = *(pdwContext+1);
	delete []pbContext;
	pbContext = NULL;

	switch(dwMaxWidth)
	{
	case 2048:
		return RES_MODE0;
	case 1024:
		return RES_MODE1;
	case 640:
		return RES_MODE2;
	case 512:
		return RES_MODE3;
	default:
		return RES_MODE0;
	}
#endif
    return RES_MODE0;
}

void CCamera::SetResolutionMode(HV_RESOLUTION mode)
{
#ifndef _MYUTEST
    if(m_hCameraHandle == NULL)
        return;

    HVSetResolution(m_hCameraHandle,mode);
#endif
}

void CCamera::SetBlanking(int iHorBlankVal,int iVerBlankVal)
{
#ifndef _MYUTEST
    if(m_hCameraHandle == NULL)
        return;

    HVSetBlanking(m_hCameraHandle,iHorBlankVal,iVerBlankVal);
#endif
}

void CCamera::SetSetSnapMode(HV_SNAP_MODE mode)
{
#ifndef _MYUTEST
    if(m_hCameraHandle == NULL)
        return;

    HVSetSnapMode(m_hCameraHandle,mode);
#endif
}

void CCamera::SetTriggerPolarity(HV_POLARITY polarity)
{
#ifndef _MYUTEST
    if(m_hCameraHandle == NULL)
        return;

    HVSetTriggerPolarity(m_hCameraHandle,polarity);
#endif
}

void CCamera::SetStrobePolarity(HV_POLARITY polarity)
{
#ifndef _MYUTEST
    if(m_hCameraHandle == NULL)
        return;

    HVSetStrobePolarity(m_hCameraHandle,polarity);
#endif
}

void CCamera::SetSnapSpeed(HV_SNAP_SPEED speed)
{
#ifndef _MYUTEST
    if(m_hCameraHandle == NULL)
        return;

    HVSetSnapSpeed(m_hCameraHandle,speed);
#endif
}

void CCamera::EnableVideoMirror(HV_MIRROR_DIRECTION dir,BOOL bEnable)
{
#ifndef _MYUTEST
    if(m_hCameraHandle == NULL)
        return;

    HVEnableVideoMirror(m_hCameraHandle,dir,bEnable);
#endif
}

void CCamera::ADCControl(BYTE byParam, long lValue)
{
#ifndef _MYUTEST
    if(m_hCameraHandle == NULL)
        return;

    HVADCControl(m_hCameraHandle,byParam,lValue);
#endif
}

void CCamera::AGCControl(BYTE byParam, long lValue)
{
#ifndef _MYUTEST
    if(m_hCameraHandle == NULL)
        return;

    HVAGCControl(m_hCameraHandle,byParam,lValue);
#endif
}

void CCamera::AECControl(BYTE byParam, long lValue)
{
#ifndef _MYUTEST
    if(m_hCameraHandle == NULL)
        return;

    HVAECControl(m_hCameraHandle,byParam,lValue);
#endif
}

bool CCamera::GetImage(CImage image[],int num,int iOutWinWidth,int iOutWinHeight)
{
#ifndef _MYUTEST
    if(m_hCameraHandle == NULL)
        return false;

	BYTE **ppRawBuf = new BYTE*[num];
	BYTE **ppDestBuf = new BYTE*[num];

	for(int i=0;i<num;i++)
	{
		ppRawBuf[i] = new BYTE[iOutWinWidth*iOutWinHeight];
		ppDestBuf[i] = new BYTE[iOutWinWidth*iOutWinHeight*3];
	}

	//	图像宽度，一般为输出窗口宽度
	m_pBmpInfo->bmiHeader.biWidth			= iOutWinWidth;
	//	图像宽度，一般为输出窗口高度
	m_pBmpInfo->bmiHeader.biHeight			= iOutWinHeight;

    HVSTATUS status = HVSnapShot(m_hCameraHandle, ppRawBuf, num);
    if (HV_SUCCESS(status))
    {
		//将原始图像数据进行Bayer转换，转换后为24位,同时将原始数据进行上下翻转
		for(int i=0;i<num;i++)
		{
			ConvertBayer2Rgb(ppDestBuf[i],ppRawBuf[i],iOutWinWidth,iOutWinHeight,BAYER2RGB_NEIGHBOUR,
				m_pLutR,m_pLutG,m_pLutB,true,BAYER_GR);
			delete []ppRawBuf[i];
			ppRawBuf[i] = NULL;

			if(!image[i].Create(iOutWinWidth,iOutWinHeight,24,0))
			{
				::AfxMessageBox("Create image object fail");
				return false;
			}

			StretchDIBits(image[i].GetDC(),
			0,
			0,
			iOutWinWidth,				//显示窗口宽度
			iOutWinHeight,				//显示窗口高度
			0,
			0,
			iOutWinWidth,				//图像宽度
			iOutWinHeight,				//图像高度
			ppDestBuf[i],		//图像缓冲区
			m_pBmpInfo,			//BMP图像描述信息
			DIB_RGB_COLORS,
			SRCCOPY
			);

			image[i].ReleaseDC();

			//转换成CBitmap对象
			/*CBitmap *bitmap = new CBitmap();
			bitmap->CreateBitmap(iOutWinWidth,iOutWinHeight,0,24,NULL);

			BYTE *pByte = ppDestBuf[i];
			bitmap->SetBitmapBits(iOutWinWidth*iOutWinHeight*3,pByte);*/

			
			/*if(!bitmap->LoadBitmap(IDB_BITMAP1))//("..\\pic\\P1.bmp"))
			{
				int i = -1;
			}*/
			
			//image[i].Attach((HBITMAP)bitmap.GetSafeHandle());
			//image[i].Attach((HBITMAP)(*bitmap));

			delete []ppDestBuf[i];
			ppDestBuf[i] = NULL;
		}

		delete ppRawBuf;
		ppRawBuf = NULL;

		delete ppDestBuf;
		ppDestBuf = NULL;

        return true;
    }
    else
    {
		for(int i=0;i<num;i++)
		{
			delete []ppRawBuf[i];
			ppRawBuf[i] = NULL;

			delete []ppDestBuf[i];
			ppDestBuf[i] = NULL;
		}

		delete ppRawBuf;
		ppRawBuf = NULL;

		delete ppDestBuf;
		ppDestBuf = NULL;

        return false;
    }
#else
	return true;
#endif
}

bool CCamera::SetOutputWindow(int nXStart, int nYStart, int nWidth, int nHeight)
{
#ifndef _MYUTEST
    if(m_hCameraHandle == NULL)
        return false;

    HVSTATUS status = HVSetOutputWindow(m_hCameraHandle, nXStart, nYStart, nWidth, nHeight);
    if (HV_SUCCESS(status))
    {
        return true;
    }
    else
    {
        return false;
    }
#else
	return true;
#endif
}