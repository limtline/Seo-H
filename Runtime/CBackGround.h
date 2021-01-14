#pragma once

class CBackGround
{
private:
	//--------------------화면을 특정한 색으로 지우기
	HPEN m_hPen;
	HPEN m_hOldPen;
	HBRUSH m_hBrush;
	HBRUSH m_hOldBrush;
	//--------------------화면을 특정한 색으로 지우기

	//--------------------트랙 그리기 용 변수 
	//------- 세로줄 그리기 변수
	HPEN m_hLinePen;
	POINT m_StLine1, m_StLine2;
	POINT m_EndLine1, m_EndLine2;
	//------- 세로줄 그리기 변수

	//------- 가로줄 그리기 변수
	HPEN m_hHLPen;		//Horizontal Line
	//------- 가로줄 그리기 변수
	HGDIOBJ m_hHLOldPen;
	//--------------------트랙 그리기 용 변수 

public:
	CBackGround();
	~CBackGround();

public:
	void BG_Init(HWND a_hWnd);					//초기화 함수
	void BG_Update(RECT &a_RT);					//게임의 연산 담당부분
	void BG_Render(HDC a_hdc, RECT &a_RT);		//게임의 그리기 부분
	void BG_Destroy();							//종료 처리 부분
};

extern CBackGround g_BGround;