#pragma once
#include "Vector2D.h"
#include "CUnit.h"

class CHero : public CUnit
{
public:
	float    m_HalfColl = 20.0f;    //주인공의 충돌 반경
	Vector2D m_CurPos;
	int      m_CurHP;
	int      m_MaxHp;

	//------ 능력치
	float m_CurAttack = 10.0f;	//주인공의 공격력
	int   m_KillCount = 0;		//킬수 
	int   m_SkillCount = 5;		//남은 스킬 수 
	int   m_ShieldCount = 5;	//남은 보호막 수
	int   m_MyMoney = 0;		//유저의 골드
	//------ 능력치

private:
	HWND	 m_hWnd;

	float   m_Speed = 300.0f;

	float   m_AttackSpeed = 0.1f;  //공격속도
	float   m_BLCycle = 0.0f;

	HBRUSH	 m_R_brsh;
	HGDIOBJ  m_R_Old_brsh;

	//------ 쉴드
	float  m_DeltaTime = 0.0f;
	Image* m_Shield;
	float  m_SdDuration = 8.0f;

public:
	float  m_SdOnTime = 0.0f;
	float  m_SdHalfSize;
	//------ 쉴드
	float    m_STDelay = 0.5f;     //게임 리스타트시에 궁극기 딜레이 주기

public:
	CHero();
	~CHero();

public:
	void Init_Unit(HWND a_hWnd);
	void Update_Unit(float a_DeltaTime, RECT& a_RT);
	void Render_Unit(HDC a_hDC);
	void Destroy_Unit();

	void HeroLimitMove(RECT& a_RT);
	void TakeDamage(float a_Damage = 10.0f);

	void Load_Shield();
	void ShieldUpdate();
	void ShieldRender(Graphics& graphics);
	void ShieldDestroy();

	static void BuyHP();
	static void BuySkill();
	static void BuyShield();
};

extern CHero g_Hero;