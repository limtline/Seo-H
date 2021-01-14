#include "stdafx.h"
#include "CUIManager.h"
#include "GlobalValue.h"

CUIManager::CUIManager()
{
}


CUIManager::~CUIManager()
{
}

void CUIManager::UIMgrInit()
{
	m_Font_200_Gulim = CreateFont(200, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("굴림"));
	m_Font_50_Batang = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("바탕"));

	m_Font_100_Batang = CreateFont(100, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("바탕"));
	m_Font_40_Batang = CreateFont(40, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("바탕"));

	m_Font_30_Gulim = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("굴림"));
	
	m_pBtnUpImg = Image::FromFile(_T(".\\RscTrunk\\Button01.png"));
	m_pBtnDownImg = Image::FromFile(_T(".\\RscTrunk\\Button02.png"));
}

void CUIManager::UIMgrUpdate(double a_DeltaTime, int a_Count)
{
	m_DeltaTime = a_DeltaTime;

	if (a_Count < 0)
		a_Count = 0;
	m_Count = a_Count;
}

void CUIManager::UIMgrRender(HDC a_hdc, RECT & a_RT)
{
	switch (g_GameState)
	{
	case GAME_READY:
		GameReady_Render(a_hdc, a_RT);
		break;

	case RACE_READY:
		RaceReady_Render(a_hdc, a_RT);
		break;

	case RACE_END:
		RaceEnd_Render(a_hdc, a_RT);
		break;
	}
}

void CUIManager::UIMgrDestroy()
{
	if (m_Font_200_Gulim != NULL)
	{
		DeleteObject(m_Font_200_Gulim);
		m_Font_200_Gulim = NULL;
	}

	if (m_Font_50_Batang != NULL)
	{
		DeleteObject(m_Font_50_Batang);
		m_Font_50_Batang = NULL;
	}

	if (m_Font_100_Batang != NULL)
	{
		DeleteObject(m_Font_100_Batang);
		m_Font_100_Batang = NULL;
	}

	if (m_Font_40_Batang != NULL)
	{
		DeleteObject(m_Font_40_Batang);
		m_Font_40_Batang = NULL;
	}

	if (m_Font_30_Gulim != NULL)
	{
		DeleteObject(m_Font_30_Gulim);
		m_Font_30_Gulim = NULL;
	}

	if (m_pBtnUpImg != NULL)
	{
		delete m_pBtnUpImg;
		m_pBtnUpImg = NULL;
	}

	if (m_pBtnDownImg != NULL)
	{
		delete m_pBtnDownImg;
		m_pBtnDownImg = NULL;
	}
}

void CUIManager::GameReady_Render(HDC a_hdc, RECT & a_RT)
{
	a_hOldFont = (HFONT)SelectObject(a_hdc, m_Font_200_Gulim);
	SetTextColor(a_hdc, RGB(218, 128, 128));
	TextOut(a_hdc, (a_RT.right / 2), (a_RT.bottom / 2) - 200, _T("경 마"), (int)_tcslen(_T("경 마")));

	SelectObject(a_hdc, m_Font_50_Batang);

	//SetTextColor(a_hdc, RGB(0, 99, 177));
	static bool blink = true;	//깜빡임 용도
	static float a_TimeTic = 0;
	a_TimeTic = a_TimeTic + m_DeltaTime;
	if (1.0f <= a_TimeTic)
	{
		blink = !blink;
		a_TimeTic = 0.0f;
	}
	if (blink)
		SetTextColor(a_hdc, RGB(0, 99, 177));
	else
		SetTextColor(a_hdc, RGB(0, 0, 0));

	TextOut(a_hdc, (a_RT.right / 2), (a_RT.bottom*0.7f), _T("[시작하려면 [Space]키를 누르세요]"), (int)_tcslen(_T("[시작하려면[Space]키를 누르세요]")));

	SetTextColor(a_hdc, RGB(0, 0, 0, ));	//원래대로...

	Graphics graphics(a_hdc);	//Bitblit에서 사용하던 그 a_hdc를 넣으면 됨.
	if (m_bPushBtnDU == true)
	{
		graphics.DrawImage(m_pBtnDownImg,
			(a_RT.right / 2) - (m_pBtnDownImg->GetWidth() / 2), (a_RT.bottom * 0.85f));
	}
	else
	{
		graphics.DrawImage(m_pBtnUpImg,
			(a_RT.right / 2) - (m_pBtnUpImg->GetWidth() / 2), (a_RT.bottom * 0.85f));
	}
	SelectObject(a_hdc, m_Font_30_Gulim);
	TextOut(a_hdc, (a_RT.right / 2), (a_RT.bottom*0.855f), _T("시  작"), (int)_tcslen(_T("시  작")));

	SelectObject(a_hdc, a_hOldFont);
}

