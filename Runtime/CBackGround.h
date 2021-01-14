#pragma once

class CBackGround
{
private:
	//--------------------ȭ���� Ư���� ������ �����
	HPEN m_hPen;
	HPEN m_hOldPen;
	HBRUSH m_hBrush;
	HBRUSH m_hOldBrush;
	//--------------------ȭ���� Ư���� ������ �����

	//--------------------Ʈ�� �׸��� �� ���� 
	//------- ������ �׸��� ����
	HPEN m_hLinePen;
	POINT m_StLine1, m_StLine2;
	POINT m_EndLine1, m_EndLine2;
	//------- ������ �׸��� ����

	//------- ������ �׸��� ����
	HPEN m_hHLPen;		//Horizontal Line
	//------- ������ �׸��� ����
	HGDIOBJ m_hHLOldPen;
	//--------------------Ʈ�� �׸��� �� ���� 

public:
	CBackGround();
	~CBackGround();

public:
	void BG_Init(HWND a_hWnd);					//�ʱ�ȭ �Լ�
	void BG_Update(RECT &a_RT);					//������ ���� ���κ�
	void BG_Render(HDC a_hdc, RECT &a_RT);		//������ �׸��� �κ�
	void BG_Destroy();							//���� ó�� �κ�
};

extern CBackGround g_BGround;