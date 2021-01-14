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
	//----���� �̹��� �ε��ϱ�
	ID2D1Bitmap* a_ImgNode = NULL;

	if (D2DLoadBmp != NULL)
		a_ImgNode = D2DLoadBmp(_T(".\\MonsterImg\\Zombi1.png"));

	m_ImgList.push_back(a_ImgNode);
	//----���� �̹��� �ε��ϱ�

	//----���� �޸�Ǯ�� �̸� ����� ����
	CMonster* a_Node = NULL;
	for (int aii = 0; aii < 20; aii++)
	{
		a_Node = new CMonster();
		a_Node->Init_Unit();
		m_InActMonList.push_back(a_Node);
	}
	//----���� �޸�Ǯ�� �̸� ����� ����

}

void CMonster_Mgr::MonMgr_Update(double a_DeltaTime, VecINT2D& a_Center, Vector2D& a_CamPos, CHero& a_Hero)
{
	m_Hero = &a_Hero;

	m_ScCollRadius = sqrtf(a_Center.x * a_Center.x + a_Center.y * a_Center.y);
	m_ScCollRadius = m_ScCollRadius + 50.0f;

	m_ScreenHalf = a_Center; //Main DC �� ���� ���� �ݻ�����
	m_CamPos = a_CamPos;

	//---------- �ֱ����� Monster Spawn   
	SpawnMonster(a_DeltaTime);
	//---------- �ֱ����� Monster Spawn  

	//-----------Monster AI
	//���� ���� ��ġ�� �ʰ� �ϱ� ���� ����...
	static Vector2D a_CacVec;
	static float a_CacSDist;
	static float a_CacMargin = 0.0f;
	static float a_RadiusHap;
	static Vector2D a_CacMovePos;
	static Vector2D a_CacSNormal;
	static float a_CacShiftLen = 0.0f;
	static CMonster* a_OwnMon = NULL;
	static CMonster* a_OtherMon = NULL;
	static list<CMonster*>::iterator a_OtIter;  //<--- �޸� Ǯ ��ȯ�� ����
	for (a_iter = m_ActMonList.begin(); a_iter != m_ActMonList.end(); a_iter++)
	{
		a_OwnMon = (*a_iter);

		a_OwnMon->Update_Unit(a_DeltaTime, a_Center, a_CamPos, a_Hero);

		//a_CacVec = a_OwnMon->m_CurPos - m_CamPos;
		//if (m_ScCollRadius < a_CacVec.Magnitude()) //�ø�
		if (a_OwnMon->m_InScRect == false)  //�ø�
			continue;

		//���� ���� ��ġ�� �ʰ� �ϱ�...
		for (a_OtIter = m_ActMonList.begin(); a_OtIter != m_ActMonList.end(); a_OtIter++)
		{
			a_OtherMon = (*a_OtIter);

			if (a_OwnMon == a_OtherMon)
				continue;

			a_CacVec = a_OwnMon->m_CurPos - a_OtherMon->m_CurPos;
			a_CacSDist = a_CacVec.Magnitude();
			a_RadiusHap = a_OwnMon->m_HalfColl + 4 + a_OtherMon->m_HalfColl + 4; //(���ݰ� + ���ݰ�) ���� ��Ȱ��
			a_CacMovePos = a_OwnMon->m_CurPos;
			if (a_CacSDist < a_RadiusHap) //��ģ ����
			{
				a_CacMargin = a_RadiusHap - a_CacSDist;
				a_CacSNormal = a_CacVec; //�з����� ����
				a_CacSNormal.Normalize();
				a_CacShiftLen = a_CacMargin;
				if (a_RadiusHap < a_CacShiftLen) //������ġ �ð����ݵ� ���������� Ŀ���� �ȵȴ�. 
				{
					a_CacShiftLen = a_RadiusHap;
				}
				a_CacMovePos = a_CacMovePos + (a_CacSNormal * a_CacShiftLen);
			}//if (a_CacSDist < a_RadiusHap)
			a_OwnMon->m_CurPos = a_CacMovePos;
		}//for (a_OtIter = m_ActMonList.begin(); a_OtIter != m_ActMonList.end(); a_OtIter++)
		//���� ���� ��ġ�� �ʰ� �ϱ�...

		//------- ���ΰ����� �浹ó��
		a_CacVec = a_OwnMon->m_CurPos - a_Hero.m_CurPos;
		a_CacSDist = a_CacVec.Magnitude();
		a_RadiusHap = a_OwnMon->m_HalfColl + 4 + a_Hero.m_HalfColl + 4; //(���ݰ� + ���ݰ�) ���� ��Ȱ��
		a_CacMovePos = a_OwnMon->m_CurPos;
		if (a_CacSDist < a_RadiusHap)
		{
			a_CacMargin = a_RadiusHap - a_CacSDist;
			a_CacSNormal = a_CacVec; //�з����� ����
			a_CacSNormal.Normalize();
			a_CacShiftLen = a_CacMargin;
			if (a_RadiusHap < a_CacShiftLen) //������ġ �ð����ݵ� ���������� Ŀ���� �ȵȴ�. 
			{
				a_CacShiftLen = a_RadiusHap;
			}
			a_CacMovePos = a_CacMovePos + (a_CacSNormal * a_CacShiftLen);
		}//if (a_CacSDist < a_RadiusHap)
		a_OwnMon->m_CurPos = a_CacMovePos;
		//------- ���ΰ����� �浹ó��
	}//for (a_iter = m_ActMonList.begin(); a_iter != m_ActMonList.end(); a_iter++)
	//-----------Monster AI
}

