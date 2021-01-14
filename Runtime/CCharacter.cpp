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
	m_CurAniInx = 0;			//���� Ani Index
	m_AniTickCount = 0;			//�����÷��ӱ��� �ð� Add
	m_EachAniDelay = 0.2f;		//�÷��� ���ݽð�
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
		m_hFont = CreateFont(15, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("����"));
	}

	if (m_hArrvFont == NULL)
	{
		m_hArrvFont = CreateFont(25, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("�ü�"));
	}
		
}

void CCharacter::CChar_ReSetPos(int a_TrackNb, LONG a_XX, LONG a_YY)
{
	m_TrackNumber = a_TrackNb;

	m_CurPos.x = a_XX;
	m_CurPos.y = a_YY;

	m_CacAddPosX = (float)m_CurPos.x;

	m_MoveSpeed = 170.0;		//�̵��ӵ� �ʱ�ȭ
	m_Ranking = -1;				//������ ��� �ʱ�ȭ

	ChangeState(Race_Ready);
}

void CCharacter::CChar_Update(double a_DeltaTime, RECT * a_rt, bool a_isRaceStart, int * a_Ranking)
{
	if (a_rt == NULL)
		return;

	//static�̸� �۷ι�ó�� ��� ĳ���Ͱ� ������ �����ϰ� �ȴ�.
	static int a_PosY;	//�� �κ��� �޸𸮿��� �ѹ��� ������� static���� ��
	static int a_YTab;
	a_PosY = (int)((float)a_rt->bottom * 0.03f);	//�ߺ� ����� �� ���� �������� 
	a_YTab = a_rt->bottom / 7;	//�ߺ� ����� �� ���� �������� �Ź� Update��

	if (m_TrackNumber == 1 || m_TrackNumber == 4)
	{
		m_CurPos.y = a_PosY + (m_TrackNumber * a_YTab) - 12;	//������, ȣ��ī
	}
	else
		m_CurPos.y = a_PosY + (m_TrackNumber * a_YTab);

	//--------- �� �̵� ó��
	if (a_isRaceStart == true)
	{
		m_CacAddPosX = m_CacAddPosX + (a_DeltaTime * m_MoveSpeed);
		m_CurPos.x = (LONG)m_CacAddPosX;

		static float a_EndLine;
		a_EndLine = (LONG)((float)a_rt->right * 0.985f) - 92;
		if (a_EndLine < m_CurPos.x)	//���� üũ
		{
			if (m_Ranking < 0)
			{
				m_Ranking = *a_Ranking; ;			//������ ���
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
	//--------- �� �̵� ó��

	//--------------------�ִϸ��̼� �÷��� ��� �κ�
	AniFrameUpDate(a_DeltaTime);
	//--------------------�ִϸ��̼� �÷��� ��� �κ�
}

void CCharacter::CChar_Render(HDC a_hdc)
{
	if (m_SocketImg == NULL)	//�ִ� ���Ͽ� ���� ���� �ִ��� Ȯ���� ���� ������ġ
		return;

	//--------------------���׸���
	m_ImgSizeX = (INT)(m_SocketImg->GetWidth());
	m_ImgSizeY = (INT)(m_SocketImg->GetHeight());

	Graphics graphics(a_hdc);	//Bitblt ���� ����ϴ� �� g_hBackDC �� ������ ��.

	graphics.DrawImage(m_SocketImg,
		Rect(m_CurPos.x, m_CurPos.y, m_ImgSizeX, m_ImgSizeY),
		0, 0, m_SocketImg->GetWidth(), m_SocketImg->GetHeight(),
		UnitPixel);
	//--------------------���׸���

	//--------------------�� ��ȣ ���
	m_OldFont = (HFONT)SelectObject(a_hdc, m_hFont);
	_stprintf_s(m_TmpStr, _T("%d����"), m_TrackNumber + 1);
	TextOut(a_hdc, m_CurPos.x, m_CurPos.y - 2, m_TmpStr, (int)_tcslen(m_TmpStr));
	//--------------------�� ��ȣ ���

	//--------------------�� ��� ���
	if (0 <= m_Ranking)	//�����ߴٴ� ��
	{
		SetTextColor(a_hdc, RGB(90, 33, 100));
		SelectObject(a_hdc, m_hArrvFont);
		_stprintf_s(m_TmpStr, _T("%d��"), m_Ranking + 1);
		TextOut(a_hdc, m_CurPos.x - 30, m_CurPos.y + 25, m_TmpStr, (int)_tcslen(m_TmpStr));
		SetTextColor(a_hdc, RGB(0, 0, 0));
	}
	//--------------------�� ��� ���
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
		m_MotionList[(int)Race_Ready]->LoadTexImg(_T(".\\RscTrunk\\Image\\black_1.png")); //<--- ���ָ������� �켱 �̷���...

		m_MotionList[(int)Idle]->LoadTexImg(_T(".\\RscTrunk\\Image\\black_1.png"));

		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\black_1.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\black_2.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\black_3.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\black_4.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\black_5.png"));
	}
	else if (Number == 1)
	{
		m_MotionList[(int)Race_Ready]->LoadTexImg(_T(".\\RscTrunk\\Nanami\\left_walk\\n030.png")); //<--- ���ָ������� �켱 �̷���...

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
		m_MotionList[(int)Race_Ready]->LoadTexImg(_T(".\\RscTrunk\\Image\\Green_1.png")); //<--- ���ָ������� �켱 �̷���...

		m_MotionList[(int)Idle]->LoadTexImg(_T(".\\RscTrunk\\Image\\Green_1.png"));

		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\Green_1.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\Green_2.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\Green_3.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\Green_4.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\Green_5.png"));
	}
	else if (Number == 3)
	{
		m_MotionList[(int)Race_Ready]->LoadTexImg(_T(".\\RscTrunk\\Image\\Brown_1.png")); //<--- ���ָ������� �켱 �̷���...

		m_MotionList[(int)Idle]->LoadTexImg(_T(".\\RscTrunk\\Image\\Brown_1.png"));

		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\Brown_1.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\Brown_2.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\Brown_3.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\Brown_4.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\Brown_5.png"));
	}
	else if (Number == 4)
	{
		m_MotionList[(int)Race_Ready]->LoadTexImg(_T(".\\RscTrunk\\Image\\h_1.png")); //<--- ���ָ������� �켱 �̷���...

		m_MotionList[(int)Idle]->LoadTexImg(_T(".\\RscTrunk\\Image\\h_1.png"));

		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\h_1.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\h_2.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\h_3.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\h_4.png"));
	}
	else if (Number == 5)
	{
		m_MotionList[(int)Race_Ready]->LoadTexImg(_T(".\\RscTrunk\\Image\\red_1.png")); //<--- ���ָ������� �켱 �̷���...

		m_MotionList[(int)Idle]->LoadTexImg(_T(".\\RscTrunk\\Image\\red_1.png"));

		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\red_1.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\red_2.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\red_3.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\red_4.png"));
		m_MotionList[(int)Left_Walk]->LoadTexImg(_T(".\\RscTrunk\\Image\\red_5.png"));
	}
	else
	{
		m_MotionList[(int)Race_Ready]->LoadTexImg(_T(".\\RscTrunk\\Image\\White_1.png")); //<--- ���ָ������� �켱 �̷���...

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

	m_SocketImg = m_MotionList[(int)m_CurAniState]->m_ImgList[0];	//ù���� ����

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
	//--------------------�ִϸ��̼� �÷��� ��� �κ�
	if (m_CurAniState == None)
		return;

	if (m_NowImgCount <= 0)	//�ִ� ���Ͽ� ���� ���� �ִ��� Ȯ���� ���� ���� ��ġ
		return;

	m_AniTickCount = m_AniTickCount + a_DeltaTime;
	if (m_EachAniDelay < m_AniTickCount)		//���� �÷���
	{
		m_CurAniInx++;
		if (m_NowImgCount <= m_CurAniInx)		//������ �÷����� ��
		{
			m_CurAniInx = 0;
		}

		m_SocketImg = m_MotionList[(int)m_CurAniState]->m_ImgList[m_CurAniInx];

		m_AniTickCount = 0;
	}
	//--------------------�ִϸ��̼� �÷��� ��� �κ�
}
