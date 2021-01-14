#include "stdafx.h"
#include "CHero.h"
#include "CBullet_Mgr.h"
#include "GlobalValue.h"

CHero::CHero()
{
	m_SocketImg = NULL;

	m_CurHP = 150;
	m_MaxHp = 150;
}

CHero::~CHero()
{
}

void CHero::Init_Unit(HWND a_hWnd)
{
	m_hWnd = a_hWnd;

	SetAni_Rsc(CT_Nanami);
	if (m_CurAniState == AS_None)
	{
		m_SocketImg = Image::FromFile(_T(".\\Nanami\\front_Idle\\n001.png"));
	}

	LoadUnitSize();

	m_R_brsh = CreateSolidBrush(RGB(255, 0, 0));//brush 추가. pen과 같은 방식으로

	//-------주인공 좌표 초기화 부분
	RECT a_Crt;
	GetClientRect(a_hWnd, &a_Crt);   //Main DC 의 가로 세로 사이즈를 얻어오는 함수 
	m_CurPos.x = (float)(a_Crt.right / 2);
	m_CurPos.y = (float)(a_Crt.bottom / 2);
	//-------주인공 좌표 초기화 부분

	Load_Shield();
}

void CHero::Update_Unit(float a_DeltaTime, RECT & a_RT)
{
	m_DeltaTime = a_DeltaTime;

	if (GetFocus() == NULL) //현재 윈도우가 활성화되어 있지 않으면 주인공 컨트롤 제외
		return;

	//----------------- 동시키 처리 방법
	static Vector2D a_KDirVec;

	a_KDirVec.x = 0.0f;
	a_KDirVec.y = 0.0f;

	if ((GetAsyncKeyState(VK_LEFT) & 0x8000) ||
		(GetAsyncKeyState('A') & 0x8000))
	{
		a_KDirVec.x -= 1.0f;
	}
	if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) ||
		(GetAsyncKeyState('D') & 0x8000))
	{
		a_KDirVec.x += 1.0f;
	}
	if ((GetAsyncKeyState(VK_UP) & 0x8000) ||
		(GetAsyncKeyState('W') & 0x8000))
	{
		a_KDirVec.y -= 1.0f;
	}
	if ((GetAsyncKeyState(VK_DOWN) & 0x8000) ||
		(GetAsyncKeyState('S') & 0x8000))
	{
		a_KDirVec.y += 1.0f;
	}
	//----------------- 동시키 처리 방법 

	if (a_KDirVec.x == 0 && a_KDirVec.y == 0)
	{
		//나중에 숨쉬기 애니 상태 전환
		ChangeState(Idle);
	}
	else
	{
		static Vector2D  m_ZeroVec;
		m_ZeroVec.x = 0.0f;
		m_ZeroVec.y = 0.0f;
		float a_Angle = GetAngle(m_ZeroVec, a_KDirVec);
		if (50.0f < a_Angle && a_Angle < 130.0f)
		{
			ChangeState(Left_Walk);
		}
		else if (130.0f <= a_Angle && a_Angle <= 230.0f)
		{
			ChangeState(Back_Walk);
		}
		else if (230.0f < a_Angle && a_Angle < 310.0f)
		{
			ChangeState(Right_Walk);
		}
		else
		{
			ChangeState(Front_Walk);
		}

		a_KDirVec.Normalize(); //길이가 1인 벡터
		m_CurPos = m_CurPos + (a_KDirVec * (a_DeltaTime * m_Speed));
	}

	HeroLimitMove(a_RT);

	//----총알 발사 업데이트
	m_BLCycle = m_BLCycle + a_DeltaTime;
	if (m_AttackSpeed < m_BLCycle)
		m_BLCycle = 100.0f;

	if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000))
	{
		if (m_AttackSpeed < m_BLCycle)
		{
			POINT a_NewMMXZ;
			GetCursorPos(&a_NewMMXZ);              //현재 마우스 좌표를 얻어오는 함수 
			ScreenToClient(m_hWnd, &a_NewMMXZ);    // 클라이언트 화면 좌표로 변경해 주는 함수
			Vector2D a_TargetV;
			a_TargetV.x = (float)a_NewMMXZ.x;		//마우스의 좌표
			a_TargetV.y = (float)a_NewMMXZ.y;

			g_Bullet_Mgr.SpawnBullet(m_CurPos, a_TargetV);

			m_BLCycle = 0.0f;
		}
	}
	////----총알 발사 업데이트

	//------------------궁극기
	static bool isSPDown = true;
	if (g_GameState == GAME_READY)
	{
		m_STDelay = 0.5f;
	}
	else
	{
		m_STDelay -= a_DeltaTime;
		if (0.0f < m_STDelay)
		{
			isSPDown = false;
		}
		else
		{
			m_STDelay = 0.0f;
		}
	}//else

	if ((GetAsyncKeyState(VK_SPACE) & 0x8000))
	{
		if (isSPDown == true)
		{
			if (0 < m_SkillCount)
			{

				CBullet* a_BNode = NULL;
				float Radius = 100.0f;
				float Angle = 0.0f;
				static Vector2D a_CacStartV;
				static Vector2D a_TargetV;
				////--12등분  16등분 
				for (Angle = 0; Angle < (2.0f * 3.141592f);
								Angle += (3.141592f / 12))
				{
					a_CacStartV = m_CurPos;

					a_TargetV.x = a_CacStartV.x + (Radius * cos(Angle));
					a_TargetV.y = a_CacStartV.y + (Radius * sin(Angle));

					g_Bullet_Mgr.SpawnBullet(a_CacStartV, a_TargetV,
												CT_Hero, BT_Skill1);
				}
	
				m_SkillCount--;
				if (m_SkillCount < 0)
				{
					m_SkillCount = 0;
				}

			}//if (0 < m_SkillCount)

			isSPDown = false;
		}// if (isSPDown == true)
	}// if ((GetAsyncKeyState(VK_SPACE) & 0x8000))
	else
	{
		isSPDown = true;
	}
	//------------------궁극기

	ShieldUpdate();

	//------------------- 애니메이션 플레임 계산 부분 
	CUnit::Update_Unit(a_DeltaTime);	//부모쪽 Update_Unit()함수 호출
	//------------------- 애니메이션 플레임 계산 부분 
}

