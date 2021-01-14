#pragma once

#include <d2d1.h>
#include "Vector2D.h"

#include <list>
#include <vector>
using namespace std;

class CHero;  //전방선언
class CMonster;
class SpawnPos;

class CMonster_Mgr
{
	list<CMonster*> m_ActMonList;        //Active 활동중인 몬스터 리스트		<--- 메모리 풀 방식
	list<CMonster*> m_InActMonList;		 //InActive 활동하지 않는 몬스터 리스트  <--- 메모리 풀 방식
	list<CMonster*>::iterator a_iter;	 //<--- 메모리 풀 순환용 변수

	float  m_ScCollRadius = 700.0f;      //화면의 대각선길이 + 50정도
	//----------절대좌표 이동 계산용 변수
	VecINT2D   m_ScreenHalf;       //화면 반 사이즈 저장용 변수
	Vector2D   m_CamPos = { 0.0f, 0.0f };           //현재 위치
	//----------절대좌표 이동 계산용 변수

public:
	vector<ID2D1Bitmap*>  m_ImgList;     //몬스터 이미지 리스트
	vector<SpawnPos*>     m_SpawnPos;    //몬스터 스폰 위치

	class CHero* m_Hero = NULL;

public:
	CMonster_Mgr();
	~CMonster_Mgr();

public:
	void MonMgr_Init(HWND a_hWnd, ID2D1Bitmap* (*D2DLoadBmp)(LPCWSTR FName) = NULL);
	void MonMgr_Update(double a_DeltaTime, VecINT2D& a_Center, Vector2D& a_CamPos, CHero& a_Hero);
	void MonMgr_Render(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush);
	void MonEdit_Render(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush,
		VecINT2D& a_Center, Vector2D& a_CamPos);
	void MonMgr_Destroy();

	void AddMonSpPos(Vector2D& a_SpPos);

	void ReSrcClear();	//라운드를 넘어갈 때 필드에 등장해 있는 모든 몬스터 삭제 함수
	void SpawnMonster(float a_DeltaTime);

	bool CheckBulletColl(Vector2D a_BL_Pos, float a_BL_Rad, 
						Vector2D& a_MonPos, float a_TakeDamage = 10.0f);
};

extern CMonster_Mgr g_Mon_Mgr;