void CMonster_Mgr::MonMgr_Render(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush)
{
	//--- ���� �̹��� �׸���...
	for (a_iter = m_ActMonList.begin(); a_iter != m_ActMonList.end(); a_iter++)
	{
		if ((*a_iter)->m_InScRect == false) //�ø�
			continue;

		(*a_iter)->Render_Unit(a_pd2dRTarget, a_pBrush);
	}
	//--- ���� �̹��� �׸���...
}

void CMonster_Mgr::MonEdit_Render(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush, VecINT2D& a_Center, Vector2D& a_CamPos)
{
	static Vector2D a_CacVec;
	for (int ii = 0; ii < m_SpawnPos.size(); ii++)
	{
		a_CacVec = m_SpawnPos[ii]->m_Pos - a_CamPos;
		if (m_ScCollRadius < a_CacVec.Magnitude())
			continue;  //�ø�
		m_SpawnPos[ii]->Render_SpPos(a_pd2dRTarget, a_pBrush, a_Center, a_CamPos);
	}
}

void CMonster_Mgr::MonMgr_Destroy()
{
	//-------------���� �̹��� �����ϱ�
	for (int aii = 0; aii < m_ImgList.size(); aii++)
	{
		if (m_ImgList[aii] != NULL)
		{
			m_ImgList[aii]->Release();
			m_ImgList[aii] = NULL;
		}
	}
	m_ImgList.clear();
	//-------------���� �̹��� �����ϱ�

	//���� �޸�Ǯ �����ϱ�... 
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
	//���� �޸�Ǯ �����ϱ�... 

	for (int ii = 0; ii < m_SpawnPos.size(); ii++)
	{
		if (m_SpawnPos[ii] != NULL)
		{
			delete m_SpawnPos[ii];
			m_SpawnPos[ii] = NULL;
		}
	}
	m_SpawnPos.clear();  //<--- ���� ����Ʈ �����
}

//---------- Edit ���� �Լ�
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
			m_SpawnPos.erase(m_SpawnPos.begin() + ii); //�̹� ��ġ�Ǿ� �ִ� ��ġ�� ����
			return;
		}
	}

	SpawnPos* a_Temp = new SpawnPos();
	a_Temp->m_Pos = a_SpPos;
	a_Temp->m_SocketImg = g_Mon_Mgr.m_ImgList[0];
	m_SpawnPos.push_back(a_Temp);
}
//---------- Edit ���� �Լ�

void CMonster_Mgr::ReSrcClear()
{
}

