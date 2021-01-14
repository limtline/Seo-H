#include "stdafx.h"
#include "CMonster_Mgr.h"
#include "CHero.h"
#include "CBullet_Mgr.h"
#include "CItem_Mgr.h"
#include "GlobalValue.h"

//----------------------------- CMonster
CMonster::CMonster()
{
	m_CharicType = CT_Zombi1; //캐릭터 종류가 "몬스터"인지? "나나미"인지?

	m_isActive = false;

	m_MaxHP = 100.0f;
	m_CurHP = 100.0f;

	m_Speed = 150.0f;
	m_HalfColl = 20;
}

CMonster::~CMonster()
{
}

void CMonster::Init_Unit()
{
}

void CMonster::Update_Unit(float a_DeltaTime, CHero & a_Hero)
{
	Vector2D a_CacVec = a_Hero.m_CurPos - m_CurPos;

	if (0.0f < a_Hero.m_SdOnTime) //쉴드 적용중이라면
	{
		if (a_CacVec.Magnitude() < a_Hero.m_SdHalfSize)
		{
			a_DeltaTime = a_DeltaTime * 0.05f;
		}
	}

	//-----------Monster AI
	if (a_Hero.m_HalfColl + m_HalfColl < a_CacVec.Magnitude()) //(충돌반경의합<중점의거리) 아직 부딪히지 않았다.는 뜻
	{
		//추적
		m_DirVec = a_CacVec;  //몬스터의 방향 벡터
		m_DirVec.Normalize();
		m_CurPos = m_CurPos + (m_DirVec * (a_DeltaTime * m_Speed));
	}
	//-----------Monster AI

	//---------- 몬스터 공격 처리 부분
	static float m_AttackTick = 0.0f;
	m_AttackTick = m_AttackTick - a_DeltaTime;
	if (a_CacVec.Magnitude() < (a_Hero.m_HalfColl + m_HalfColl + 10.0f))
	{
		if (m_AttackTick < 0.0f)  //공격 범위 안에 들어오면 즉시 공격
		{
			//몬스터가 주인공 공격 공격 
			a_Hero.TakeDamage(10);

			m_AttackTick = 1.0f;
		}
	}
	//---------- 몬스터 공격 처리 부분

	//------------------- 애니메이션 플레임 계산 부분 
	if (0.0f < a_CacVec.x)
	{
		ChangeState(Right_Walk);  //몬스터 개별적으로 있어야 한다. 
	}
	else
	{
		ChangeState(Left_Walk);
	}
	CUnit::Update_Unit(a_DeltaTime);
	//------------------- 애니메이션 플레임 계산 부분 

}

