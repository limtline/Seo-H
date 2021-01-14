#pragma once

class CMyMain
{
	ULONG_PTR m_gdiplusToken = NULL;		//GDI+ 초기화를 위한 글로벌 변수

	//-----------------BackDC 생성용 변수
	HDC			m_hBackDC = NULL;
	HBITMAP		m_hBackBitmap = NULL;
	//-----------------BackDC 생성용 변수

	RECT		m_Rect;

	//-----------------델타 타임을 얻기 위한 변수
	double m_DeltaTime;
	DWORD m_LastTime = 0;
	DWORD m_CurTime = 0;
	//-----------------델타 타임을 얻기 위한 변수

	int m_Ranking = 0;			//랭킹 판정하기 위한 변수
	int m_Count = 3;			//스타트 카운트 3, 2, 1 시작

public:
	CMyMain();
	~CMyMain();

public:
	void MainInit(HWND hWnd);
	void MainUpdate(HWND hWnd);
	void MainRender(HWND hWnd);
	void MainDestroy();

	void ReSetGame();			//게임 초기화

	static void CallLButtonUp();	//버튼업 발생시 처리함수
};

extern CMyMain g_CMyMain;	//변수의 선언