#pragma once

class CMyMain
{
	ULONG_PTR m_gdiplusToken = NULL;		//GDI+ �ʱ�ȭ�� ���� �۷ι� ����

	//-----------------BackDC ������ ����
	HDC			m_hBackDC = NULL;
	HBITMAP		m_hBackBitmap = NULL;
	//-----------------BackDC ������ ����

	RECT		m_Rect;

	//-----------------��Ÿ Ÿ���� ��� ���� ����
	double m_DeltaTime;
	DWORD m_LastTime = 0;
	DWORD m_CurTime = 0;
	//-----------------��Ÿ Ÿ���� ��� ���� ����

	int m_Ranking = 0;			//��ŷ �����ϱ� ���� ����
	int m_Count = 3;			//��ŸƮ ī��Ʈ 3, 2, 1 ����

public:
	CMyMain();
	~CMyMain();

public:
	void MainInit(HWND hWnd);
	void MainUpdate(HWND hWnd);
	void MainRender(HWND hWnd);
	void MainDestroy();

	void ReSetGame();			//���� �ʱ�ȭ

	static void CallLButtonUp();	//��ư�� �߻��� ó���Լ�
};

extern CMyMain g_CMyMain;	//������ ����