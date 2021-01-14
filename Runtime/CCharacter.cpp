#include "stdafx.h"
#include "CCharacter.h"


HFONT CCharacter::m_hFont = NULL;
HFONT CCharacter::m_hArrvFont = NULL;

CCharacter::CCharacter()
{
	for (int i = 0; i < AniLength; i++)
	{
		CMotion *a_Node = new CMotion();
		m_MotionList.push_back(a_Node);
	}

	m_CurAniState = None;
	m_NowImgCount = 0;
	m_CurAniInx = 0;			//진행 Ani Index
	m_AniTickCount = 0;			//다음플레임까지 시간 Add
	m_EachAniDelay = 0.2f;		//플레임 간격시간
}


CCharacter::~CCharacter()
{
}

void CCharacter::CChar_Init(Image * a_LoadImg)
{
	if (m_SocketImg == NULL)
	{
		m_SocketImg = a_LoadImg;
	}
	if (m_hFont == NULL)
	{
		m_hFont = CreateFont(15, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("돋움"));
	}

	if (m_hArrvFont == NULL)
	{
		m_hArrvFont = CreateFont(25, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("궁서"));
	}
		
}

void CCharacter::CChar_ReSetPos(int a_TrackNb, LONG a_XX, LONG a_YY)
{
	m_TrackNumber = a_TrackNb;

	m_CurPos.x = a_XX;
	m_CurPos.y = a_YY;

	m_CacAddPosX = (float)m_CurPos.x;

	m_MoveSpeed = 170.0;		//이동속도 초기화
	m_Ranking = -1;				//도착시 등수 초기화

	ChangeState(Race_Ready);
}

void CCharacter::CChar_Update(double a_DeltaTime, RECT * a_rt, bool a_isRaceStart, int * a_Ranking)
{
	if (a_rt == NULL)
		return;

	//static이면 글로벌처럼 모든 캐릭터가 변수를 공유하게 된다.
	static int a_PosY;	//이 부분은 메모리에서 한번만 잡기위해 static으로 함
	static int a_YTab;
	a_PosY = (int)((float)a_rt->bottom * 0.03f);	//중복 사용한 것 같아 보이지만 
	a_YTab = a_rt->bottom / 7;	//중복 사용한 것 같아 보이지만 매번 Update로

	if (m_TrackNumber == 1 || m_TrackNumber == 4)
	{
		m_CurPos.y = a_PosY + (m_TrackNumber * a_YTab) - 12;	//나나미, 호노카
	}
	else
		m_CurPos.y = a_PosY + (m_TrackNumber * a_YTab);

	//--------- 말 이동 처리
	if (a_isRaceStart == true)
	{
		m_CacAddPosX = m_CacAddPosX + (a_DeltaTime * m_MoveSpeed);
		m_CurPos.x = (LONG)m_CacAddPosX;

		static float a_EndLine;
		a_EndLine = (LONG)((float)a_rt->right * 0.985f) - 92;
		if (a_EndLine < m_CurPos.x)	//도착 체크
		{
			if (m_Ranking < 0)
			{
				m_Ranking = *a_Ranking; ;			//도착시 등수
				(*a_Ranking)++;
			}

			ChangeState(Idle);

			m_CurPos.x = a_EndLine;
		}
		else
		{
			ChangeState(Left_Walk);
		}
	}
	//--------- 말 이동 처리

	//--------------------애니메이션 플레임 계산 부분
	AniFrameUpDate(a_DeltaTime);
	//--------------------애니메이션 플레임 계산 부분
}

