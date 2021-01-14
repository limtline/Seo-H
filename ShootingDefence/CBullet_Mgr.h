#pragma once
#include "Vector2D.h"

#include <vector>
using namespace std;

class CMonster_Mgr;

enum UClassType
{
	CT_Unit = 0,
	CT_Hero,
	CT_Monster,
};

enum BulletType
{
	BT_Normal = 0,
	BT_Skill1,
};

//----------------------------- CBullet
class CBullet
{
public:
	bool	    m_BLActive;   //총알의 액티브 상태
	BulletType  m_BL_Type;    //총알이 일반 총알인지? 스킬 총알인지?
	UClassType  m_UC_Type;	  //누가 쏜 총알인지?

	float		m_LifeTime;   //총알의 라이프 타임(총알을 몇초동안 날아가게 할 것인지의 시간)
	Vector2D	m_CurPos;     //총알의 현재 위치
	Vector2D	m_DirVec;     //총알의 날아가는 방향벡터
	float		m_MoveSpeed;  //총알의 이동 속도

	float       m_HalfColl;   //충돌 반경

	static HBRUSH	m_R_brsh;
	static HBRUSH	m_G_brsh;
	HGDIOBJ		a_R_Old_brsh;

public:
	CBullet();
	~CBullet();

	bool BulletUpdate(float a_DeltaTime);
	void BulletRender(HDC a_hDC);
};
//----------------------------- CBullet

//----------------------------- CBullet_Mgr
class CBullet_Mgr
{
private:
	vector<CBullet*> m_BullList;  //<--- 총알 풀 방식

	HBRUSH	m_h_brsh = NULL;
	HGDIOBJ m_h_Old_brsh = NULL;
	HPEN	m_h_pen = NULL;  //선종류(실선), 두께(1), RGB색
	HGDIOBJ	m_h_Oldpen = NULL;

public:
	CBullet_Mgr();
	~CBullet_Mgr();

public:
	void BLMgr_Init();
	void BLMgr_Update(float a_DeltaTime, HWND a_hWnd, CMonster_Mgr* a_MonMgr = NULL);
	void BLMgr_Render(HDC a_hDC);
	void BLMgr_Destroy();

	void SpawnBullet(Vector2D a_StartV, Vector2D a_TargetV,
					UClassType a_Own_Type = CT_Hero, BulletType a_BLTP = BT_Normal);

	void ReSrcClear();
};
//----------------------------- CBullet_Mgr

extern CBullet_Mgr g_Bullet_Mgr;

