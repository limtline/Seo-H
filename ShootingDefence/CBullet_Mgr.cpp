#include "stdafx.h"
#include "CBullet_Mgr.h"
#include "CMonster_Mgr.h"
#include "CHero.h"

//----------------------------- CBullet
HBRUSH CBullet::m_R_brsh = NULL;
HBRUSH CBullet::m_G_brsh = NULL;

CBullet::CBullet()
{
	if (m_R_brsh == NULL)
	{
		m_R_brsh = CreateSolidBrush(RGB(255, 0, 0));
	}

	if (m_G_brsh == NULL)
	{
		m_G_brsh = CreateSolidBrush(RGB(70, 70, 70));
	}

	m_BLActive = false;
	m_BL_Type = BT_Normal;   //기본은 일반 총알
	m_UC_Type = CT_Hero;     //기본은 주인 공이 쏜 것으로 판단한다.

	m_LifeTime = 4.0f;
	m_MoveSpeed = 800.0f;   //800픽셀 / 1초

	m_HalfColl = 20.0f;     //충돌반경
}

CBullet::~CBullet()
{
	if (m_R_brsh != NULL)
	{
		DeleteObject(m_R_brsh);
		m_R_brsh = NULL;
	}

	if (m_G_brsh != NULL)
	{
		DeleteObject(m_G_brsh);
		m_G_brsh = NULL;
	}
}

bool CBullet::BulletUpdate(float a_DeltaTime)
{
	m_LifeTime = m_LifeTime - a_DeltaTime;
	if (m_LifeTime < 0.0f)
	{
		m_BLActive = false;
		return false;  //일정 시간뒤에 제거해 준다.
	}

	m_CurPos = m_CurPos + (m_DirVec * (a_DeltaTime * m_MoveSpeed)); //총알 이동처리

	return true;
}

void CBullet::BulletRender(HDC a_hDC)
{
	if (m_BL_Type == BT_Skill1) //스킬 타입의 총알
	{
		if (m_R_brsh != NULL)
		{
			a_R_Old_brsh = SelectObject(a_hDC, m_R_brsh);

			Ellipse(a_hDC, (int)(m_CurPos.x - 8.0f), (int)(m_CurPos.y - 8.0f),
				(int)(m_CurPos.x + 8.0f), (int)(m_CurPos.y + 8.0f));

			SelectObject(a_hDC, a_R_Old_brsh);//기존 브러쉬로 교체
		}
	}
	else  //일반 타입의 총알
	{
		if (m_UC_Type == CT_Monster) //스킬 타입의 총알
		{
			if (m_G_brsh == NULL)
				return;

			a_R_Old_brsh = SelectObject(a_hDC, m_G_brsh);

			Ellipse(a_hDC, (int)(m_CurPos.x - 8.0f), (int)(m_CurPos.y - 8.0f),
				(int)(m_CurPos.x + 8.0f), (int)(m_CurPos.y + 8.0f));

			SelectObject(a_hDC, a_R_Old_brsh);//기존 브러쉬로 교체
		}
		else
		{
			Ellipse(a_hDC, (int)(m_CurPos.x - 8.0f), (int)(m_CurPos.y - 8.0f),
				(int)(m_CurPos.x + 8.0f), (int)(m_CurPos.y + 8.0f));
		}
	}
}
//----------------------------- CBullet

//----------------------------- CBullet_Mgr
CBullet_Mgr::CBullet_Mgr()
{
}

CBullet_Mgr::~CBullet_Mgr()
{
}

void CBullet_Mgr::BLMgr_Init()
{
	//----총알 메모리풀로 미리 만들어 놓기	
	CBullet* a_BLNode = NULL;
	for (int aii = 0; aii < 200; aii++)
	{
		a_BLNode = new CBullet();
		m_BullList.push_back(a_BLNode);
	}
	//----총알 메모리풀로 미리 만들어 놓기

	m_h_brsh = CreateSolidBrush(RGB(0, 100, 255));//brush 추가. pen과 같은 방식으로
	m_h_pen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));//선종류(실선), 두께(1), RGB색
}

