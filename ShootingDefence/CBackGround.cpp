#include "stdafx.h"
#include "CBackGround.h"

CBackGround::CBackGround()
{
}

CBackGround::~CBackGround()
{
}

void CBackGround::BG_Init(HWND a_hWnd)
{
	HDC hdc = GetDC(a_hWnd);  //WM_PAINT 밖에서 HDC 얻어오는 방법 
	m_BGImgDC = CreateCompatibleDC(hdc);  //MainDC의 복사본을 만든다.
	m_hTexImgBMP = (HBITMAP)LoadImage(NULL, _T(".\\RscTrunk\\bg1.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	//BMP파일을 로딩한다. 
	SelectObject(m_BGImgDC, m_hTexImgBMP); //복사본DC에 로딩된 HBITMAP을 선택해서 복사본DC를 준비시킨다.
	ReleaseDC(a_hWnd, hdc);  //얻어온 Main HDC를 해제해 준다.

	BITMAP bm;
	GetObject(m_hTexImgBMP, sizeof(BITMAP), &bm); //비트맵 정보 얻기
	m_TexImgWidth = bm.bmWidth;    //BMP 원본 의 가로 사이즈 얻어오기
	m_TexImgHeight = bm.bmHeight;  //BMP 원본 의 세로 사이즈 얻어오기
}

void CBackGround::BG_Update()
{
}

void CBackGround::BG_Render(HDC a_hdc, RECT & a_RT)
{
	StretchBlt(a_hdc, 0, 0, a_RT.right, a_RT.bottom, m_BGImgDC,
						0, 0, m_TexImgWidth, m_TexImgHeight, SRCCOPY);
}

void CBackGround::BG_Destroy()
{
	if (m_BGImgDC != NULL)
	{
		DeleteDC(m_BGImgDC);
		m_BGImgDC = NULL;
	}

	if (m_hTexImgBMP != NULL)
	{
		DeleteObject(m_hTexImgBMP);
		m_hTexImgBMP = NULL;
	}
}

CBackGround g_BGround;