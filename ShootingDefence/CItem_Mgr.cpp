#include "stdafx.h"
#include "CItem_Mgr.h"
#include "CHero.h"

//--------------------------- CItem class
CItem::CItem()
{
	m_IT_Type = IT_Gold;
	m_Duration = 10.0f;
	m_HalfWidth = 0.0f;
	m_HalfHeight = 0.0f;
	m_ItemImg = NULL;
}

CItem::~CItem()
{
}

bool CItem::ItemUpdate(float a_DeltaTime)
{
	m_Duration = m_Duration - a_DeltaTime;
	if (m_Duration < 0.0f)
	{
		return false;  //일정 시간뒤에 제거해 준다.
	}

	a_CacIVec = g_Hero.m_CurPos - m_CurPos;
	if (a_CacIVec.Magnitude() < (g_Hero.m_HalfColl + m_HalfWidth))   //주인공의 반지름 10 + Item의 반지름 10
	{
		if (m_IT_Type == IT_Potion)
		{
			g_Hero.m_CurHP = g_Hero.m_CurHP + 20.0f;
			if (g_Hero.m_MaxHp < g_Hero.m_CurHP)
			{
				g_Hero.m_CurHP = g_Hero.m_MaxHp;
			}
		}
		else if (m_IT_Type == IT_Gold)
		{
			g_Hero.m_MyMoney = g_Hero.m_MyMoney + 100;
		}
		else if (m_IT_Type == IT_Silver)
		{
			g_Hero.m_MyMoney = g_Hero.m_MyMoney + 50;
		}

		return false;

	}//if (a_CacIVec.Magnitude() < ((*a_Miter)->m_HalfColl + 10)) 

	return true;
}

void CItem::ItemRender(HDC a_hDC)
{
	if (m_ItemImg == NULL)
		return;

	Graphics graphics(a_hDC);
	graphics.DrawImage(m_ItemImg, m_CurPos.x - m_HalfWidth, m_CurPos.y - m_HalfHeight,
					  (float)m_ItemImg->GetWidth(), (float)m_ItemImg->GetHeight());
}
//--------------------------- CItem class

//--------------------------- CItem_Mgr class
CItem_Mgr::CItem_Mgr()
{
}

CItem_Mgr::~CItem_Mgr()
{
}

void CItem_Mgr::ItemMgr_Init()
{
	//----아이템 이미지 로딩하기
	Image* a_ImgNode = NULL;
	for (int aii = 0; aii < (int)IT_Length; aii++)
	{
		a_ImgNode = NULL;
		if (aii == (int)IT_Gold)
		{
			a_ImgNode = Image::FromFile(_T(".\\RscTrunk\\gold.png"));
		}
		else if (aii == (int)IT_Silver)
		{
			a_ImgNode = Image::FromFile(_T(".\\RscTrunk\\silver.png"));
		}
		else if (aii == (int)IT_Potion)
		{
			a_ImgNode = Image::FromFile(_T(".\\RscTrunk\\potion.png"));
		}

		m_ImgList.push_back(a_ImgNode);
	}
	//----아이템 이미지 로딩하기
}

void CItem_Mgr::ItemMgr_Update(float a_DeltaTime)
{
	//------------------ 중간에 조건에 걸리는 모든 아이템들을 제거 하는 방법
	if (!m_ItemList.empty())
	{
		for (m_Iiter = m_ItemList.begin(); m_Iiter != m_ItemList.end();)
		{
			if ((*m_Iiter)->ItemUpdate(a_DeltaTime) == false)
			{
				if ((*m_Iiter) != NULL)
				{
					delete (*m_Iiter);
					(*m_Iiter) = NULL;
				}

				m_Iiter = m_ItemList.erase(m_Iiter);  // erase() 이후 it가 다음것을 가리키게 되므로 it를 증가시키지 않는다.
			}
			else
			{
				++m_Iiter;
			}
		}//for (m_Iiter = m_ItemList.begin(); m_Iiter != m_ItemList.end(); m_Iiter++)
	}// if (!m_ItemList.empty())
	//------------------ 중간에 조건에 걸리는 모든 아이템들을 제거 하는 방법
}

