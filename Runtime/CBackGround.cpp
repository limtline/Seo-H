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
	//------ ȭ���� ����� ���� ��� ���� ����
	m_hPen = CreatePen(PS_SOLID, 1, RGB(255, 211, 176));
	m_hBrush = CreateSolidBrush(RGB(255, 211, 176));
	//------ ȭ���� ����� ���� ��� ���� ����

	//------ Ʈ�� �׸���� �� ����
	m_hLinePen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	m_hHLPen= CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
	//------ Ʈ�� �׸���� �� ����
	
}

void CBackGround::BG_Update(RECT & a_RT)
{
	//---------- ������ â ������ ��ȭ�� ���� ���� ��ǥ ������ �ֱ�
	m_StLine1.x = (LONG)((float)a_RT.right * 0.015f) + 92;
	m_StLine1.y = (LONG)((float)a_RT.bottom * 0.01f);

	m_StLine2.x = (LONG)((float)a_RT.right * 0.015f) + 92;
	m_StLine2.y = (LONG)((float)a_RT.bottom * 0.99f);

	m_EndLine1.x = (LONG)((float)a_RT.right * 0.985f) - 92;
	m_EndLine1.y = (LONG)((float)a_RT.bottom * 0.01f);

	m_EndLine2.x = (LONG)((float)a_RT.right * 0.985f) - 92;
	m_EndLine2.y = (LONG)((float)a_RT.bottom * 0.99f);
	//---------- ������ â ������ ��ȭ�� ���� ���� ��ǥ ������ �ֱ�
}

void CBackGround::BG_Render(HDC a_hdc, RECT & a_RT)
{
	//------ DC ȭ�� �����
	m_hOldBrush = (HBRUSH)SelectObject(a_hdc, m_hBrush);
	m_hOldPen = (HPEN)SelectObject(a_hdc, m_hPen);
	Rectangle(a_hdc, 0, 0, a_RT.right, a_RT.bottom);
	SelectObject(a_hdc, m_hOldBrush);
	SelectObject(a_hdc, m_hOldPen);
	//------ DC ȭ�� �����

	 //--------������ - ���,������:
	m_hOldPen = (HPEN)SelectObject(a_hdc, m_hLinePen);

	MoveToEx(a_hdc, m_StLine1.x, m_StLine1.y, NULL);    //��߼� �׸���
	LineTo(a_hdc, m_StLine2.x, m_StLine2.y);

	MoveToEx(a_hdc, m_EndLine1.x, m_EndLine1.y, NULL);  //������ �׸���
	LineTo(a_hdc, m_EndLine2.x, m_EndLine2.y);

	SelectObject(a_hdc, m_hOldPen);//���� ������ ��ü
	//--------������ - ���,������:

	//--------������
	static int  a_CacPosX;
	static int  a_CacPosY;
	static int  a_CacYTab;
	m_hHLOldPen = SelectObject(a_hdc, m_hHLPen);

	a_CacPosX = (int)((float)a_RT.right * 0.01f);
	a_CacYTab = a_RT.bottom / 7;
	a_CacPosY = a_CacYTab;

	for (int i = 0; i < 6; i++)
	{
		MoveToEx(a_hdc, a_CacPosX, a_CacPosY + (i * a_CacYTab), NULL); //��ǥ�� �̵�
		LineTo(a_hdc, a_RT.right - a_CacPosX, a_CacPosY + (i * a_CacYTab)); //�� ��ǥ���� �ش���ġ���� ���� �׾��        
	}
	SelectObject(a_hdc, m_hHLOldPen);//���� ������ ��ü
	//--------������
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