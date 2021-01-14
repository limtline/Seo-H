#pragma once
#include "Vector2D.h"

//----- GDI+ Image 사용을 위한 헤더 추가
#include <ole2.h>  
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;
//----- GDI+ Image 사용을 위한 헤더 추가

#include <list>
#include <vector>
using namespace std;

enum ItemType
{
	IT_Gold = 0,
	IT_Silver,
	IT_Potion,
	IT_Length
};

//--------------------------- CItem class
class CItem
{
public:
	ItemType	m_IT_Type;
	float		m_Duration;
	Vector2D	m_CurPos;
	float       m_HalfWidth;
	float		m_HalfHeight;
	Vector2D	a_CacIVec;
	Image*		m_ItemImg;

public:
	CItem();
	~CItem();

	bool ItemUpdate(float a_DeltaTime);
	void ItemRender(HDC a_hDC);
};
//--------------------------- CItem class

//--------------------------- CItem_Mgr class
class CItem_Mgr
{
	//------ Item List
	list <CItem*> m_ItemList;
	list <CItem*>::iterator m_Iiter;
	//------ Item List

	vector<Image*>    m_ImgList;  //아이템 이미지 리스트

public:
	CItem_Mgr();
	~CItem_Mgr();

public:
	void ItemMgr_Init();						//초기화 함수
	void ItemMgr_Update(float a_DeltaTime);		//아이템 연산 담당부분
	void ItemMgr_Render(HDC a_MainDC);			//아아템 그리기 부분
	void ItemMgr_Destroy();						//아이템 리소스 삭제 부분

	void SpawnItem(Vector2D a_StartV, bool a_isBoss = false);			//아이템 스폰
	void ReSrcClear();							//라운드를 넘어갈 때 필드에 뿌려져 있는 모든 아이템 삭제 함수
};
//--------------------------- CItem_Mgr class

extern CItem_Mgr g_ItemMgr;