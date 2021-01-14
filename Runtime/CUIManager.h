#pragma once

//--------------------- GDI+ Image 사용을 위한 헤더 추가
#include <ole2.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;
//--------------------- GDI+ Image 사용을 위한 헤더 추가

class CUIManager
{
	double m_DeltaTime = 0;
	HFONT a_hOldFont = NULL;
	HFONT m_Font_200_Gulim = NULL;
	HFONT m_Font_50_Batang = NULL;
	HFONT m_Font_RT_Fiophi = NULL;
	HFONT m_Font_100_Batang = NULL;
	HFONT m_Font_40_Batang = NULL;
	HFONT m_Font_30_Gulim = NULL;

	int m_Count = 3;			//스타트 카운트 시작 3, 2, 1 

	bool m_bPushBtnDU = false;
	Image *m_pBtnDownImg = NULL;
	Image *m_pBtnUpImg = NULL;

public:
	CUIManager();
	~CUIManager();

public:
	void UIMgrInit();
	void UIMgrUpdate(double a_DeltaTime, int a_Count);
	void UIMgrRender(HDC a_hdc, RECT &a_RT);
	void UIMgrDestroy();
	
	void GameReady_Render(HDC a_hdc, RECT &a_RT);
	void RaceReady_Render(HDC a_hdc, RECT &a_RT);
	void RaceEnd_Render(HDC a_hdc, RECT &a_RT);

	void OnLButtonDown(HWND hWnd, RECT &a_RT);	//마우스를 누르는 순간 한 번 발생한다.
	void OnLButtonUp(void(*CallLbittonUp)());	//마우스를 떼는 순간 한 번 발생한다.
};

extern CUIManager g_UI_Mgr;