#include "framework.h"
#include "CMonster_Mgr.h"
#include "CMonster.h"
#include "SpawnPos.h"
#include "CHero.h"

CMonster_Mgr::CMonster_Mgr()
{
}

CMonster_Mgr::~CMonster_Mgr()
{
}

void CMonster_Mgr::MonMgr_Init(HWND a_hWnd, ID2D1Bitmap* (*D2DLoadBmp)(LPCWSTR FName))
{
	//----몬스터 이미지 로딩하기
	ID2D1Bitmap* a_ImgNode = NULL;

	if (D2DLoadBmp != NULL)
		a_ImgNode = D2DLoadBmp(_T(".\\MonsterImg\\Zombi1.png"));

	m_ImgList.push_back(a_ImgNode);
	//----몬스터 이미지 로딩하기

	//----몬스터 메모리풀로 미리 만들어 놓기
	CMonster* a_Node = NULL;
	for (int aii = 0; aii < 20; aii++)
	{
		a_Node = new CMonster();
		a_Node->Init_Unit();
		m_InActMonList.push_back(a_Node);
	}
	//----몬스터 메모리풀로 미리 만들어 놓기

}

void CMonster_Mgr::MonMgr_Update(double a_DeltaTime, VecINT2D& a_Center, Vector2D& a_CamPos, CHero& a_Hero)
{
	m_Hero = &a_Hero;

	m_ScCollRadius = sqrtf(a_Center.x * a_Center.x + a_Center.y * a_Center.y);
	m_ScCollRadius = m_ScCollRadius + 50.0f;

	m_ScreenHalf = a_Center; //Main DC 의 가로 세로 반사이즈
	m_CamPos = a_CamPos;

	//---------- 주기적인 Monster Spawn   
	SpawnMonster(a_DeltaTime);
	//---------- 주기적인 Monster Spawn  

	//-----------Monster AI
	//몬스터 끼리 겹치지 않게 하기 관련 변수...
	static Vector2D a_CacVec;
	static float a_CacSDist;
	static float a_CacMargin = 0.0f;
	static float a_RadiusHap;
	static Vector2D a_CacMovePos;
	static Vector2D a_CacSNormal;
	static float a_CacShiftLen = 0.0f;
	static CMonster* a_OwnMon = NULL;
	static CMonster* a_OtherMon = NULL;
	static list<CMonster*>::iterator a_OtIter;  //<--- 메모리 풀 순환용 변수
	for (a_iter = m_ActMonList.begin(); a_iter != m_ActMonList.end(); a_iter++)
	{
		a_OwnMon = (*a_iter);

		a_OwnMon->Update_Unit(a_DeltaTime, a_Center, a_CamPos, a_Hero);

		//a_CacVec = a_OwnMon->m_CurPos - m_CamPos;
		//if (m_ScCollRadius < a_CacVec.Magnitude()) //컬링
		if (a_OwnMon->m_InScRect == false)  //컬링
			continue;

		//몬스터 끼리 겹치지 않게 하기...
		for (a_OtIter = m_ActMonList.begin(); a_OtIter != m_ActMonList.end(); a_OtIter++)
		{
			a_OtherMon = (*a_OtIter);

			if (a_OwnMon == a_OtherMon)
				continue;

			a_CacVec = a_OwnMon->m_CurPos - a_OtherMon->m_CurPos;
			a_CacSDist = a_CacVec.Magnitude();
			a_RadiusHap = a_OwnMon->m_HalfColl + 4 + a_OtherMon->m_HalfColl + 4; //(내반경 + 적반경) 변수 재활용
			a_CacMovePos = a_OwnMon->m_CurPos;
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
			a_OwnMon->m_CurPos = a_CacMovePos;
		}//for (a_OtIter = m_ActMonList.begin(); a_OtIter != m_ActMonList.end(); a_OtIter++)
		//몬스터 끼리 겹치지 않게 하기...

		//------- 주인공과의 충돌처리
		a_CacVec = a_OwnMon->m_CurPos - a_Hero.m_CurPos;
		a_CacSDist = a_CacVec.Magnitude();
		a_RadiusHap = a_OwnMon->m_HalfColl + 4 + a_Hero.m_HalfColl + 4; //(내반경 + 적반경) 변수 재활용
		a_CacMovePos = a_OwnMon->m_CurPos;
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
		a_OwnMon->m_CurPos = a_CacMovePos;
		//------- 주인공과의 충돌처리
	}//for (a_iter = m_ActMonList.begin(); a_iter != m_ActMonList.end(); a_iter++)
	//-----------Monster AI
}