void CCharacter::CChar_Render(HDC a_hdc)
{
	if (m_SocketImg == NULL)	//애니 소켓에 뭔가 꼽혀 있는지 확인해 보는 안전장치
		return;

	//--------------------말그리기
	m_ImgSizeX = (INT)(m_SocketImg->GetWidth());
	m_ImgSizeY = (INT)(m_SocketImg->GetHeight());

	Graphics graphics(a_hdc);	//Bitblt 에서 사용하던 그 g_hBackDC 를 넣으면 됨.

	graphics.DrawImage(m_SocketImg,
		Rect(m_CurPos.x, m_CurPos.y, m_ImgSizeX, m_ImgSizeY),
		0, 0, m_SocketImg->GetWidth(), m_SocketImg->GetHeight(),
		UnitPixel);
	//--------------------말그리기

	//--------------------말 번호 출력
	m_OldFont = (HFONT)SelectObject(a_hdc, m_hFont);
	_stprintf_s(m_TmpStr, _T("%d번말"), m_TrackNumber + 1);
	TextOut(a_hdc, m_CurPos.x, m_CurPos.y - 2, m_TmpStr, (int)_tcslen(m_TmpStr));
	//--------------------말 번호 출력

	//--------------------말 등수 출력
	if (0 <= m_Ranking)	//도착했다는 뜻
	{
		SetTextColor(a_hdc, RGB(90, 33, 100));
		SelectObject(a_hdc, m_hArrvFont);
		_stprintf_s(m_TmpStr, _T("%d등"), m_Ranking + 1);
		TextOut(a_hdc, m_CurPos.x - 30, m_CurPos.y + 25, m_TmpStr, (int)_tcslen(m_TmpStr));
		SetTextColor(a_hdc, RGB(0, 0, 0));
	}
	//--------------------말 등수 출력
}

void CCharacter::CChar_Destroy()
{
	if (m_hFont != NULL)
	{
		DeleteObject(m_hFont);
		m_hFont = NULL;
	}

	if (m_hArrvFont != NULL)
	{
		DeleteObject(m_hArrvFont);
		m_hArrvFont = NULL;
	}

	for (int i = 0; i < m_MotionList.size(); i++)
	{
		if (m_MotionList[i] != NULL)
		{
			delete m_MotionList[i];
			m_MotionList[i] = NULL;
		}
	}
}