void CItem_Mgr::ItemMgr_Render(HDC a_MainDC)
{
	//----아이템 그려주기
	if (!m_ItemList.empty())
	{
		for (m_Iiter = m_ItemList.begin(); m_Iiter != m_ItemList.end(); m_Iiter++)
		{
			if ((*m_Iiter) != NULL)
			{
				(*m_Iiter)->ItemRender(a_MainDC);
			}
		}//for (m_Iiter = m_StdList.begin(); m_Iiter != m_StdList.end(); m_Iiter++)

	}// if (!m_StdList.empty())
	//----아이템 그려주기
}

void CItem_Mgr::ItemMgr_Destroy()
{
	//-------------아이템 이미지 제거하기
	for (int aii = 0; aii < (int)IT_Length; aii++)
	{
		if (m_ImgList[aii] != NULL)
		{
			delete m_ImgList[aii];
			m_ImgList[aii] = NULL;
		}
	}
	m_ImgList.clear();
	//-------------아이템 이미지 제거하기

	ReSrcClear();
}

void CItem_Mgr::SpawnItem(Vector2D a_StartV, bool a_isBoss)
{
	int a_CacRand = 0;//rand() % 10;
	static CItem* a_INode = NULL;
	a_INode = NULL;

	if (a_isBoss == true)
	{
		a_CacRand = rand() % 5;
	}
	else
	{
		a_CacRand = rand() % 10;
	}

	if (a_CacRand == 0)
	{
		a_INode = new CItem();
		a_INode->m_IT_Type = IT_Potion;
	}
	else if (1 <= a_CacRand && a_CacRand <= 2)
	{
		a_INode = new CItem();
		a_INode->m_IT_Type = IT_Gold;
	}
	else if (8 <= a_CacRand && a_CacRand <= 9)
	{
		a_INode = new CItem();
		a_INode->m_IT_Type = IT_Silver;
	}

	if (a_isBoss == true)
	if (3 <= a_CacRand && a_CacRand <= 4)
	{
		a_INode = new CItem();
		a_INode->m_IT_Type = IT_Silver;
	}

	if (a_INode != NULL)
	{
		a_INode->m_CurPos.x = a_StartV.x;
		a_INode->m_CurPos.y = a_StartV.y;
		if (m_ImgList[(int)a_INode->m_IT_Type] != NULL)
		{
			a_INode->m_HalfWidth = m_ImgList[(int)a_INode->m_IT_Type]->GetWidth() / 2;      //기본 이미지의 가로 반사이즈
			a_INode->m_HalfHeight = m_ImgList[(int)a_INode->m_IT_Type]->GetHeight() / 2;    //기본 이미지의 세로 반사이즈
			a_INode->m_ItemImg = m_ImgList[(int)a_INode->m_IT_Type];
		}
		m_ItemList.push_back(a_INode);
	}//if (a_INode != NULL)

}

void CItem_Mgr::ReSrcClear()
{
	//-------------필드상의 아이템 제거하기
	if (!m_ItemList.empty())
	{
		for (m_Iiter = m_ItemList.begin(); m_Iiter != m_ItemList.end(); m_Iiter++)
		{
			if ((*m_Iiter) != NULL)
			{
				delete (*m_Iiter);
				(*m_Iiter) = NULL;
			}
		}//for (m_Iiter = m_StdList.begin(); m_Iiter != m_StdList.end(); m_Iiter++)

		m_ItemList.clear();
	}// if (!m_StdList.empty())
	//-------------필드상의 아이템 제거하기
}
//--------------------------- CItem_Mgr class

CItem_Mgr g_ItemMgr;

