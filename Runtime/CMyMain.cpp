#include "stdafx.h"
#include "CMyMain.h"
#include "CBackGround.h"
#include "CCharacter_Mgr.h"
#include "CCharacter.h"
#include "GlobalValue.h"
#include "CUIManager.h"

#include <Mmsystem.h>						//timeGetTime() 함수 사용을 위하여...
#include <time.h>							//<---- 랜덤값을 얻기 위하여...
#pragma comment(lib,"winmm.lib")			//timeGetTime() 함수 사용을 위하여...

//--------------------- GDI+ Image 사용을 위한 헤더 추가
#include <ole2.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;
//--------------------- GDI+ Image 사용을 위한 헤더 추가

CMyMain::CMyMain()
{
}

CMyMain::~CMyMain()
{
}

void CMyMain::MainInit(HWND hWnd)
{
	srand((unsigned)time(NULL));	//랜덤 초기화

	m_LastTime = timeGetTime();		//현재시간 초기화

	//---------- GDI+ 초기화 (프로젝트 시작하자마자 한번만 해 주면된다.)
	GdiplusStartupInput gdiplusStartInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartInput, NULL);
	//---------- GDI+ 초기화

	//---------- 후면 BackDC 생성
	HDC hdc = GetDC(hWnd);		//메인 DC 가져오기
	m_hBackDC = CreateCompatibleDC(hdc);				  //메인 DC의 쌍둥이 복사본 DC를 하나 만든다.
	m_hBackBitmap = CreateCompatibleBitmap(hdc,           //BackDC에 붙여줄 Bitmap(이미지)를 하나 만들어 준다.
					GetSystemMetrics(SM_CXSCREEN),		  //바탕화면의 가로 사이즈
					GetSystemMetrics(SM_CYSCREEN));		  //바탕화면의 세로 사이즈
	SelectObject(m_hBackDC, m_hBackBitmap);				  //BackDC에 Bitmap(이미지)을 붙여준다
	ReleaseDC(hWnd, hdc);
	//---------- 후면 BackDC 생성

	//---------- 백그라운드 초기화
	g_BGround.BG_Init(hWnd);
	//---------- 백그라운드 초기화

	//---------- 캐릭터 매니저 초기화
	g_Char_Mgr.CM_Init(hWnd);
	//---------- 캐릭터 매니저 초기화

	//---------- UI 초기화 부분
	g_UI_Mgr.UIMgrInit();
	//---------- UI 초기화 부분
}

void CMyMain::MainUpdate(HWND hWnd)
{
	//---------- DeltaTime 얻어 오는 방법
	timeBeginPeriod(1);    //timeGetTime() 함수의 정밀도를 높이기 위한 함수
	m_CurTime = timeGetTime();
	m_DeltaTime = (m_CurTime - m_LastTime) * 0.001f;
	m_LastTime = m_CurTime;
	timeEndPeriod(1);
	//---------- DeltaTime 얻어 오는 방법

	if (m_hBackDC == NULL)
		return;

	//----키보드 처리 부분
	static bool a_OnceSpace = true;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (a_OnceSpace == true)
		{
			if (g_GameState == GAME_READY)
			{
				g_GameState = RACE_READY;
			}
			a_OnceSpace = false;
		}
	}
	else
		a_OnceSpace = true;

	static bool a_Once_R = true;
	if (GetAsyncKeyState('R') & 0x8000)
	{
		if (a_Once_R == true)
		{
			if (g_GameState == RACE_END)
			{
				ReSetGame();	//게임 초기화
			}

			a_Once_R = false;
		}
	}
	else
		a_Once_R = true;
	//----키보드 처리 부분

	//----마우스 처리 부분
	static bool a_IsCkLBtn = false;

	if (GetFocus() != NULL)	//내 윈도우가 포커스를 가지고 있다는 뜻
	{
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)	//마우스 좌측 버튼 클릭
		{
			if (a_IsCkLBtn == false)	//WM_LBUTTONDOWN: 대신
			{
				g_UI_Mgr.OnLButtonDown(hWnd, m_Rect);
				a_IsCkLBtn = true;
			}
		}
		else
		{
			if (a_IsCkLBtn == true)	//WM_LBUTTONUP: 대신
			{
				g_UI_Mgr.OnLButtonUp(CallLButtonUp);
				a_IsCkLBtn = false;
			}
		}
	}
	//----마우스 처리 부분

	//----게임 상태 처리 부분
	if (g_GameState == RACE_READY)
	{
		static float a_TimeTic = 0;
		a_TimeTic = a_TimeTic + m_DeltaTime;
		if (1.0f <= a_TimeTic)
		{
			if (-1 < m_Count)
			{
				m_Count--;
				if (m_Count <= -1)
				{
					g_GameState = RACE_ING;
						m_Count = 1;
				}
			}
			a_TimeTic = 0;
		}
	}
	//----게임 상태 처리 부분

	GetClientRect(hWnd, &m_Rect);

	//---------- 백그라운드 업데이트
	g_BGround.BG_Update(m_Rect);
	//---------- 백그라운드 업데이트

	//---------- 캐릭터 매니저 업데이트
	bool isRaceStart = (g_GameState == RACE_ING) ? true : false;
	g_Char_Mgr.CM_UpDate(m_DeltaTime, &m_Rect, isRaceStart, &m_Ranking);
	//---------- 캐릭터 매니저 업데이트

	//---------- UI 업데이트 부분
	g_UI_Mgr.UIMgrUpdate(m_DeltaTime, m_Count);
	//---------- UI 업데이트 부분
}

