#pragma once
#include "CAnimData.h"

class CMyMain
{
	ULONG_PTR m_gdiplusToken = NULL;

	HWND	  m_hWnd = NULL;
	RECT      m_Rect;

	//------------------ BackDC ������ ����
	HDC       m_hBackDC = NULL;
	HBITMAP   m_hBackBitmap = NULL;
	//------------------ BackDC ������ ����

	//--------------------- ��Ÿ Ÿ���� ��� ���� ����
	double m_DeltaTime;
	DWORD  m_LastTime = 0;
	DWORD  m_CurTime = 0;
	//--------------------- ��Ÿ Ÿ���� ��� ���� ����

public:
	vector<CAnimData*> m_CharAniList;

public:
	CMyMain();
	~CMyMain();

public:
	void MainInit(HWND hWnd);
	void MainUpdate(HWND hWnd);
	void MainRender(HWND hWnd);
	void MainDestroy();

	static void NextLevel();    //�����ܰ�
	static void ReSetGame();    //���� �ʱ�ȭ (�ٽ��ϱ�...)
};

extern CMyMain g_CMyMain;  //������ ����