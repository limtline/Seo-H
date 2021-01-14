#include "stdafx.h"
#include "CBossMon.h"
#include "CItem_Mgr.h"
#include "CHero.h"
#include "CBullet_Mgr.h"
#include "GlobalValue.h"

CBossMon::CBossMon()
{
	m_CharicType = CT_Boss;

	m_isActive	  = false;
	IsAppearState = false;
}

CBossMon::~CBossMon()
{
}

void CBossMon::Init_Unit(HWND a_hWnd)
{
	m_hWnd = a_hWnd;

	m_R_brsh = CreateSolidBrush(RGB(255, 0, 0));//brush 추가. pen과 같은 방식으로
}

void CBossMon::Update_Unit(float a_DeltaTime)
{
	static RECT a_RT;
	GetClientRect(m_hWnd, &a_RT);   //Main DC 의 가로 세로 사이즈를 얻어오는 함수 
	static Vector2D a_CacVec;

	//-----------Boss AI
	if (IsAppearState == true) //등연 연출 상태임을 나타내는 플래그변수 On 상태
	{
		//보스를 둘러싼 여백이 있는 박스가 화면 안에 들어오면 등장 종료로 판단해 버린다.
		//Spawn 후 Target 지점으로 이동 
		static double a_CacStep;
		static Vector2D a_CacEndVec;

		// 가야할 방향에 시간 값을 곱해서 조금씩 이동하게 한다
		a_CacStep = a_DeltaTime * m_Speed; //이번에 한걸음 길이 (보폭)
		a_CacEndVec = m_TargetPos - m_CurPos;
		if (a_CacEndVec.Magnitude() <= a_CacStep)  //목표점까지의 거리보다 보폭이 크거나 같으면 도착으로 본다.
		{
			IsAppearState = false;
			//m_CurPos = m_TargetPos;  //<--이게 주인공의 좌표
			m_SkillCount = 0; //초기화
			m_SkillTime = (float)((rand() % 6) + 5);
		}
		else
		{
			m_DirVec = a_CacEndVec;
			m_DirVec.Normalize();
			m_CurPos = m_CurPos + (m_DirVec * a_CacStep);

			a_CacVec = m_TargetPos - m_CurPos;
		}

		if (BossLimitMove(a_RT) == true) //이것도 등장 완료로 본다.
		{
			IsAppearState = false;
			//m_CurPos = m_TargetPos;  //<--이게 주인공의 좌표
			m_SkillCount = 0; //초기화
			m_SkillTime = (float)((rand() % 6) + 5);
		}

	}//if (IsAppearState == true)
	else
	{
		//8초 뒤에 발동 궁극기 타임
		m_SkillTime = m_SkillTime - a_DeltaTime;
		if (m_SkillTime <= 0)
		{
			if (-2.5f < m_SkillTime && m_SkillTime <= -2.0f)
			{
				if (m_SkillCount == 0)
				{
					SkillShoot();
					m_SkillCount = 1;
				}
			}
			else if (-3.0f < m_SkillTime)
			{
				if (m_SkillCount == 1)
				{
					SkillShoot();
					m_SkillCount = 2;
				}
			}
			else if (-3.5f < m_SkillTime)
			{
				if (m_SkillCount == 2)
				{
					SkillShoot();
					m_SkillCount = 3;
				}
			}
			else if (-4.0f < m_SkillTime)
			{
				if (m_SkillCount == 3)
				{
					SkillShoot();
					m_SkillCount = 4;
				}
			}
			else if (-5.0f < m_SkillTime)
			{
				if (m_SkillCount == 4)
				{
					SkillShoot();
					m_SkillCount = 5;
				}
			}
			else if (-6.5f < m_SkillTime)
			{
				m_SkillCount = 0; //초기화
				m_SkillTime = (float)((rand() % 6) + 5);
			}
		}
		else //일반 공격
		{
			//주인공을 향하여 총알발사 
			//----총알 발사 업데이트
			m_BLCycle = m_BLCycle + a_DeltaTime;
			if (m_AttackSpeed < m_BLCycle)
			{
				Vector2D a_CurV;
				a_CurV.x = m_CurPos.x;		//마우스의 좌표
				a_CurV.y = m_CurPos.y - 50.0f;

				g_Bullet_Mgr.SpawnBullet(a_CurV, g_Hero.m_CurPos, CT_Monster, BT_Normal);

				m_BLCycle = 0.0f;
			}
			////----총알 발사 업데이트

		}////일반 공격
	}//등장 연출이 끝난 상황에서...

	//------------------- 애니메이션 플레임 계산 부분 
	if (0.0f < a_CacVec.x)
	{
		ChangeState(Right_Walk);  //몬스터 개별적으로 있어야 한다. 
	}
	else
	{
		ChangeState(Left_Walk);
	}
	CUnit::Update_Unit(a_DeltaTime);	//부모쪽 Update_Unit()함수 호출
	//------------------- 애니메이션 플레임 계산 부분 

	//------- 주인공과의 충돌처리
	static float a_CacSDist;
	static float a_CacMargin = 0.0f;
	static float a_RadiusHap;
	static Vector2D a_CacMovePos;
	static Vector2D a_CacSNormal;
	static float a_CacShiftLen = 0.0f;
	a_CacVec = g_Hero.m_CurPos - m_CurPos;
	a_CacSDist = a_CacVec.Magnitude();
	a_RadiusHap = m_HalfColl + 4 + g_Hero.m_HalfColl + 4; //(내반경 + 적반경) 변수 재활용

	a_CacMovePos = g_Hero.m_CurPos;

	if (a_CacSDist < a_RadiusHap)
	{
		a_CacMargin = a_RadiusHap - a_CacSDist;
		a_CacSNormal = a_CacVec; //밀려야할 방향
		a_CacSNormal.Normalize();

		a_CacShiftLen = a_CacMargin;
		if (a_RadiusHap < a_CacShiftLen) //안전장치 시간간격도 반지름보다 커지면 안된다. 
		{
			a_CacShiftLen = a_RadiusHap;
		}

		a_CacMovePos = a_CacMovePos + (a_CacSNormal * a_CacShiftLen);

	}//if (a_CacSDist < a_RadiusHap)

	g_Hero.m_CurPos = a_CacMovePos;
	//------- 주인공과의 충돌처리
}

