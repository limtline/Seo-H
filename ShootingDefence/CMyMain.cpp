#include "stdafx.h"
#include "CMyMain.h"
#include "CBackGround.h"
#include "CHero.h"
#include "CBullet_Mgr.h"
#include "CMonster_Mgr.h"
#include "GlobalValue.h"
#include "CUI_Manager.h"
#include "CItem_Mgr.h"

//----- GDI+ Image 사용을 위한 헤더 추가
#include <ole2.h>  
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;
//----- GDI+ Image 사용을 위한 헤더 추가

#include <Mmsystem.h>                //timeGetTime() 함수 사용을 위하여...
#include <time.h>					 //<---랜덤값을 얻기 위하여...
#pragma comment(lib, "winmm.lib")    //timeGetTime() 함수 사용을 위하여...

CMyMain::CMyMain()
{
}

CMyMain::~CMyMain()
{
}

void CMyMain::MainInit(HWND hWnd)
{
	m_hWnd = hWnd;
	GetClientRect(hWnd, &m_Rect); // 클라이언트 화면의 크기를 받는다

	srand((unsigned)time(NULL));//----rand 초기화
	//DeltaTime 얻어 오는 방법
	m_LastTime = timeGetTime();          //현재 시간 가져오기

	//--- GDI+ 초기화
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
	//gdiplusToken <-- 마지막에 GdiplusShutdown에서 사용하므로 저장필수.
	//--- GDI+ 초기화

	//-------- 후면 BackDC 생성
	HDC hdc = GetDC(hWnd); //메인 DC 가져오기...
	m_hBackDC = CreateCompatibleDC(hdc);    //메인 DC의 쌍둥이 복사본 DC를 하나 만든다.
	m_hBackBitmap = CreateCompatibleBitmap(hdc, //BackDC에 붙여줄 Bitmap(이미지)를 하나 만들어 온다.
		GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	SelectObject(m_hBackDC, m_hBackBitmap);     //BackDC에 Bitmap(이미지)을 붙여준다.
	ReleaseDC(hWnd, hdc);
	//-------- 후면 BackDC 생성

	for (int ii = 0; ii < CTT_Length; ii++)
	{
		CAnimData* a_Node = new CAnimData();
		a_Node->LoadAnimList((CT_Type)ii);
		m_CharAniList.push_back(a_Node);
	}

	//-------- GUI 초기화
	g_GUI_Mgr.UIMgr_Init();
	//-------- GUI 초기화

	//-------- 백그라운드 초기화
	g_BGround.BG_Init(hWnd);
	//-------- 백그라운드 초기화

	//-------- 주인공 초기화
	g_Hero.Init_Unit(hWnd);
	//-------- 주인공 초기화

	//-------- 총알 매니저 초기화
	g_Bullet_Mgr.BLMgr_Init();
	//-------- 총알 매니저 초기화

	//-------- 몬스터 매니저 초기화
	g_Mon_Mgr.MonMgr_Init(hWnd);
	//-------- 몬스터 매니저 초기화

	//-------- 아이템 매니저 초기화
	g_ItemMgr.ItemMgr_Init();
	//-------- 아이템 매니저 초기화
}

void CMyMain::MainUpdate(HWND hWnd)
{
	//DeltaTime 얻어 오는 방법
	timeBeginPeriod(1);         //정밀도를 높이기 위한 함수 사용  
	m_CurTime = timeGetTime();
	m_DeltaTime = ((m_CurTime - m_LastTime)*0.001);
	m_LastTime = m_CurTime;
	timeEndPeriod(1);
	//DeltaTime 얻어 오는 방법

	if (m_hBackDC == NULL)
		return;

	GetClientRect(hWnd, &m_Rect);

	if (g_GameState == GAME_START)
	{
		static float a_TimeTic = 0;
		a_TimeTic = a_TimeTic + m_DeltaTime;
		if (1.0f <= a_TimeTic)
		{
			if (0 < g_TimeOut)
			{
				g_TimeOut--;
				if (g_TimeOut <= 0)
				{
					g_GameState = NEXT_LEVEL;
					g_GUI_Mgr.m_ShopOnOff = true;
					g_DiffLevel++;
					g_TimeOut = 0;
				}
			}
			a_TimeTic = 0;
		}


		////-------- 백그라운드 업데이트
		//g_BGround.BG_Update();
		////-------- 백그라운드 업데이트

		//-------- 주인공 업데이트
		g_Hero.Update_Unit(m_DeltaTime, m_Rect);
		//-------- 주인공 업데이트

		//-------- 몬스터 매니저 업데이트
		g_Mon_Mgr.MonMgr_Update(m_DeltaTime, hWnd, g_Hero);
		//-------- 몬스터 매니저 업데이트

		//-------- 총알 매니저 업데이트
		g_Bullet_Mgr.BLMgr_Update(m_DeltaTime, hWnd, &g_Mon_Mgr);
		//-------- 총알 매니저 업데이트

		//-------- 아이템 매니저 업데이트
		g_ItemMgr.ItemMgr_Update(m_DeltaTime);
		//-------- 아이템 매니저 업데이트

	}//if (g_GameState == GAME_START)

	//-------- GUI 업데이트
	g_GUI_Mgr.UIMgr_Update(hWnd, m_DeltaTime, NextLevel, ReSetGame);
	//-------- GUI 업데이트
}

void CMyMain::MainRender(HWND hWnd)
{
	if (m_hBackDC == NULL)
		return;

	if (g_GameState == GAME_START)
	{
		//-------- 백그라운드 랜더
		g_BGround.BG_Render(m_hBackDC, m_Rect);
		//-------- 백그라운드 랜더

		//---- 아이템 그려주기
		g_ItemMgr.ItemMgr_Render(m_hBackDC);
		//---- 아이템 그려주기

		//--- 몬스터 이미지 그리기...
		g_Mon_Mgr.MonMgr_Render(m_hBackDC);
		//--- 몬스터 이미지 그리기...

		//----총알 그려주기
		g_Bullet_Mgr.BLMgr_Render(m_hBackDC);
		//----총알 그려주기

		//----주인공 그려주기
		g_Hero.Render_Unit(m_hBackDC);
		//----주인공 그려주기

	}//if (g_GameState == GAME_START)

	//----GUI 그려주기
	g_GUI_Mgr.UIMgr_Render(m_hBackDC, m_Rect);
	//----GUI 그려주기

	//------------------- 더블 버퍼링 (화면전환)
	static HDC hdc;
	hdc = GetDC(hWnd);
	BitBlt(hdc, 0, 0, m_Rect.right, m_Rect.bottom, m_hBackDC, 0, 0, SRCCOPY);
	ReleaseDC(hWnd, hdc);
	//------------------ 더블 버퍼링 (화면전환)
}

void CMyMain::MainDestroy()
{
	//------------애니메이션 데이터 삭제 부분
	for (int ii = 0; ii < m_CharAniList.size(); ii++)
	{
		if (m_CharAniList[ii] != NULL)
		{
			delete m_CharAniList[ii];
			m_CharAniList[ii] = NULL;
		}
	}
	m_CharAniList.clear();
	//------------애니메이션 데이터 삭제 부분

	//--- 아이템 지우기...
	g_ItemMgr.ItemMgr_Destroy();
	//--- 아이템 지우기...

	//--- 몬스터 지우기...
	g_Mon_Mgr.MonMgr_Destroy();
	//--- 몬스터 지우기...

	//----총알 지우기
	g_Bullet_Mgr.BLMgr_Destroy();
	//----총알 지우기

	//----주인공 지우기
	g_Hero.Destroy_Unit();
	//----주인공 지우기

	//-------- 백그라운드 제거
	g_BGround.BG_Destroy();
	//-------- 백그라운드 제거

	//-------- GUI 지우기...
	g_GUI_Mgr.UIMgr_Destroy();
	//-------- GUI 지우기...

	//-------- 후면 BackDC 제거
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
	//-------- 후면 BackDC 제거

	//--- GDI+ 해제  
	if (m_gdiplusToken != NULL)
	{
		GdiplusShutdown(m_gdiplusToken);
		m_gdiplusToken = NULL;
	}
	//--- GDI+ 해제 
}

void CMyMain::NextLevel()
{
	//게임 초기화
	g_TimeOut = 30;

	//----남은 총알 제거 
	g_Bullet_Mgr.ReSrcClear();  //주인공 텍스쳐와 남은 총알 제거
	//----남은 총알 제거 

	//----남은 아이템 제거
	g_ItemMgr.ReSrcClear();
	//----남은 아이템 제거

	//----남은 몬스터 제거 
	g_Mon_Mgr.ReSrcClear();
	//----남은 몬스터 제거 

	//---------------주인공 초기화 
	static RECT a_RT;
	GetClientRect(g_CMyMain.m_hWnd, &a_RT);
	g_Hero.m_CurPos.x = (float)(a_RT.right / 2.0f);
	g_Hero.m_CurPos.y = (float)(a_RT.bottom / 2.0f);

	g_Hero.m_SdOnTime = 0.0f;

	//g_Hero.ChangeState(Idle);
	//---------------주인공 초기화 

	g_GameState = GAME_START;
}

void CMyMain::ReSetGame()
{
	//게임 초기 화
	g_TimeOut = 30;

	g_DiffLevel = 1;

	//----남은 총알 제거 
	g_Bullet_Mgr.ReSrcClear();  //주인공 텍스쳐와 남은 총알 제거
	//----남은 총알 제거 

	//----남은 아이템 제거
	g_ItemMgr.ReSrcClear();
	//----남은 아이템 제거

	//----남은 몬스터 제거 
	g_Mon_Mgr.ReSrcClear();
	//----남은 몬스터 제거 

	//---------------주인공 초기화 
	static RECT a_RT;
	GetClientRect(g_CMyMain.m_hWnd, &a_RT);
	g_Hero.m_CurPos.x = (float)(a_RT.right / 2.0f);
	g_Hero.m_CurPos.y = (float)(a_RT.bottom / 2.0f);

	g_Hero.m_SdOnTime = 0.0f;

//	g_Hero.ChangeState(Idle);

	g_Hero.m_CurHP = g_Hero.m_MaxHp;
	g_Hero.m_KillCount = 0;
	g_Hero.m_SkillCount = 5;
	g_Hero.m_ShieldCount = 5;
	g_Hero.m_STDelay = 0.5f;
	//---------------주인공 초기화 

	g_GameState = GAME_READY;

}

CMyMain g_CMyMain;