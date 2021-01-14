#include "stdafx.h"
#include "CCharacter_Mgr.h"
#include "GlobalValue.h"

CCharacter_Mgr::CCharacter_Mgr()
{
}


CCharacter_Mgr::~CCharacter_Mgr()
{
}

void CCharacter_Mgr::CM_Init(HWND a_hWnd)
{
	m_hWnd = a_hWnd;

	Image* a_LoadImg = NULL;

	a_LoadImg = Image::FromFile(_T(".\\RscTrunk\\Image\\black_1.png"));  //[0] �� �̹��� �ε�
	m_ImgList.push_back(a_LoadImg);

	a_LoadImg = Image::FromFile(_T(".\\RscTrunk\\Nanami\\left_walk\\n030.png")); //[1] �� �̹��� �ε�
	m_ImgList.push_back(a_LoadImg);

	a_LoadImg = Image::FromFile(_T(".\\RscTrunk\\Image\\Green_1.png")); //[2] �� �̹��� �ε�
	m_ImgList.push_back(a_LoadImg);

	a_LoadImg = Image::FromFile(_T(".\\RscTrunk\\Image\\Brown_1.png")); //[3] �� �̹��� �ε�
	m_ImgList.push_back(a_LoadImg);

	a_LoadImg = Image::FromFile(_T(".\\RscTrunk\\Image\\h_1.png")); //[4] �� �̹��� �ε�
	m_ImgList.push_back(a_LoadImg);

	a_LoadImg = Image::FromFile(_T(".\\RscTrunk\\Image\\red_1.png")); //[5] �� �̹��� �ε�
	m_ImgList.push_back(a_LoadImg);

	a_LoadImg = Image::FromFile(_T(".\\RscTrunk\\Image\\White_1.png")); //[6] �� �̹��� �ε�
	m_ImgList.push_back(a_LoadImg);

	for (int aa = 0; aa < 7; aa++)
	{
		m_CharList[aa].LoadAnimList(aa);  //�ִϸ��̼� �̹��� �ε� 

		m_CharList[aa].CChar_Init(m_ImgList[aa]); //�̹��� �ε� �� �ʱ�ȭ
	}
	CM_ReSetPos(); //���� ��ġ�� ��ǥ �̵�
}

void CCharacter_Mgr::CM_ReSetPos()
{
	RECT a_Rect;
	GetClientRect(m_hWnd, &a_Rect);	//Ŭ���̾�Ʈ ȭ���� ũ�⸦ �޴´�.
	//------ ĳ���� �ʱ�ȭ �κ�
	int a_PosX = (int)((float)a_Rect.right * 0.015f);
	int a_PosY = (int)((float)a_Rect.bottom * 0.03f);
	int a_YTab = a_Rect.bottom / 7;
	for(int aa = 0; aa < 7; aa++)
	{
		m_CharList[aa].CChar_ReSetPos(aa, a_PosX, a_PosY + (aa*a_YTab));
	}
	//------ ĳ���� �ʱ�ȭ �κ�

	m_RandTic = 10.0f;
}

void CCharacter_Mgr::CM_UpDate(double a_DeltaTime, RECT * a_rt, bool a_isRaceStart, int * a_Ranking)
{
	if (g_GameState == RACE_ING)	//ĳ���� �̵��ӵ��� �����ϰ� �����ϴ� �κ�
	{
		static int a_TrackFor = 0;
		m_RandTic = m_RandTic + a_DeltaTime;
		if (0.3f <= m_RandTic)
		{
			m_CharList[a_TrackFor].m_MoveSpeed = (float)((rand() % 140) + 50);
			a_TrackFor++;
			if (7 <= a_TrackFor)
			{
				a_TrackFor = 0;
			}
			m_RandTic = 0;
		}
	}

	for (int aa = 0; aa < 7; aa++)
	{
		m_CharList[aa].CChar_Update(a_DeltaTime, a_rt, a_isRaceStart, a_Ranking);
	}

	//-----------���� ���� ���� üũ
	static bool m_isEnd;
	m_isEnd = true;
	for (int bb = 0; bb < 7; bb++)
	{
		if (m_CharList[bb].m_Ranking < 0)
		{
			m_isEnd = false;
			break;
		}
	}

	if (m_isEnd == true)
	{
		g_GameState = RACE_END;
	}
	//-----------���� ���� ���� üũ
}

void CCharacter_Mgr::CM_Render(HDC a_hdc)
{
	for (int aa = 0; aa < 7; aa++)
	{
		m_CharList[aa].CChar_Render(a_hdc);	//-----------ĳ���� �׸��� �κ�
	}
}

void CCharacter_Mgr::CM_Destroy()
{
	//-------- ĳ���� ���ҽ� ����
	for (int aa = 0; aa < 7; aa++)
	{
		m_CharList[aa].CChar_Destroy();
	}
	//-------- ĳ���� ���ҽ� ����

	//-------- �̹��� ����
	for (int ii = 0; ii < m_ImgList.size(); ii++)
	{
		if (m_ImgList[ii] != NULL)
		{
			delete m_ImgList[ii];
			m_ImgList[ii] = NULL;
		}
	}
	m_ImgList.clear();
	//-------- �̹��� ����
}

CCharacter_Mgr g_Char_Mgr;