void CHero::Render_Unit(HDC a_hDC)
{
	//---- HP Bar Render
	static HBRUSH h_Old_Brush;
	h_Old_Brush = (HBRUSH)SelectObject(a_hDC, m_R_brsh);
	static float a_CacMXX = 0.0f;
	static float a_CacMYY = (int)(m_HalfHeight * 0.8f);
	static float a_HpBSize = 50;
	static float a_CurHpSize = 0;
	a_CacMXX = a_HpBSize / 2.0f;
	a_CurHpSize = a_HpBSize * ((float)m_CurHP / (float)m_MaxHp);

	a_CacMYY = m_HalfHeight; //(int)(m_HalfHeight * 1.1f);

	Rectangle(a_hDC, m_CurPos.x - a_CacMXX, m_CurPos.y - a_CacMYY,
		m_CurPos.x - a_CacMXX + a_CurHpSize,
		m_CurPos.y - (a_CacMYY + 10.0f));
	SelectObject(a_hDC, h_Old_Brush);//기존 브러쉬로 교체
	//---- HP Bar Render

	Graphics graphics(a_hDC);
	graphics.DrawImage(m_SocketImg,
		m_CurPos.x - m_HalfWidth,
		m_CurPos.y - (int)(m_HalfHeight * 1.2f),
		(float)m_ImgSizeX, (float)m_ImgSizeY);

	ShieldRender(graphics);
}

void CHero::Destroy_Unit()
{
	if (m_R_brsh != NULL)
	{
		DeleteObject(m_R_brsh);//brush object 제거
		m_R_brsh = NULL;
	}

	ShieldDestroy();
}

void CHero::HeroLimitMove(RECT & a_RT)
{
	static float a_CacLXXX = 0.0f;
	static float a_CacLYYY = 0.0f;

	a_CacLXXX = 15;
	if (m_CurPos.x < a_CacLXXX)
	{
		m_CurPos.x = a_CacLXXX;
	}

	if ((float)(a_RT.right - a_CacLXXX) < m_CurPos.x)
	{
		m_CurPos.x = (float)(a_RT.right - a_CacLXXX);
	}

	a_CacLYYY = 30;
	if (m_CurPos.y < a_CacLYYY)
	{
		m_CurPos.y = a_CacLYYY;
	}

	if ((float)(a_RT.bottom - a_CacLYYY) < m_CurPos.y)
	{
		m_CurPos.y = (float)(a_RT.bottom - a_CacLYYY);
	}
}