void CMonster_Mgr::MonMgr_Render(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush)
{
	//--- 몬스터 이미지 그리기...
	for (a_iter = m_ActMonList.begin(); a_iter != m_ActMonList.end(); a_iter++)
	{
		if ((*a_iter)->m_InScRect == false) //컬링
			continue;

		(*a_iter)->Render_Unit(a_pd2dRTarget, a_pBrush);
	}
	//--- 몬스터 이미지 그리기...
}

void CMonster_Mgr::MonEdit_Render(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush, VecINT2D& a_Center, Vector2D& a_CamPos)
{
	static Vector2D a_CacVec;
	for (int ii = 0; ii < m_SpawnPos.size(); ii++)
	{
		a_CacVec = m_SpawnPos[ii]->m_Pos - a_CamPos;
		if (m_ScCollRadius < a_CacVec.Magnitude())
			continue;  //컬링
		m_SpawnPos[ii]->Render_SpPos(a_pd2dRTarget, a_pBrush, a_Center, a_CamPos);
	}
}

void CMonster_Mgr::MonMgr_Destroy()
{
	//-------------몬스터 이미지 제거하기
	for (int aii = 0; aii < m_ImgList.size(); aii++)
	{
		if (m_ImgList[aii] != NULL)
		{
			m_ImgList[aii]->Release();
			m_ImgList[aii] = NULL;
		}
	}
	m_ImgList.clear();
	//-------------몬스터 이미지 제거하기

	//몬스터 메모리풀 제거하기... 
	for (a_iter = m_ActMonList.begin(); a_iter != m_ActMonList.end(); a_iter++)
	{
		if ((*a_iter) != NULL)
		{
			(*a_iter)->Destroy_Unit();

			delete (*a_iter);
			(*a_iter) = NULL;
		}
	}//for (a_iter = m_ActMonList.begin(); a_iter != m_ActMonList.end(); a_iter++)
	m_ActMonList.clear();

	for (a_iter = m_InActMonList.begin(); a_iter != m_InActMonList.end(); a_iter++)
	{
		if ((*a_iter) != NULL)
		{
			(*a_iter)->Destroy_Unit();

			delete (*a_iter);
			(*a_iter) = NULL;
		}
	}//for (a_iter = m_InActMonList.begin(); a_iter != m_InActMonList.end(); a_iter++)
	m_InActMonList.clear();
	//몬스터 메모리풀 제거하기... 

	for (int ii = 0; ii < m_SpawnPos.size(); ii++)
	{
		if (m_SpawnPos[ii] != NULL)
		{
			delete m_SpawnPos[ii];
			m_SpawnPos[ii] = NULL;
		}
	}
	m_SpawnPos.clear();  //<--- 스폰 리스트 지우기
}

//---------- Edit 지원 함수
void CMonster_Mgr::AddMonSpPos(Vector2D& a_SpPos)
{
	Vector2D a_CacVec;
	for (int ii = 0; ii < m_SpawnPos.size(); ii++)
	{
		a_CacVec = a_SpPos - m_SpawnPos[ii]->m_Pos;

		if (a_CacVec.Magnitude() < 40)
		{
			if (m_SpawnPos[ii] != NULL)
			{
				delete m_SpawnPos[ii];
				m_SpawnPos[ii] = NULL;
			}
			m_SpawnPos.erase(m_SpawnPos.begin() + ii); //이미 설치되어 있는 위치면 삭제
			return;
		}
	}

	SpawnPos* a_Temp = new SpawnPos();
	a_Temp->m_Pos = a_SpPos;
	a_Temp->m_SocketImg = g_Mon_Mgr.m_ImgList[0];
	m_SpawnPos.push_back(a_Temp);
}
//---------- Edit 지원 함수

void CMonster_Mgr::ReSrcClear()
{
}

void CMonster_Mgr::SpawnMonster(float a_DeltaTime)
{
	//--- 몬스터 Active 감시 및 마리수 카운트 
	static int g_MonCount = 0;
	g_MonCount = m_ActMonList.size();
	//--- 몬스터 Active 감시 및 마리수 카운트 
	CMonster* a_Node = NULL;
	for (int ii = 0; ii < m_SpawnPos.size(); ii++)
	{
		if (m_SpawnPos[ii]->Update_SpPos(a_DeltaTime) == false)
			continue;

		a_Node = NULL;
		if (0 < m_InActMonList.size())
		{
			a_iter = m_InActMonList.begin();  //활동하지 않는 몬스터 첫번째 노드를 꺼내와서
			a_Node = (*a_iter);
			m_InActMonList.erase(a_iter);     //<-- 활동하지 않는 몬스터 리스트에서는 제거
			m_ActMonList.push_back(a_Node);   //활동 노드 몬스터 리스트로 추가해 준다.
		}

		if (a_Node == NULL)
		{
			a_Node = new CMonster();          //새로 추가
			a_Node->Init_Unit();
			m_ActMonList.push_back(a_Node);
		}

		if (a_Node != NULL)
		{
			a_Node->m_CharicType = CT_Zombi1;
			a_Node->Spawn(m_SpawnPos[ii]->m_Pos.x, m_SpawnPos[ii]->m_Pos.y);
			a_Node->m_SpawnIdx = ii;
		}
	}//for (int ii = 0; ii < m_SpawnPos.size(); ii++)
}