void CBossMon::Render_Unit(HDC a_hDC)
{
	if (m_SocketImg == NULL)
		return;

	//---- HP Bar Render
	static HBRUSH h_Old_Brush;
	h_Old_Brush = (HBRUSH)SelectObject(a_hDC, m_R_brsh);
	static float a_CacMXX = 0.0f;
	static float a_CacMYY = (int)(m_HalfHeight * 0.8f);
	static float a_HpBSize = 50;
	static float a_CurHpSize = 0;
	a_CacMXX = a_HpBSize / 2.0f;
	a_CurHpSize = a_HpBSize * ((float)m_CurHP / (float)m_MaxHp);

	a_CacMYY = (int)(m_HalfHeight * 0.97f);

	Rectangle(a_hDC, m_CurPos.x - a_CacMXX, m_CurPos.y - a_CacMYY,
		m_CurPos.x - a_CacMXX + a_CurHpSize,
		m_CurPos.y - (a_CacMYY + 10.0f));
	SelectObject(a_hDC, h_Old_Brush);//기존 브러쉬로 교체
	//---- HP Bar Render

	Graphics graphics(a_hDC);
	graphics.DrawImage(m_SocketImg, m_CurPos.x - m_HalfWidth,
		m_CurPos.y - (int)(m_HalfHeight * 1.2f),
		(float)m_ImgSizeX, (float)m_ImgSizeY);
}

void CBossMon::Destroy_Unit()
{
	if (m_R_brsh != NULL)
	{
		DeleteObject(m_R_brsh);
		m_R_brsh = NULL;
	}
}