void CMonster_Mgr::SpawnMonster(float a_DeltaTime)
{
	//--- ���� Active ���� �� ������ ī��Ʈ 
	static int g_MonCount = 0;
	g_MonCount = m_ActMonList.size();
	//--- ���� Active ���� �� ������ ī��Ʈ 
	CMonster* a_Node = NULL;
	for (int ii = 0; ii < m_SpawnPos.size(); ii++)
	{
		if (m_SpawnPos[ii]->Update_SpPos(a_DeltaTime) == false)
			continue;

		a_Node = NULL;
		if (0 < m_InActMonList.size())
		{
			a_iter = m_InActMonList.begin();  //Ȱ������ �ʴ� ���� ù��° ��带 �����ͼ�
			a_Node = (*a_iter);
			m_InActMonList.erase(a_iter);     //<-- Ȱ������ �ʴ� ���� ����Ʈ������ ����
			m_ActMonList.push_back(a_Node);   //Ȱ�� ��� ���� ����Ʈ�� �߰��� �ش�.
		}

		if (a_Node == NULL)
		{
			a_Node = new CMonster();          //���� �߰�
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
	//�Ѿ��� ���Ϳ� �¾����� ������ �ش�. ���� ���� �Ѿ� ����
	if (0 < m_ActMonList.size())
		for (a_iter = m_ActMonList.begin(); a_iter != m_ActMonList.end();) //a_iter++)
		{
			a_MonNode = (*a_iter);
			//if (a_MonNode->m_isActive == false)
			// continue;

			if (a_MonNode->m_InScRect == false)  //�ø�
			{
				a_iter++;
				continue;
			}

			a_CacBVec = a_MonNode->m_CurPos - a_BL_Pos;
			if (a_CacBVec.Magnitude() < (a_MonNode->m_HalfColl + a_BL_Rad))   //������ ������ 10 + �Ѿ��� ������ 10 + 10(��  �а�)
			{
				a_MonNode->m_CurHP = a_MonNode->m_CurHP - a_TakeDamage;

				if (a_MonNode->m_CurHP <= 0.0f)
				{
					// CHero::m_KillCount++;

					// //--------- Item ����
					// g_ItemMgr.SpawnItem(a_MonNode->m_CurPos);
					// //--------- Item ����

					//----������ �ð� ������
					if (0 <= a_MonNode->m_SpawnIdx)
					{
						if (a_MonNode->m_SpawnIdx < m_SpawnPos.size())
						{
							m_SpawnPos[a_MonNode->m_SpawnIdx]->m_SpDelay = ((rand() % 351) + 150) / 100.0f; //�ٽ� ������ �ش�.  
						}
					}
					//----������ �ð� ������

					//-------- ���� ���� ����
					a_MonNode->m_isActive = false;
					a_MonNode->m_SpawnIdx = -1;
					//-------- ���� ���� ����

					a_MonPos.x = a_MonNode->m_CurPos.x;
					a_MonPos.y = a_MonNode->m_CurPos.y;

					a_MonNode->m_CurHP = a_MonNode->m_MaxHP; //�ٽ� ����
					a_MonNode->m_AggroTarget = NULL;
					a_MonNode->m_AIState = MAI_Patrol;

					a_iter = m_ActMonList.erase(a_iter); //Ȱ�� ��� ���� ����Ʈ���� ������ �ְ�
					//a_MonNode->m_isActive = false;    //�Ѿ� ����
					m_InActMonList.push_back(a_MonNode);  //Ȱ������ �ʴ� ���� ����Ʈ�� �߰��� �ش�.
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
						if (a_CacBVec.Magnitude() < 400.0f)    //�ֺ� ���� ��� ���ݸ���...
						{
							a_OM_Mon->m_AggroTarget = m_Hero;
							a_OM_Mon->m_AIState = MAI_AggroTrace;
						}
					}//for (a_OMIter = m_ActMonList.begin(); a_OMIter != m_ActMonList.end(); a_OMIter++)
				}

				return true;  //�Ѿ� ����
			}

			a_iter++;
		}
	//�Ѿ��� ���Ϳ� �¾����� ������ �ش�. ���� ���� �Ѿ� ����

	return false;
}

CMonster_Mgr g_Mon_Mgr;