bool CMonster_Mgr::CheckBulletColl(Vector2D a_BL_Pos, float a_BL_Rad, Vector2D& a_MonPos, float a_TakeDamage)
{
	static Vector2D a_CacBVec;
	static Vector2D a_BaseVec;
	static CMonster* a_MonNode = NULL;
	static list<CMonster*>::iterator a_OMIter;
	static CMonster* a_OM_Mon = NULL;
	//총알이 몬스터에 맞았으면 제거해 준다. 몬스터 제거 총알 제거
	if (0 < m_ActMonList.size())
		for (a_iter = m_ActMonList.begin(); a_iter != m_ActMonList.end();) //a_iter++)
		{
			a_MonNode = (*a_iter);
			//if (a_MonNode->m_isActive == false)
			// continue;

			if (a_MonNode->m_InScRect == false)  //컬링
			{
				a_iter++;
				continue;
			}

			a_CacBVec = a_MonNode->m_CurPos - a_BL_Pos;
			if (a_CacBVec.Magnitude() < (a_MonNode->m_HalfColl + a_BL_Rad))   //몬스터의 반지름 10 + 총알의 반지름 10 + 10(좀  넓게)
			{
				a_MonNode->m_CurHP = a_MonNode->m_CurHP - a_TakeDamage;

				if (a_MonNode->m_CurHP <= 0.0f)
				{
					// CHero::m_KillCount++;

					// //--------- Item 스폰
					// g_ItemMgr.SpawnItem(a_MonNode->m_CurPos);
					// //--------- Item 스폰

					//----리스폰 시간 제충전
					if (0 <= a_MonNode->m_SpawnIdx)
					{
						if (a_MonNode->m_SpawnIdx < m_SpawnPos.size())
						{
							m_SpawnPos[a_MonNode->m_SpawnIdx]->m_SpDelay = ((rand() % 351) + 150) / 100.0f; //다시 생성해 준다.  
						}
					}
					//----리스폰 시간 제충전

					//-------- 맞은 몬스터 제거
					a_MonNode->m_isActive = false;
					a_MonNode->m_SpawnIdx = -1;
					//-------- 맞은 몬스터 제거

					a_MonPos.x = a_MonNode->m_CurPos.x;
					a_MonPos.y = a_MonNode->m_CurPos.y;

					a_MonNode->m_CurHP = a_MonNode->m_MaxHP; //다시 리젠
					a_MonNode->m_AggroTarget = NULL;
					a_MonNode->m_AIState = MAI_Patrol;

					a_iter = m_ActMonList.erase(a_iter); //활동 노드 몬스터 리스트에서 제거해 주고
					//a_MonNode->m_isActive = false;    //총알 제거
					m_InActMonList.push_back(a_MonNode);  //활동하지 않는 몬스터 리스트로 추가해 준다.
				}
				else
				{
					a_BaseVec = a_MonNode->m_CurPos;
					a_MonNode->m_AggroTarget = m_Hero;
					a_MonNode->m_AIState = MAI_AggroTrace;

					for (a_OMIter = m_ActMonList.begin(); a_OMIter != m_ActMonList.end(); a_OMIter++)
					{
						a_OM_Mon = (*a_OMIter);
						if (a_MonNode == a_OM_Mon)
							continue;
						if (a_OM_Mon->m_isActive == false)
							continue;
						if (a_OM_Mon->m_AIState == MAI_AggroTrace)
							continue;
						a_CacBVec = a_OM_Mon->m_CurPos - a_BaseVec;
						if (a_CacBVec.Magnitude() < 400.0f)    //주변 몬스터 모두 공격모드로...
						{
							a_OM_Mon->m_AggroTarget = m_Hero;
							a_OM_Mon->m_AIState = MAI_AggroTrace;
						}
					}//for (a_OMIter = m_ActMonList.begin(); a_OMIter != m_ActMonList.end(); a_OMIter++)
				}

				return true;  //총알 제거
			}

			a_iter++;
		}
	//총알이 몬스터에 맞았으면 제거해 준다. 몬스터 제거 총알 제거

	return false;
}

CMonster_Mgr g_Mon_Mgr;