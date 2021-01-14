#pragma once
#include "CCharacter.h"

#include <vector>
using namespace std;

class CCharacter_Mgr
{
	HWND m_hWnd;
	vector<Image*> m_ImgList;		//ĳ���� �� �ִ� ���� �̹��� �ε� ����
	CCharacter m_CharList[7];		//ĳ���� �� 7����

	float m_RandTic;				//�ֱ������� ������ �ӵ��� ���� �����ֱ� ���� ����

public:
	CCharacter_Mgr();
	~CCharacter_Mgr();

public:
	void CM_Init(HWND a_hWnd);		//�ʱ�ȭ �Լ�
	void CM_ReSetPos();
	void CM_UpDate(double a_DeltaTime, RECT *a_rt = NULL,
		bool a_isRaceStart = false, int *a_Ranking = NULL);		 //������ ���� ��� �κ�
	void CM_Render(HDC a_hdc);		//������ �׸��� �κ�
	void CM_Destroy();
};

extern CCharacter_Mgr g_Char_Mgr;