void CBossMon::Spawn(float a_XX, float a_YY)
{
	m_CharicType = CT_Boss;

	m_Speed = 300.0f; //보스의 이동 속도
	m_HalfColl = 90;  //보스의 충돌 반경
	m_MaxHp = 550;    //보스의 피통

	m_AttackSpeed = 1.0f - ((g_DiffLevel - 3) * 0.05f);
	if (1.0f < m_AttackSpeed )
		m_AttackSpeed = 1.0f;
	if (m_AttackSpeed < 0.6f)
		m_AttackSpeed = 0.6f;

	m_CurPos.x = a_XX;
	m_CurPos.y = a_YY;

	m_isActive = true;
	IsAppearState = true;

	static RECT a_Crt;
	GetClientRect(m_hWnd, &a_Crt);   //Main DC 의 가로 세로 사이즈를 얻어오는 함수 
	static Vector2D m_CenterPos;
	m_CenterPos.x = (float)(a_Crt.right / 2);
	m_CenterPos.y = (float)(a_Crt.bottom / 2); //화면 중앙 점
	m_TargetPos = m_CenterPos; 

	m_CurHP = m_MaxHp;

	SetAni_Rsc(m_CharicType);

	LoadUnitSize();

	m_ImgSizeX = m_SocketImg->GetWidth() * 0.4f;     //기본 이미지의 가로 사이즈
	m_ImgSizeY = m_SocketImg->GetHeight() * 0.5f;	  //기본 이미지의 세로 사이즈
	m_HalfWidth = m_ImgSizeX / 2;			  //기본 이미지의 가로 반사이즈
	m_HalfHeight = m_ImgSizeY / 2;			  //기본 이미지의 세로 반사이즈
}

bool CBossMon::BossLimitMove(RECT & a_RT)
{
	static float a_CacLXXX = 0.0f;
	static float a_CacLYYY = 0.0f;

	a_CacLXXX = m_CurPos.x - (m_HalfColl + 50.0f);
	if (a_CacLXXX < a_RT.left)
	{
		return false;
	}

	a_CacLYYY = m_CurPos.y - (m_HalfColl + 50.0f);
	if (a_CacLYYY < a_RT.top)
	{
		return false;
	}

	a_CacLXXX = m_CurPos.x + (m_HalfColl + 50.0f);
	if (a_RT.right < a_CacLXXX)
	{
		return false;
	}

	a_CacLYYY = m_CurPos.y + (m_HalfColl + 50.0f);
	if (a_RT.bottom < a_CacLYYY)
	{
		return false;
	}

	return true;
}

void CBossMon::TakeDamage(float a_Damage)
{
	m_CurHP = m_CurHP - (int)a_Damage;

	if (m_CurHP <= 0)   //사망처리
	{
		m_isActive = false;

		//---보상 주기
		for (int ii = 0; ii < 5; ii++)
		{
			Vector2D a_CacPos = m_CurPos;
			a_CacPos.x = m_CurPos.x + (float)((rand() % 60) - 30);
			a_CacPos.y = m_CurPos.y + (float)((rand() % 60) - 30);
			//--------- Item 스폰
			if (ii < 2)
				g_ItemMgr.SpawnItem(a_CacPos, true);
			else
				g_ItemMgr.SpawnItem(a_CacPos);
			//--------- Item 스폰
		}
		//---보상 주기

		m_CurHP = 0;
	}
}

void CBossMon::ReSrcClear()
{
	m_isActive = false;
}

void CBossMon::SkillShoot()
{
	float Radius = 100.0f;
	float Angle = 0.0f;
	static Vector2D a_CacStartV;
	static Vector2D a_TargetV;
	////--12등분  16등분 
	for (Angle = 0; Angle < (2.0f * 3.141592f); Angle += (3.141592f / 12))
	{
		a_CacStartV = m_CurPos;

		a_TargetV.x = a_CacStartV.x + (Radius * cos(Angle));
		a_TargetV.y = a_CacStartV.y + (Radius * sin(Angle));

		g_Bullet_Mgr.SpawnBullet(a_CacStartV, a_TargetV, CT_Monster, BT_Normal);
	}
}