void CBullet_Mgr::BLMgr_Update(float a_DeltaTime, HWND a_hWnd, CMonster_Mgr* a_MonMgr)
{
	//------------------ 모든 총알 이동관련 업데이트 처리
	static Vector2D a_CacBVec;
	for (int aii = 0; aii < m_BullList.size(); aii++)
	{
		if (m_BullList[aii]->m_BLActive == false)
			continue;

		m_BullList[aii]->BulletUpdate(a_DeltaTime);
	
		if (m_BullList[aii]->m_UC_Type == CT_Hero) //주인공이 쏜 총알이라면...
		{
			a_MonMgr->TakeDamage_MonMgr(m_BullList[aii]);
		}
		else //몬스터가 쏜 총알이라면...
		{
			//------------------ 주인공 데미지 주기
			a_CacBVec = g_Hero.m_CurPos - m_BullList[aii]->m_CurPos;

			if (0.0f < g_Hero.m_SdOnTime) //쉴드 적용중이라면
			{
				if (a_CacBVec.Magnitude() < g_Hero.m_SdHalfSize)
				{
					m_BullList[aii]->m_BLActive = false;  //총알 제거
					continue;
				}
			}

			if (a_CacBVec.Magnitude() < (g_Hero.m_HalfColl + m_BullList[aii]->m_HalfColl)) //몬스터의 반지름 10 + 총알의 반지름 10 + 10(좀  넓게)
			{
				g_Hero.TakeDamage(10);
				m_BullList[aii]->m_BLActive = false;  //총알 제거
			}
			//------------------ 주인공 데미지 주기
		}////몬스터가 쏜 총알이라면...

	}//for (int aii = 0; aii < m_BullList.size(); aii++)
	//------------------ 모든 총알 이동관련 업데이트 처리
}

void CBullet_Mgr::BLMgr_Render(HDC a_hDC)
{
	if (m_h_pen == NULL || m_h_brsh == NULL)
		return;

	m_h_Oldpen = SelectObject(a_hDC, m_h_pen);//HGDIOBJ 모든 GDI object의 집합														
	m_h_Old_brsh = SelectObject(a_hDC, m_h_brsh);

	for (int aii = 0; aii < m_BullList.size(); aii++)
	{
		if (m_BullList[aii]->m_BLActive == false)
			continue;

		m_BullList[aii]->BulletRender(a_hDC);

	}//for (m_Biter = m_StdList.begin(); m_Biter != m_StdList.end(); m_Biter++)

	SelectObject(a_hDC, m_h_Oldpen);//기존 펜으로 교체
	SelectObject(a_hDC, m_h_Old_brsh);//기존 브러쉬로 교체
}

void CBullet_Mgr::BLMgr_Destroy()
{
	//총알 메모리풀 제거하기... 
	for (int aii = 0; aii < m_BullList.size(); aii++)
	{
		if (m_BullList[aii] != NULL)
		{
			delete m_BullList[aii];
			m_BullList[aii] = NULL;
		}
	}
	m_BullList.clear();
	//총알 메모리풀 제거하기...

	if (m_h_brsh != NULL)
	{
		DeleteObject(m_h_brsh);//brush object 제거
		m_h_brsh = NULL;
	}
	if (m_h_pen != NULL)
	{
		DeleteObject(m_h_pen);//GDI object 생성 갯수는 운영체제에서 정해져 있음
		m_h_pen = NULL;
	}

}
void CBullet_Mgr::SpawnBullet(Vector2D a_StartV, Vector2D a_TargetV,
								UClassType a_Own_Type, BulletType a_BLTP)
{
	CBullet* a_BNode = NULL;

	for (int aii = 0; aii < m_BullList.size(); aii++)
	{
		if (m_BullList[aii]->m_BLActive == false)
		{
			a_BNode = m_BullList[aii];
			break;
		}
	}

	if (a_BNode == NULL)
	{
		a_BNode = new CBullet();
		m_BullList.push_back(a_BNode);
	}

	a_BNode->m_CurPos.x = a_StartV.x;
	a_BNode->m_CurPos.y = a_StartV.y;

	a_BNode->m_DirVec = a_TargetV - a_StartV;
	a_BNode->m_DirVec.Normalize();
	a_BNode->m_CurPos = a_BNode->m_CurPos + a_BNode->m_DirVec * 20.0f;
	a_BNode->m_BLActive = true;
	a_BNode->m_LifeTime = 4.0f;
	a_BNode->m_UC_Type = a_Own_Type;
	a_BNode->m_BL_Type = a_BLTP;
}

void CBullet_Mgr::ReSrcClear()
{
	//----남은 총알 제거 
	for (int aii = 0; aii < m_BullList.size(); aii++)
	{
		m_BullList[aii]->m_BLActive = false;
	}
	//----남은 총알 제거 
}
//----------------------------- CBullet_Mgr

CBullet_Mgr g_Bullet_Mgr;