void CHero::TakeDamage(float a_Damage)
{
	//몬스터가 주인공 공격 공격 
	m_CurHP = m_CurHP - (int)a_Damage;

	if (m_CurHP <= 0)   //사망처리
	{
		g_GameState = GAME_OVER;

		m_CurHP = 0;
	}
}

void CHero::Load_Shield()
{
	if (m_Shield != NULL)
		return;

	m_Shield = Image::FromFile(_T(".\\RscTrunk\\Shield1.png"));
}

void CHero::ShieldUpdate()
{
	//------------------쉴드 스킬 발동
	static bool isSkDown = true;
	if ((GetAsyncKeyState(VK_RBUTTON) & 0x8000))
	{
		if (isSkDown == true)
		{
			//쉴드 발동
			if (0 < m_ShieldCount)
			{
				if (m_SdOnTime <= 0.0f)
				{
					m_SdOnTime = m_SdDuration;
					m_ShieldCount--;
					if (m_ShieldCount < 0)
						m_ShieldCount = 0;
				}
			}////쉴드 발동

			isSkDown = false;
		}
	}
	else
	{
		isSkDown = true;
	}
	//------------------쉴드 스킬 발동
}

void CHero::ShieldRender(Graphics & graphics)
{
	//---- Shield 그리기 
	if (m_Shield == NULL)
		return;

	if (0.0f < m_SdOnTime)
	{
		m_SdOnTime = m_SdOnTime - m_DeltaTime;
		static float a_EffTime = 0.0f;
		static float a_CacSize = 0.0f;
		static Vector2D a_SdCen;
		if ((m_SdDuration - 0.2f) <= m_SdOnTime)
		{
			a_EffTime = (m_SdDuration - m_SdOnTime) / 0.2f;  //(m_SdDuration - m_SdOnTime) 변화값은 0.0f ~ 0.2f
		}
		else if (m_SdOnTime <= 0.2f)
		{
			a_EffTime = m_SdOnTime / 0.2f;		//(m_SdDuration - m_SdOnTime) 변화값은 0.2f ~ 0.0f
		}
		else
		{
			a_EffTime = 1.0f;
		}
		a_CacSize = 500.0f * a_EffTime;
		m_SdHalfSize = a_CacSize / 2.0f;

		graphics.DrawImage(m_Shield,
			Rect(m_CurPos.x - m_SdHalfSize + 1.0f, m_CurPos.y - m_SdHalfSize,
				a_CacSize, a_CacSize),
			0, 0, m_Shield->GetWidth(), m_Shield->GetHeight(), UnitPixel);
	}
	//---- Shield 그리기 

}

void CHero::ShieldDestroy()
{
	if (m_Shield != NULL)
	{
		delete m_Shield;
		m_Shield = NULL;
	}
}

void CHero::BuyHP()
{
	if (g_Hero.m_MyMoney < 100)
		return;

	if (g_Hero.m_CurHP < g_Hero.m_MaxHp)
	{
		g_Hero.m_CurHP = g_Hero.m_CurHP + 30.0f;
		if (g_Hero.m_MaxHp < g_Hero.m_CurHP)
		{
			g_Hero.m_CurHP = g_Hero.m_MaxHp;
		}
		g_Hero.m_MyMoney = g_Hero.m_MyMoney - 100;
	}//if (g_Hero.m_CurHP < g_Hero.m_MaxHp)
}

void CHero::BuySkill()
{
	if (150 <= g_Hero.m_MyMoney)
	{
		g_Hero.m_SkillCount = g_Hero.m_SkillCount + 1;
		g_Hero.m_MyMoney = g_Hero.m_MyMoney - 150;
	}//if (100 <= g_Hero.m_MyMoney)
}

void CHero::BuyShield()
{
	if (200 <= g_Hero.m_MyMoney)
	{
		g_Hero.m_ShieldCount = g_Hero.m_ShieldCount + 1;
		g_Hero.m_MyMoney = g_Hero.m_MyMoney - 200;
	}//if (100 <= g_Hero.m_MyMoney)
}

CHero g_Hero;