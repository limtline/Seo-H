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

	a_LoadImg = Image::FromFile(_T(".\\RscTrunk\\Image\\black_1.png"));  //[0] 말 이미지 로딩
	m_ImgList.push_back(a_LoadImg);

	a_LoadImg = Image::FromFile(_T(".\\RscTrunk\\Nanami\\left_walk\\n030.png")); //[1] 말 이미지 로딩
	m_ImgList.push_back(a_LoadImg);

	a_LoadImg = Image::FromFile(_T(".\\RscTrunk\\Image\\Green_1.png")); //[2] 말 이미지 로딩
	m_ImgList.push_back(a_LoadImg);

	a_LoadImg = Image::FromFile(_T(".\\RscTrunk\\Image\\Brown_1.png")); //[3] 말 이미지 로딩
	m_ImgList.push_back(a_LoadImg);

	a_LoadImg = Image::FromFile(_T(".\\RscTrunk\\Image\\h_1.png")); //[4] 말 이미지 로딩
	m_ImgList.push_back(a_LoadImg);

	a_LoadImg = Image::FromFile(_T(".\\RscTrunk\\Image\\red_1.png")); //[5] 말 이미지 로딩
	m_ImgList.push_back(a_LoadImg);

	a_LoadImg = Image::FromFile(_T(".\\RscTrunk\\Image\\White_1.png")); //[6] 말 이미지 로딩
	m_ImgList.push_back(a_LoadImg);

	for (int aa = 0; aa < 7; aa++)
	{
		m_CharList[aa].LoadAnimList(aa);  //애니메이션 이미지 로딩 

		m_CharList[aa].CChar_Init(m_ImgList[aa]); //이미지 로딩 등 초기화
	}
	CM_ReSetPos(); //시작 위치로 좌표 이동
}

void CCharacter_Mgr::CM_ReSetPos()
{
	RECT a_Rect;
	GetClientRect(m_hWnd, &a_Rect);	//클라이언트 화면의 크기를 받는다.
	//------ 캐릭터 초기화 부분
	int a_PosX = (int)((float)a_Rect.right * 0.015f);
	int a_PosY = (int)((float)a_Rect.bottom * 0.03f);
	int a_YTab = a_Rect.bottom / 7;
	for(int aa = 0; aa < 7; aa++)
	{
		m_CharList[aa].CChar_ReSetPos(aa, a_PosX, a_PosY + (aa*a_YTab));
	}
	//------ 캐릭터 초기화 부분

	m_RandTic = 10.0f;
}

void CCharacter_Mgr::CM_UpDate(double a_DeltaTime, RECT * a_rt, bool a_isRaceStart, int * a_Ranking)
{
	if (g_GameState == RACE_ING)	//캐릭터 이동속도를 랜덤하게 변경하는 부분
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

	//-----------게임 종료 조건 체크
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
	//-----------게임 종료 조건 체크
}

void CCharacter_Mgr::CM_Render(HDC a_hdc)
{
	for (int aa = 0; aa < 7; aa++)
	{
		m_CharList[aa].CChar_Render(a_hdc);	//-----------캐릭터 그리는 부분
	}
}

void CCharacter_Mgr::CM_Destroy()
{
	//-------- 캐릭터 리소스 제거
	for (int aa = 0; aa < 7; aa++)
	{
		m_CharList[aa].CChar_Destroy();
	}
	//-------- 캐릭터 리소스 제거

	//-------- 이미지 제거
	for (int ii = 0; ii < m_ImgList.size(); ii++)
	{
		if (m_ImgList[ii] != NULL)
		{
			delete m_ImgList[ii];
			m_ImgList[ii] = NULL;
		}
	}
	m_ImgList.clear();
	//-------- 이미지 제거
}

CCharacter_Mgr g_Char_Mgr;