void CCharacter::LoadAnimList(int Number)
{
	if (Number == 0)
	{
		m_MotionList[(int)Race_Ready]->LoadTexImg(_T(".\\RscTrunk\\Image\\black_1.png")); //<--- 경주마에서는 우선 이렇게...

		m_MotionList[(int)Idle]->LoadTexImg(_T(".\\RscTrunk\\Image\\black_1.png"));

		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\black_1.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\black_2.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\black_3.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\black_4.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\black_5.png"));
	}
	else if (Number == 1)
	{
		m_MotionList[(int)Race_Ready]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\left_walk\\n030.png")); //<--- 경주마에서는 우선 이렇게...

		m_MotionList[(int)Idle]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\front_Idle\\n001.png"));
		m_MotionList[(int)Idle]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\front_Idle\\n002.png"));

		m_MotionList[(int)Front_Walk]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\front_walk\\n010.png"));
		m_MotionList[(int)Front_Walk]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\front_walk\\n011.png"));
		m_MotionList[(int)Front_Walk]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\front_walk\\n012.png"));
		m_MotionList[(int)Front_Walk]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\front_walk\\n013.png"));

		m_MotionList[(int)Back_Walk]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\back_walk\\n040.png"));
		m_MotionList[(int)Back_Walk]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\back_walk\\n041.png"));
		m_MotionList[(int)Back_Walk]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\back_walk\\n042.png"));
		m_MotionList[(int)Back_Walk]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\back_walk\\n043.png"));

		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\left_walk\\n030.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\left_walk\\n031.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\left_walk\\n032.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\left_walk\\n033.png"));

		m_MotionList[(int)Right_Walk]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\right_walk\\n020.png"));
		m_MotionList[(int)Right_Walk]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\right_walk\\n021.png"));
		m_MotionList[(int)Right_Walk]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\right_walk\\n022.png"));
		m_MotionList[(int)Right_Walk]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\right_walk\\n023.png"));
	}
	else if (Number == 2)
	{
		m_MotionList[(int)Race_Ready]->LoadTexImg(_T(".\\RscTrunk\\Image\\Green_1.png")); //<--- 경주마에서는 우선 이렇게...

		m_MotionList[(int)Idle]->LoadTexImg(_T(".\\RscTrunk\\Image\\Green_1.png"));

		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\Green_1.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\Green_2.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\Green_3.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\Green_4.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\Green_5.png"));
	}
	else if (Number == 3)
	{
		m_MotionList[(int)Race_Ready]->LoadTexImg(_T(".\\RscTrunk\\Image\\Brown_1.png")); //<--- 경주마에서는 우선 이렇게...

		m_MotionList[(int)Idle]->LoadTexImg(_T(".\\RscTrunk\\Image\\Brown_1.png"));

		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\Brown_1.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\Brown_2.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\Brown_3.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\Brown_4.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\Brown_5.png"));
	}
	else if (Number == 4)
	{
		m_MotionList[(int)Race_Ready]->LoadTexImg(_T(".\\RscTrunk\\Image\\h_1.png")); //<--- 경주마에서는 우선 이렇게...

		m_MotionList[(int)Idle]->LoadTexImg(_T(".\\RscTrunk\\Image\\h_1.png"));

		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\h_1.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\h_2.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\h_3.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\h_4.png"));
	}
	else if (Number == 5)
	{
		m_MotionList[(int)Race_Ready]->LoadTexImg(_T(".\\RscTrunk\\Image\\red_1.png")); //<--- 경주마에서는 우선 이렇게...

		m_MotionList[(int)Idle]->LoadTexImg(_T(".\\RscTrunk\\Image\\red_1.png"));

		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\red_1.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\red_2.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\red_3.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\red_4.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\red_5.png"));
	}
	else
	{
		m_MotionList[(int)Race_Ready]->LoadTexImg(_T(".\\RscTrunk\\Image\\White_1.png")); //<--- 경주마에서는 우선 이렇게...

		m_MotionList[(int)Idle]->LoadTexImg(_T(".\\RscTrunk\\Image\\White_1.png"));

		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\White_1.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\White_2.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\White_3.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\White_4.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\White_5.png"));
	}

	ChangeState(Race_Ready);
}

bool CCharacter::ChangeState(AniState newState)
{
	if (m_CurAniState == newState)
		return false;

	if (newState < None || AniLength <= newState)
		return false;

	static int a_NowImgCount = 0;
	a_NowImgCount = m_MotionList[(int)newState]->m_ImgList.size();	
	if (a_NowImgCount <= 0)
		return false;

	m_CurAniState = newState;

	m_CurAniInx = 0;
	m_AniTickCount = 0;
	m_NowImgCount = a_NowImgCount;

	m_SocketImg = m_MotionList[(int)m_CurAniState]->m_ImgList[0];	//첫동작 대입

	if (m_CurAniState == Idle)
	{
		m_EachAniDelay = 0.5f;
	}
	else
	{
		m_EachAniDelay = 0.17f;
	}
	return true;
}

void CCharacter::AniFrameUpDate(double a_DeltaTime)
{
	//--------------------애니메이션 플레임 계산 부분
	if (m_CurAniState == None)
		return;

	if (m_NowImgCount <= 0)	//애니 소켓에 뭔가 꼽혀 있는지 확인해 보는 안전 장치
		return;

	m_AniTickCount = m_AniTickCount + a_DeltaTime;
	if (m_EachAniDelay < m_AniTickCount)		//다음 플레임
	{
		m_CurAniInx++;
		if (m_NowImgCount <= m_CurAniInx)		//마지막 플레임일 때
		{
			m_CurAniInx = 0;
		}

		m_SocketImg = m_MotionList[(int)m_CurAniState]->m_ImgList[m_CurAniInx];

		m_AniTickCount = 0;
	}
	//--------------------애니메이션 플레임 계산 부분
}