void CUIManager::RaceReady_Render(HDC a_hdc, RECT & a_RT)
{
	static int a_FontSize = 0;
	static HFONT a_hOldFont = NULL;
	
	a_FontSize = (a_RT.bottom * 0.3f);
	m_Font_RT_Fiophi = CreateFont(a_FontSize, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("바탕"));
	a_hOldFont = (HFONT)SelectObject(a_hdc, m_Font_RT_Fiophi);
	SetTextColor(a_hdc, RGB(87, 166, 74));

	static TCHAR a_TmStr[128];
	_stprintf_s(a_TmStr, _T("%d"), m_Count);
	TextOut(a_hdc, (a_RT.right / 2) - (a_FontSize * 0.1f), (a_RT.bottom / 2) - (a_FontSize*0.65f), a_TmStr, (int)_tclen(a_TmStr));

	SetTextColor(a_hdc, RGB(0, 0, 0));
	SelectObject(a_hdc, a_hOldFont);
	DeleteObject(m_Font_RT_Fiophi);
}

void CUIManager::RaceEnd_Render(HDC a_hdc, RECT & a_RT)
{
	static HFONT a_hOldFont = NULL;

	SetTextColor(a_hdc, RGB(87, 166, 74));
	a_hOldFont = (HFONT)SelectObject(a_hdc, m_Font_100_Batang);
	TextOut(a_hdc, (a_RT.right / 2), (a_RT.bottom / 2) - 200, _T("게임끝~~"), (int)_tcslen(_T("게임끝~~")));

	SelectObject(a_hdc, m_Font_40_Batang);
	static bool blink = true;	//깜빡임 용도
	static float a_TimeTic = 0;
	a_TimeTic = a_TimeTic + m_DeltaTime;
	if (1.0f <= a_TimeTic)
	{
		blink = !blink;
		a_TimeTic = 0.0f;
	}
	if (blink)
		SetTextColor(a_hdc, RGB(0, 99, 177));
	else
		SetTextColor(a_hdc, RGB(0, 0, 0));

	TextOut(a_hdc, (a_RT.right / 2), (a_RT.bottom / 2) + 150, _T("[다시 시작하려면 [R]키를 누르세요]"), (int)_tcslen(_T("[다시 시작하려면 [R]키를 누르세요]")));

	SetTextColor(a_hdc, RGB(0, 0, 0));

	Graphics graphics(a_hdc);	//Bitblit에서 사용하던 그 a_hdc를 넣으면 됨.
	if (m_bPushBtnDU == true)
	{
		graphics.DrawImage(m_pBtnDownImg,
			(a_RT.right / 2) - (m_pBtnDownImg->GetWidth() / 2), (a_RT.bottom * 0.85f));
	}
	else
	{
		graphics.DrawImage(m_pBtnUpImg,
			(a_RT.right / 2) - (m_pBtnUpImg->GetWidth() / 2), (a_RT.bottom * 0.85f));
	}
	SelectObject(a_hdc, m_Font_30_Gulim);
	TextOut(a_hdc, (a_RT.right / 2), (a_RT.bottom*0.855f), _T("게임다시"), (int)_tcslen(_T("게임다시")));

	SelectObject(a_hdc, a_hOldFont);

	SelectObject(a_hdc, a_hOldFont);
}

void CUIManager::OnLButtonDown(HWND hWnd, RECT & a_RT)
{
	if (m_bPushBtnDU == false)
	{
		LONG a_StXX = (a_RT.right / 2) - (m_pBtnDownImg->GetWidth() / 2);
		LONG a_StYY = (a_RT.bottom * 0.85f);
		LONG a_EndXX = a_StXX + m_pBtnDownImg->GetWidth();
		LONG a_EndYY = a_StYY + m_pBtnDownImg->GetWidth();

		POINT a_NewMMXZ;
		GetCursorPos(&a_NewMMXZ);				//현재 마우스 좌표를 얻어오는 함수
		ScreenToClient(hWnd, &a_NewMMXZ);		//클라이언트 화면 좌표로 변경해 주는 함수

		if (a_StXX <= a_NewMMXZ.x && a_NewMMXZ.x <= a_EndXX
			&& a_StYY <= a_NewMMXZ.y && a_NewMMXZ.y <= a_EndYY)
		{
			m_bPushBtnDU = true;
		}
		
	}
}

void CUIManager::OnLButtonUp(void(*CallLButtonUp)())
{
	if (m_bPushBtnDU == true)
	{
		if (CallLButtonUp != NULL)
			CallLButtonUp();

		m_bPushBtnDU = false;
	}
}

CUIManager g_UI_Mgr;