void CMonster::Render_Unit(HDC a_hDC, HBRUSH & a_R_brsh)
{
	if (m_SocketImg == NULL)
		return;

	//---- HP Bar Render
	static HBRUSH h_Old_Brush;
	h_Old_Brush = (HBRUSH)SelectObject(a_hDC, a_R_brsh);
	static float a_CacMXX = 0.0f;
	static float a_CacMYY = (int)(m_HalfHeight * 0.8f);
	static float a_HpBSize = 50;
	static float a_CurHpSize = 0;
	a_CacMXX = a_HpBSize / 2.0f;
	a_CurHpSize = a_HpBSize * ((float)m_CurHP / (float)m_MaxHP);

	//a_CacMYY = (int)(m_HalfHeight * 0.83f);
	a_CacMYY = (int)(m_HalfHeight * 0.97f);
	if (m_CharicType == CT_Zombi1)
	{
		a_CacMYY = (int)(m_HalfHeight * 0.83f);
	}
	else if (m_CharicType == CT_Zombi2 || m_CharicType == CT_Zombi4)
	{
		a_CacMYY = (int)(m_HalfHeight * 0.67f);
	}

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

void CMonster::Destroy_Unit()
{
}

void CMonster::Spawn(float a_XX, float a_YY)
{
	//난이도에 따른 HP 변경
	m_MaxHP = 50.0f + ((g_DiffLevel - 3) * 15.0f);
	if (m_MaxHP < 50.0f)
		m_MaxHP = 50.0f;

	if (200.0f < m_MaxHP)
		m_MaxHP = 200.0f;
	//난이도에 따른 HP 변경

	//난이도에 따른 이속 변경
	m_Speed = 150.0f + ((g_DiffLevel - 1) * 30.0f);
	if (280.0f < m_Speed)
		m_Speed = 280.0f;
	//난이도에 따른 이속 변경

	m_CurPos.x = a_XX;
	m_CurPos.y = a_YY;

	m_isActive = true;

	m_CurHP = m_MaxHP;

	SetAni_Rsc(m_CharicType);
	if (m_CurAniState == AS_None)
	{
		m_SocketImg = g_Mon_Mgr.m_ImgList[0];
		LoadUnitSize();
	}
}

void CMonster::TakeDamage(float a_Value)
{
	m_CurHP = m_CurHP - a_Value;

	if (m_CurHP <= 0.0f) //몬스터 사망 처리
	{
		g_Hero.m_KillCount++;

		//--------- Item 스폰
		g_ItemMgr.SpawnItem(m_CurPos);
		//--------- Item 스폰

		m_isActive = false; 	//-------- 사망 몬스터 제거
	}
}
//----------------------------- CMonster

//----------------------------- CMonster_Mgr
CMonster_Mgr::CMonster_Mgr()
{
}

CMonster_Mgr::~CMonster_Mgr()
{
}

void CMonster_Mgr::MonMgr_Init(HWND a_hWnd)
{
	//----몬스터 이미지 로딩하기
	Image* a_ImgNode = NULL;

	//a_ImgNode = Image::FromFile(_T(".\\MonsterImg\\Zombi1.png"));
	//m_ImgList.push_back(a_ImgNode);
	for (int ii = 0; ii < CTT_Length; ii++)
	{
		a_ImgNode = NULL;
		if ((CT_Type)ii == CT_Nanami)
		{
			a_ImgNode = Image::FromFile(_T(".\\Nanami\\front_Idle\\n001.png"));
		}
		else if ((CT_Type)ii == CT_Zombi4)
		{
			a_ImgNode = Image::FromFile(_T(".\\MonsterImg\\Zombi4.png"));
		}
		else if ((CT_Type)ii == CT_Zombi1)
		{
			a_ImgNode = Image::FromFile(_T(".\\MonsterImg\\Zombi1.png"));
		}
		else if ((CT_Type)ii == CT_Zombi2)
		{
			a_ImgNode = Image::FromFile(_T(".\\MonsterImg\\Zombi2.png"));
		}
		else if ((CT_Type)ii == CT_Zombi6)
		{
			a_ImgNode = Image::FromFile(_T(".\\MonsterImg\\Zombi6.png"));
		}
		else if ((CT_Type)ii == CT_Zombi3)
		{
			a_ImgNode = Image::FromFile(_T(".\\MonsterImg\\Zombi3.png"));
		}
		else if ((CT_Type)ii == CT_Boss)
		{
			a_ImgNode = Image::FromFile(_T(".\\MonsterImg\\Front\\Megapack III Void Gargoyle.png"));
		}
		else //if ((CT_Type)ii == CT_Zombi3)
		{
			a_ImgNode = Image::FromFile(_T(".\\MonsterImg\\Zombi8.png"));
		}
		m_ImgList.push_back(a_ImgNode);
	}
	//----몬스터 이미지 로딩하기

	//----몬스터 메모리풀로 미리 만들어 놓기
	CMonster* a_Node = NULL;
	for (int aii = 0; aii < 20; aii++)
	{
		a_Node = new CMonster();
		a_Node->Init_Unit();
		m_MonList.push_back(a_Node);
	}
	//----몬스터 메모리풀로 미리 만들어 놓기

	//----보스 몬스터  초기화
	CBossMon* a_BossNode = NULL;
	for (int ii = 0; ii < 3; ii++)
	{
		a_BossNode = new CBossMon();
		a_BossNode->Init_Unit(a_hWnd);
		m_BossList.push_back(a_BossNode);  //<--- 보스 몬스터 리스트
	}
	//----보스 몬스터  초기화

	m_R_brsh = CreateSolidBrush(RGB(255, 0, 0));//brush 추가. pen과 같은 방식으로
}

void CMonster_Mgr::MonMgr_Update(float a_DeltaTime, HWND a_hWnd, CHero& a_Hero)
{
	//---------- 주기적인 Monster Spawn   
	SpawnMonster(a_DeltaTime, a_hWnd);
	//---------- 주기적인 Monster Spawn  

	//-------- 보스몹 업데이트
	for (int ii = 0; ii < m_BossList.size(); ii++)
	{
		if (m_BossList[ii]->m_isActive == false)
			continue;

		m_BossList[ii]->Update_Unit(a_DeltaTime);
	}
	//-------- 보스몹 업데이트

	//-----------Monster AI
	//몬스터 끼리 겹치지 않게 하기 관련 변수...
	static Vector2D a_CacVec;
	static float a_CacSDist;
	static float a_CacMargin = 0.0f;
	static float a_RadiusHap;
	static Vector2D a_CacMovePos;
	static Vector2D a_CacSNormal;
	static float a_CacShiftLen = 0.0f;
	//몬스터 끼리 겹치지 않게 하기 관련 변수...

	for (int ii = 0; ii < m_MonList.size(); ii++)
	{
		if (m_MonList[ii]->m_isActive == false)
			continue;

		m_MonList[ii]->Update_Unit(a_DeltaTime, a_Hero);

		//몬스터 끼리 겹치지 않게 하기...
		for (int xx = 0; xx < m_MonList.size(); xx++)
		{
			if (m_MonList[ii] == m_MonList[xx])
				continue;
			if (m_MonList[xx]->m_isActive == false)
				continue;
			a_CacVec = m_MonList[ii]->m_CurPos - m_MonList[xx]->m_CurPos;
			a_CacSDist = a_CacVec.Magnitude();
			a_RadiusHap = m_MonList[ii]->m_HalfColl + 4 + m_MonList[xx]->m_HalfColl + 4; //(내반경 + 적반경) 변수 재활용
			a_CacMovePos = m_MonList[ii]->m_CurPos;
			if (a_CacSDist < a_RadiusHap) //겹친 상태
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
			m_MonList[ii]->m_CurPos = a_CacMovePos;
		}
		//몬스터 끼리 겹치지 않게 하기...

		//------- 주인공과의 충돌처리
		a_CacVec = m_MonList[ii]->m_CurPos - a_Hero.m_CurPos;
		a_CacSDist = a_CacVec.Magnitude();
		a_RadiusHap = m_MonList[ii]->m_HalfColl + 4 + a_Hero.m_HalfColl + 4; //(내반경 + 적반경) 변수 재활용
		a_CacMovePos = m_MonList[ii]->m_CurPos;
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
		m_MonList[ii]->m_CurPos = a_CacMovePos;
		//------- 주인공과의 충돌처리
	
	}//for (int ii = 0; ii < m_MonList.size(); ii++)
	//-----------Monster AI
}

void CMonster_Mgr::MonMgr_Render(HDC a_hDC)
{
	//--- 몬스터 이미지 그리기...
	for (int ii = 0; ii < m_MonList.size(); ii++)
	{
		if (m_MonList[ii]->m_isActive == false)
			continue;

		m_MonList[ii]->Render_Unit(a_hDC, m_R_brsh);
	}
	//--- 몬스터 이미지 그리기...

	//-------- 보스몹 그리기...
	for (int ii = 0; ii < m_BossList.size(); ii++)
	{
		if (m_BossList[ii]->m_isActive == false)
			continue;

		m_BossList[ii]->Render_Unit(a_hDC);
	}
	//-------- 보스몹 그리기...
}

void CMonster_Mgr::MonMgr_Destroy()
{
	//-------- 보스몹 제거하기... 
	for (int ii = 0; ii < m_BossList.size(); ii++)
	{
		if (m_BossList[ii] != NULL)
		{
			m_BossList[ii]->Destroy_Unit();

			delete m_BossList[ii];
			m_BossList[ii] = NULL;
		}
	}
	m_BossList.clear();
	//-------- 보스몹 제거하기... 

	//-------------몬스터 이미지 제거하기
	for (int aii = 0; aii < m_ImgList.size(); aii++)
	{
		if (m_ImgList[aii] != NULL)
		{
			delete m_ImgList[aii];
			m_ImgList[aii] = NULL;
		}
	}
	m_ImgList.clear();
	//-------------몬스터 이미지 제거하기

	//몬스터 메모리풀 제거하기... 
	for (int aii = 0; aii < m_MonList.size(); aii++)
	{
		if (m_MonList[aii] != NULL)
		{
			m_MonList[aii]->Destroy_Unit();

			delete m_MonList[aii];
			m_MonList[aii] = NULL;
		}
	}
	m_MonList.clear();
	//몬스터 메모리풀 제거하기... 

	if (m_R_brsh != NULL)
	{
		DeleteObject(m_R_brsh);//brush object 제거
		m_R_brsh = NULL;
	}
}

void CMonster_Mgr::SpawnMonster(float a_DeltaTime, HWND a_hWnd)
{
	//--- 몬스터 Active 감시 및 마리수 카운트 
	static int g_MonCount = 0;
	g_MonCount = 0;
	for (int ii = 0; ii < m_MonList.size(); ii++)
	{
		if (m_MonList[ii]->m_isActive == true)
		{
			g_MonCount++;
		}
	}
	//--- 몬스터 Active 감시 및 마리수 카운트 

	//---------- 주기적인 Monster Spawn         //몬스터 랜덤 생성
	static float a_SpawnTime = 0.0f;
	a_SpawnTime = a_SpawnTime - a_DeltaTime;
	if (a_SpawnTime < 0.0f)
	{
		m_SpawnLim = 6 + (g_DiffLevel - 2);
		if (m_SpawnLim < 6)
			m_SpawnLim = 6;

		if (10 < m_SpawnLim)
			m_SpawnLim = 10;

		for (int ii = 0; ii < m_MonList.size(); ii++)
		{
			if (m_SpawnLim <= g_MonCount)
				break;

			if (m_MonList[ii]->m_isActive == false)
			{
				////----------------------Random 하게 스폰 좌표 생성하는 부분 
				////세로 - [0] x축 -150.0f ~ -50.0f  Y축 -150.0f ~ (float)m_cRT.bottom + 150.0f
				////       [1] x축 -50.0f ~ (int)m_cRT.right + 50.0f  Y축 -150.0f ~ -50.0f
				////세로 - [2] x축 (int)m_cRT.right + 50.0f ~ (int)m_cRT.right + 150.0f  Y축 -150.0f ~ (float)m_cRT.bottom + 150.0f
				////       [3] x축 -50.0f ~ (int)m_cRT.right + 50.0f  Y축 (float)m_cRT.bottom + 50.0f ~ (float)m_cRT.bottom + 150.0f
				////양옆 //위 아래 4곳중 한곳 랜덤 그중에서 랜덤으로 걸리게 하는 방법도 있다.

				static bool a_TicToc = true;
				//한번은 x축으로 
				//한번은 y축으로

				GetClientRect(a_hWnd, &m_cRT); // 클라이언트 화면의 크기를 받는다

				float a_XX = 0.0f;
				float a_YY = 0.0f;
				if (a_TicToc == true)
				{
					a_XX = (float)((rand() % 200) - 100);
					float a_PushVal = 50.0f;
					int a_YLenVal = (int)m_cRT.bottom + 100;

					if (a_XX < 0.0f)
					{
						a_XX = a_XX - a_PushVal;
					}
					else
					{
						a_XX = a_XX + (float)m_cRT.right + a_PushVal;
					}

					a_YY = (float)((rand() % a_YLenVal) + (100 / 2));

					a_TicToc = false;
				}
				else
				{
					a_YY = (float)((rand() % 200) - 100);
					float a_PushVal = 50.0f;
					int a_XLenVal = (int)m_cRT.right + 100;

					if (a_YY < 0.0f)
					{
						a_YY = a_YY - a_PushVal;
					}
					else
					{
						a_YY = a_YY + (float)m_cRT.bottom + a_PushVal;
					}

					a_XX = (float)((rand() % a_XLenVal) - (100 / 2));

					a_TicToc = true;
				}
				////----------------------Random 하게 스폰 좌표 생성하는 부분 

				m_MonList[ii]->m_CharicType = GetSpMonType();
				m_MonList[ii]->Spawn(a_XX, a_YY);
				//if (CT_None < m_MonList[ii]->m_CharicType && m_MonList[ii]->m_CharicType < CTT_Length)
				//{
				//	m_MonList[ii]->m_SocketImg = g_Mon_Mgr.m_ImgList[m_MonList[ii]->m_CharicType];
				//	m_MonList[ii]->LoadUnitSize();
				//}

				break;

			}//if (m_MonList[ii]->m_isActive == false)
		}//for (int ii = 0; ii < m_MonList.size(); ii++)

		a_SpawnTime = GetSpTimeLevel();
	}
	//---------- 주기적인 Monster Spawn  

	//---------- 주기적으로 Boss Mon Spawn
	//--- 보스 몬스터 Active 감시 및 마리수 카운트 
	static int g_BossCount = 0;
	static float g_Side = 1.0f;

	g_MonCount = 0;
	for (int ii = 0; ii < m_BossList.size(); ii++)
	{
		if (m_BossList[ii]->m_isActive == true)
		{
			g_MonCount++;
		}
	}
	//g_MonCount <--- 지금 활동중인 보스의 마리수
	//g_BossCount <--- 이전 플레임에서 활동중이었던 보스의 마리수
	if (0 < g_BossCount && g_MonCount <= 0) //활동하던 보스들이 모두 사망한 직후부터 5초뒤에 스폰되도록...
	{
		g_Side = 1.0f;
		m_BossSpawnTm = 5.0f;
	}
	g_BossCount = g_MonCount;
	//--- 보스 몬스터 Active 감시 및 마리수 카운트 

	GetClientRect(a_hWnd, &m_cRT);

	if (0.0f < m_BossSpawnTm)
	{
		m_BossSpawnTm = m_BossSpawnTm - a_DeltaTime;
		if (m_BossSpawnTm <= 0.0f)
		{
			for (int ii = 0; ii < m_BossList.size(); ii++)
			{
				//보스를 2마리 이상 스폰시키고 싶으면....
				//if (1 <= g_BossCount)
				//	break;
				//if (g_BossCount == 0) //이제 한마리 추가 시키는 상황이라면...
				//{
				//	m_BossSpawnTm = 0.1f; //0.1초 뒤에 또한마리 추가 시킨다.
				//}

				if (m_BossList[ii]->m_isActive == false)
				{
					float a_XX = 0.0f;
					float a_YY = 0.0f;

					//----------------------------------- 화면 밖 위치 랜덤 발생
					a_XX = (float)((rand() % 200) - 100);  //-100 ~ 99
					float a_PushVal = 50.0f;
					int a_YLenVal = (int)m_cRT.bottom + 100;

					if (g_BossCount == 1) //두마리째 추가 시키는 상황이라면... (반대편에서 나오도록...)
					{
						if ( (a_XX < 0.0f && g_Side < 0.0f) || (0.0f < a_XX  && 0.0f < g_Side) )
						{
							a_XX = a_XX * -1.0f;
						}
					}

					if (a_XX < 0.0f)
					{
						a_XX = a_XX - a_PushVal;

						if (g_BossCount == 0) //이제 한마리 추가 시키는 상황이라면...
							g_Side = -1.0f;
					}
					else
					{
						a_XX = a_XX + (float)m_cRT.right + a_PushVal;

						if (g_BossCount == 0) //이제 한마리 추가 시키는 상황이라면...
							g_Side = 1.0f;
					}

					a_YY = (float)((rand() % a_YLenVal) + (100 / 2));
					//----------------------------------- 화면 밖 위치 랜덤 발생

					m_BossList[ii]->Spawn(a_XX, a_YY);
					//if (CT_None < m_BossList[ii]->m_CharicType && m_BossList[ii]->m_CharicType < CTT_Length)
					//{
					//	m_BossList[ii]->m_SocketImg = g_Mon_Mgr.m_ImgList[m_BossList[ii]->m_CharicType];
					//	m_BossList[ii]->LoadUnitSize();
					//}

					break;
				}//if (m_BossList[ii]->m_isActive == false)
			}//for (int ii = 0; ii < m_BossList.size(); ii++)

		}//if (m_BossSpawnTm <= 0.0f)
	}//if (0.0f < m_BossSpawnTm)
	//---------- 주기적으로 Boss Mon Spawn

}

void CMonster_Mgr::TakeDamage_MonMgr(CBullet* a_RefBullet)
{
	if (a_RefBullet == NULL)
		return;

	float a_Damage = 10.0f;
	if (a_RefBullet->m_BL_Type == BT_Skill1)
	{
		a_Damage = a_Damage * 20.0f;
	}
	else
	{
		a_Damage = 10.0f;
	}

	static Vector2D a_CurV;
	static Vector2D a_CacBVec;

	//--총알이 몬스터에 맞았으면 제거해 준다. 몬스터 제거 총알 제거
	for (int ii = 0; ii < m_MonList.size(); ii++)
	{
		if (m_MonList[ii]->m_isActive == false)
			continue;

		a_CacBVec = m_MonList[ii]->m_CurPos - a_RefBullet->m_CurPos;
		if (a_CacBVec.Magnitude() <
			(m_MonList[ii]->m_HalfColl + a_RefBullet->m_HalfColl))   //몬스터의 반지름 10 + 총알의 반지름 10 + 10(좀  넓게)
		{
			m_MonList[ii]->TakeDamage(a_Damage);

			a_RefBullet->m_BLActive = false;  //총알 제거
			break;
		}
	}
	//총알이 몬스터에 맞았으면 제거해 준다. 몬스터 제거 총알 제거

	if (a_RefBullet->m_BLActive == false)
		return;

	for (int ii = 0; ii < m_BossList.size(); ii++)
	{
		if (m_BossList[ii]->m_isActive == false)
			continue;

		//------------------ 보스데미지 주기
		a_CurV.x = m_BossList[ii]->m_CurPos.x;		//마우스의 좌표
		a_CurV.y = m_BossList[ii]->m_CurPos.y - 30.0f; // - 30.0f; 은 충돌구에 OffSet값 주기
		a_CacBVec = a_CurV - a_RefBullet->m_CurPos;
		if (a_CacBVec.Magnitude() < (m_BossList[ii]->m_HalfColl + a_RefBullet->m_HalfColl))   //몬스터의 반지름 10 + 총알의 반지름 10 + 10(좀  넓게)
		{
			a_Damage = 10.0f; //보스는 무조건 스킬도 일반 데이지가 들어가게 한다.
			m_BossList[ii]->TakeDamage(a_Damage);
			a_RefBullet->m_BLActive = false;  //총알 제거
			break;
		}
		//------------------ 보스데미지 주기
	}//for (int ii = 0; ii < m_BossList.size(); ii++)
}

void CMonster_Mgr::ReSrcClear()
{
	for (int ii = 0; ii < m_MonList.size(); ii++)
	{
		m_MonList[ii]->m_isActive = false;
	}

	//----남은 보스 제거 
	for (int ii = 0; ii < m_BossList.size(); ii++)
	{
		m_BossList[ii]->ReSrcClear();
	}
	m_BossSpawnTm = 3.0f;
	//----남은 보스 제거 
}

CT_Type CMonster_Mgr::GetSpMonType()
{
	CT_Type a_SpType = CT_Zombi1;

	if (g_DiffLevel <= 1)
	{
		a_SpType = CT_Zombi4;
	}
	else if (g_DiffLevel == 2)
	{
		a_SpType = CT_Zombi1;
	}
	else if (g_DiffLevel == 3)
	{
		a_SpType = CT_Zombi2;
	}
	else if (g_DiffLevel == 4)
	{
		a_SpType = CT_Zombi6;
	}
	else if (g_DiffLevel == 5 || g_DiffLevel == 6)
	{
		a_SpType = CT_Zombi3;
	}
	else
	{
		a_SpType = CT_Zombi8;
	}

	return a_SpType;
}

float CMonster_Mgr::GetSpTimeLevel()
{
	float a_SpTime = 2.0f;

	if (g_DiffLevel <= 1)
	{
		a_SpTime = (float)((rand() % 3) + 2);  //다음 스폰 시간 랜덤
	}
	else if (g_DiffLevel == 2)
	{
		a_SpTime = (float)((rand() % 3) + 1);  //다음 스폰 시간 랜덤
	}
	else if (g_DiffLevel == 3)
	{
		a_SpTime = (float)((rand() % 2) + 1);  //다음 스폰 시간 랜덤
	}
	else if (g_DiffLevel == 4)
	{
		a_SpTime = 1.0f;  //다음 스폰 시간 랜덤
	}
	else if (g_DiffLevel == 5)
	{
		a_SpTime = 0.5f;  //다음 스폰 시간 랜덤
	}
	else
	{
		a_SpTime = 0.3f;  //다음 스폰 시간 랜덤
	}

	return a_SpTime;
}

CMonster_Mgr g_Mon_Mgr;

//----------------------------- CMonster_Mgr