void CMyMain::MainRender(HWND hWnd)
{
	if (m_hBackDC == NULL)
		return;

	//---------- 백그라운드 랜더
	g_BGround.BG_Render(m_hBackDC, m_Rect);
	//---------- 백그라운드 랜더

	SetBkMode(m_hBackDC, TRANSPARENT);		//텍스트 배경을 뺀다.
	SetTextAlign(m_hBackDC, TA_CENTER);		//글씨 정렬함수 중앙정렬

	//---------- 캐릭터 매니저 랜더
	g_Char_Mgr.CM_Render(m_hBackDC);
	//---------- 캐릭터 매니저 랜더

	//---------- UI 그리기 부분
	g_UI_Mgr.UIMgrRender(m_hBackDC, m_Rect);
	//---------- UI 그리기 부분

	SetTextAlign(m_hBackDC, TA_LEFT);		//원래대로...
	SetBkMode(m_hBackDC, OPAQUE);			//텍스트 배경 원래대로..

	//---------- 더블 버퍼링
	static HDC hdc;
	hdc = GetDC(hWnd);
	BitBlt(hdc, 0, 0, m_Rect.right, m_Rect.bottom, m_hBackDC, 0, 0, SRCCOPY);	//----- 화면전환
	ReleaseDC(hWnd, hdc);
	//---------- 더블 버퍼링
}

void CMyMain::MainDestroy()
{
	//---------- 백그라운드 제거
	g_BGround.BG_Destroy();
	//---------- 백그라운드 제거

	//---------- 캐릭터 매니저 제거
	g_Char_Mgr.CM_Destroy();
	//---------- 캐릭터 매니저 제거

	//---------- UI 제거 부분
	g_UI_Mgr.UIMgrDestroy();
	//---------- UI 제거 부분

	//---------- 후면 BackDC 제거
	if (m_hBackBitmap != NULL)
	{
		DeleteObject(m_hBackBitmap);
		m_hBackBitmap = NULL;
	}

	if (m_hBackDC != NULL)
	{
		DeleteDC(m_hBackDC);
		m_hBackDC = NULL;
	}
	//---------- 후면 BackDC 제거

	 //---------- GDI+ 해제 //다른 모든 이미지 제거 후 맨 마지막에 한번 해야 한다.
	if (m_gdiplusToken != NULL)
	{
		GdiplusShutdown(m_gdiplusToken);
		m_gdiplusToken = NULL;
	}
	//---------- GDI+ 해제 //다른 모든 이미지 제거 후 맨 마지막에 한번 해야 한다.
}

void CMyMain::ReSetGame()
{
	m_Ranking = 0;

	//---------- 캐릭터 초기화 부분
	g_Char_Mgr.CM_ReSetPos();
	//---------- 캐릭터 초기화 부분

	m_Count = 3;

	g_GameState = GAME_READY;
}

void CMyMain::CallLButtonUp()
{
	if (g_GameState == GAME_READY)
	{
		g_GameState = RACE_READY;
	}

	if (g_GameState == RACE_END)
	{
		g_CMyMain.ReSetGame();	//게임 초기화
	}
}

CMyMain g_CMyMain;	//변수의 정의