#pragma once

#ifndef		_INC_IPROCESSBH
#define		_INC_IPROCESSBH

struct IMAGEPARAMENT {       
	int		nWidth;
	int		nHeight;
	int		nBitCount;
    int		nBytesPerLine;
	int		nBytesPerPixel;
	int		nNumColors;
	int		nSize;
};
struct SCOREDATA{
	int scoreR;
	int scoreG;
	int scoreB;
	int Rx;
	int Ry;
	int Gx;
	int Gy;
	int Bx;
	int By;
};
SCOREDATA  Match(CImage *pImgS,CImage *pImgT);
void  GetImageParament(CImage *pImg,struct IMAGEPARAMENT *ppImgParam);
int	  InImage(CImage *pImg,int x,int y);

DWORD GetPixelValue(CImage *pImg,int x,int y);
void  SetPixelValue(CImage *pImg,int x,int y,DWORD c);
void  GetRectValue(CImage *pImg,int x,int y,int Dx,int Dy,BYTE *buf);
void  SetRectValue(CImage *pImg,int x,int y,int Dx,int Dy,BYTE *buf);

void  GetPalette(CImage *pImg,int n,int *pr,int *pg,int *pb);
void  SetPalette(CImage *pImg,int n,int r,int g,int b);
void  GetAllPalette(CImage *pImg,RGBQUAD *ColorTab);
void  SetAllPalette(CImage *pImg,RGBQUAD *ColorTab);

void  RectFillValue(CImage *pImg,int x,int y,int Dx,int Dy,DWORD color);
void  DrawBox(CImage *pImg,int x,int y,int Dx,int Dy,int w,DWORD c);
void  SetDefaultPalette(CImage *pImg);

int   PaletteType(RGBQUAD	*ColorTab);
int   ImageType(CImage *pImgm);
void  ImageCopy(CImage *pImgn,CImage *pImgm);

BYTE** Create2DList(CImage *pImgm);
void  Release2DList(BYTE** list);

COLORREF DwordToColorref(DWORD c);
 
#endif  //!_INC_IPROCESSBH
