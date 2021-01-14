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
	//------ 화면을 지우기 위한 펜과 블러쉬 생성
	m_hPen = CreatePen(PS_SOLID, 1, RGB(255, 211, 176));
	m_hBrush = CreateSolidBrush(RGB(255, 211, 176));
	//------ 화면을 지우기 위한 펜과 블러쉬 생성

	//------ 트랙 그리기용 펜 생성
	m_hLinePen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	m_hHLPen= CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
	//------ 트랙 그리기용 펜 생성
	
}

void CBackGround::BG_Update(RECT & a_RT)
{
	//---------- 윈도우 창 사이즈 변화에 따른 라인 좌표 조정해 주기
	m_StLine1.x = (LONG)((float)a_RT.right * 0.015f) + 92;
	m_StLine1.y = (LONG)((float)a_RT.bottom * 0.01f);

	m_StLine2.x = (LONG)((float)a_RT.right * 0.015f) + 92;
	m_StLine2.y = (LONG)((float)a_RT.bottom * 0.99f);

	m_EndLine1.x = (LONG)((float)a_RT.right * 0.985f) - 92;
	m_EndLine1.y = (LONG)((float)a_RT.bottom * 0.01f);

	m_EndLine2.x = (LONG)((float)a_RT.right * 0.985f) - 92;
	m_EndLine2.y = (LONG)((float)a_RT.bottom * 0.99f);
	//---------- 윈도우 창 사이즈 변화에 따른 라인 좌표 조정해 주기
}

void CBackGround::BG_Render(HDC a_hdc, RECT & a_RT)
{
	//------ DC 화면 지우기
	m_hOldBrush = (HBRUSH)SelectObject(a_hdc, m_hBrush);
	m_hOldPen = (HPEN)SelectObject(a_hdc, m_hPen);
	Rectangle(a_hdc, 0, 0, a_RT.right, a_RT.bottom);
	SelectObject(a_hdc, m_hOldBrush);
	SelectObject(a_hdc, m_hOldPen);
	//------ DC 화면 지우기

	 //--------세로줄 - 출발,도착선:
	m_hOldPen = (HPEN)SelectObject(a_hdc, m_hLinePen);

	MoveToEx(a_hdc, m_StLine1.x, m_StLine1.y, NULL);    //출발선 그리기
	LineTo(a_hdc, m_StLine2.x, m_StLine2.y);

	MoveToEx(a_hdc, m_EndLine1.x, m_EndLine1.y, NULL);  //도착선 그리기
	LineTo(a_hdc, m_EndLine2.x, m_EndLine2.y);

	SelectObject(a_hdc, m_hOldPen);//기존 펜으로 교체
	//--------세로줄 - 출발,도착선:

	//--------가로줄
	static int  a_CacPosX;
	static int  a_CacPosY;
	static int  a_CacYTab;
	m_hHLOldPen = SelectObject(a_hdc, m_hHLPen);

	a_CacPosX = (int)((float)a_RT.right * 0.01f);
	a_CacYTab = a_RT.bottom / 7;
	a_CacPosY = a_CacYTab;

	for (int i = 0; i < 6; i++)
	{
		MoveToEx(a_hdc, a_CacPosX, a_CacPosY + (i * a_CacYTab), NULL); //좌표를 이동
		LineTo(a_hdc, a_RT.right - a_CacPosX, a_CacPosY + (i * a_CacYTab)); //현 좌표에서 해당위치까지 선을 그어라        
	}
	SelectObject(a_hdc, m_hHLOldPen);//기존 펜으로 교체
	//--------가로줄
}

void CBackGround::BG_Destroy()
{
	if (m_hPen != NULL)
	{
		DeleteObject(m_hPen);
		m_hPen = NULL;
	}

	if (m_hBrush != NULL)
	{
		DeleteObject(m_hBrush);
		m_hBrush = NULL;
	}

	if (m_hLinePen != NULL)
	{
		DeleteObject(m_hLinePen);
		m_hLinePen = NULL;
	}

	if (m_hHLPen != NULL)
	{
		DeleteObject(m_hHLPen);
		m_hHLPen = NULL;
	}
}

CBackGround g_BGround;