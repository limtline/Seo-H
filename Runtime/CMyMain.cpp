#include "stdafx.h"
#include "CMyMain.h"
#include "CBackGround.h"
#include "CCharacter_Mgr.h"
#include "CCharacter.h"
#include "GlobalValue.h"
#include "CUIManager.h"

#include <Mmsystem.h>						//timeGetTime() �Լ� ����� ���Ͽ�...
#include <time.h>							//<---- �������� ��� ���Ͽ�...
#pragma comment(lib,"winmm.lib")			//timeGetTime() �Լ� ����� ���Ͽ�...

//--------------------- GDI+ Image ����� ���� ��� �߰�
#include <ole2.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;
//--------------------- GDI+ Image ����� ���� ��� �߰�

CMyMain::CMyMain()
{
}

CMyMain::~CMyMain()
{
}

void CMyMain::MainInit(HWND hWnd)
{
	srand((unsigned)time(NULL));	//���� �ʱ�ȭ

	m_LastTime = timeGetTime();		//����ð� �ʱ�ȭ

	//---------- GDI+ �ʱ�ȭ (������Ʈ �������ڸ��� �ѹ��� �� �ָ�ȴ�.)
	GdiplusStartupInput gdiplusStartInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartInput, NULL);
	//---------- GDI+ �ʱ�ȭ

	//---------- �ĸ� BackDC ����
	HDC hdc = GetDC(hWnd);		//���� DC ��������
	m_hBackDC = CreateCompatibleDC(hdc);				  //���� DC�� �ֵ��� ���纻 DC�� �ϳ� �����.
	m_hBackBitmap = CreateCompatibleBitmap(hdc,           //BackDC�� �ٿ��� Bitmap(�̹���)�� �ϳ� ����� �ش�.
					GetSystemMetrics(SM_CXSCREEN),		  //����ȭ���� ���� ������
					GetSystemMetrics(SM_CYSCREEN));		  //����ȭ���� ���� ������
	SelectObject(m_hBackDC, m_hBackBitmap);				  //BackDC�� Bitmap(�̹���)�� �ٿ��ش�
	ReleaseDC(hWnd, hdc);
	//---------- �ĸ� BackDC ����

	//---------- ��׶��� �ʱ�ȭ
	g_BGround.BG_Init(hWnd);
	//---------- ��׶��� �ʱ�ȭ

	//---------- ĳ���� �Ŵ��� �ʱ�ȭ
	g_Char_Mgr.CM_Init(hWnd);
	//---------- ĳ���� �Ŵ��� �ʱ�ȭ

	//---------- UI �ʱ�ȭ �κ�
	g_UI_Mgr.UIMgrInit();
	//---------- UI �ʱ�ȭ �κ�
}

void CMyMain::MainUpdate(HWND hWnd)
{
	//---------- DeltaTime ��� ���� ���
	timeBeginPeriod(1);    //timeGetTime() �Լ��� ���е��� ���̱� ���� �Լ�
	m_CurTime = timeGetTime();
	m_DeltaTime = (m_CurTime - m_LastTime) * 0.001f;
	m_LastTime = m_CurTime;
	timeEndPeriod(1);
	//---------- DeltaTime ��� ���� ���

	if (m_hBackDC == NULL)
		return;

	//----Ű���� ó�� �κ�
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
				ReSetGame();	//���� �ʱ�ȭ
			}

			a_Once_R = false;
		}
	}
	else
		a_Once_R = true;
	//----Ű���� ó�� �κ�

	//----���콺 ó�� �κ�
	static bool a_IsCkLBtn = false;

	if (GetFocus() != NULL)	//�� �����찡 ��Ŀ���� ������ �ִٴ� ��
	{
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)	//���콺 ���� ��ư Ŭ��
		{
			if (a_IsCkLBtn == false)	//WM_LBUTTONDOWN: ���
			{
				g_UI_Mgr.OnLButtonDown(hWnd, m_Rect);
				a_IsCkLBtn = true;
			}
		}
		else
		{
			if (a_IsCkLBtn == true)	//WM_LBUTTONUP: ���
			{
				g_UI_Mgr.OnLButtonUp(CallLButtonUp);
				a_IsCkLBtn = false;
			}
		}
	}
	//----���콺 ó�� �κ�

	//----���� ���� ó�� �κ�
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
	//----���� ���� ó�� �κ�

	GetClientRect(hWnd, &m_Rect);

	//---------- ��׶��� ������Ʈ
	g_BGround.BG_Update(m_Rect);
	//---------- ��׶��� ������Ʈ

	//---------- ĳ���� �Ŵ��� ������Ʈ
	bool isRaceStart = (g_GameState == RACE_ING) ? true : false;
	g_Char_Mgr.CM_UpDate(m_DeltaTime, &m_Rect, isRaceStart, &m_Ranking);
	//---------- ĳ���� �Ŵ��� ������Ʈ

	//---------- UI ������Ʈ �κ�
	g_UI_Mgr.UIMgrUpdate(m_DeltaTime, m_Count);
	//---------- UI ������Ʈ �κ�
}

void CMyMain::MainRender(HWND hWnd)
{
	if (m_hBackDC == NULL)
		return;

	//---------- ��׶��� ����
	g_BGround.BG_Render(m_hBackDC, m_Rect);
	//---------- ��׶��� ����

	SetBkMode(m_hBackDC, TRANSPARENT);		//�ؽ�Ʈ ����� ����.
	SetTextAlign(m_hBackDC, TA_CENTER);		//�۾� �����Լ� �߾�����

	//---------- ĳ���� �Ŵ��� ����
	g_Char_Mgr.CM_Render(m_hBackDC);
	//---------- ĳ���� �Ŵ��� ����

	//---------- UI �׸��� �κ�
	g_UI_Mgr.UIMgrRender(m_hBackDC, m_Rect);
	//---------- UI �׸��� �κ�

	SetTextAlign(m_hBackDC, TA_LEFT);		//�������...
	SetBkMode(m_hBackDC, OPAQUE);			//�ؽ�Ʈ ��� �������..

	//---------- ���� ���۸�
	static HDC hdc;
	hdc = GetDC(hWnd);
	BitBlt(hdc, 0, 0, m_Rect.right, m_Rect.bottom, m_hBackDC, 0, 0, SRCCOPY);	//----- ȭ����ȯ
	ReleaseDC(hWnd, hdc);
	//---------- ���� ���۸�
}

void CMyMain::MainDestroy()
{
	//---------- ��׶��� ����
	g_BGround.BG_Destroy();
	//---------- ��׶��� ����

	//---------- ĳ���� �Ŵ��� ����
	g_Char_Mgr.CM_Destroy();
	//---------- ĳ���� �Ŵ��� ����

	//---------- UI ���� �κ�
	g_UI_Mgr.UIMgrDestroy();
	//---------- UI ���� �κ�

	//---------- �ĸ� BackDC ����
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
	//---------- �ĸ� BackDC ����

	 //---------- GDI+ ���� //�ٸ� ��� �̹��� ���� �� �� �������� �ѹ� �ؾ� �Ѵ�.
	if (m_gdiplusToken != NULL)
	{
		GdiplusShutdown(m_gdiplusToken);
		m_gdiplusToken = NULL;
	}
	//---------- GDI+ ���� //�ٸ� ��� �̹��� ���� �� �� �������� �ѹ� �ؾ� �Ѵ�.
}

void CMyMain::ReSetGame()
{
	m_Ranking = 0;

	//---------- ĳ���� �ʱ�ȭ �κ�
	g_Char_Mgr.CM_ReSetPos();
	//---------- ĳ���� �ʱ�ȭ �κ�

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
		g_CMyMain.ReSetGame();	//���� �ʱ�ȭ
	}
}

CMyMain g_CMyMain;	//������ ����