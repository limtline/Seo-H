#pragma once
#include "Vector2D.h"
#include "CUnit.h"
#include "CBossMon.h"

class CHero;
class CBullet;

//----------------------------- CMonster
class CMonster : public CUnit
{
public:
	bool	     m_isActive;
	float        m_MaxHP = 100.0f;
	float        m_CurHP = 100.0f;

	////--- ���� ��ǥ ���� ������....
	Vector2D     m_CurPos;
	float        m_Speed = 150.0f;
	Vector2D     m_DirVec;
	float        m_HalfColl = 17;		//������ �浹 �ݰ�
	////--- ���� ��ǥ ���� ������....

public:
	CMonster();
	~CMonster();

public:
	void Init_Unit();
	void Update_Unit(float a_DeltaTime, CHero& a_Hero);
	void Render_Unit(HDC a_hDC, HBRUSH& a_R_brsh);
	void Destroy_Unit();

	void Spawn(float a_XX, float a_YY);
	void TakeDamage(float a_Value = 10.0f);
};
//----------------------------- CMonster

//----------------------------- CMonster_Mgr
class CMonster_Mgr
{
private:
	HBRUSH	 m_R_brsh;
	HGDIOBJ  m_R_Old_brsh;
	float	 m_BossSpawnTm = 3.0f;

public:
	vector<Image*>    m_ImgList;      //���� �̹��� ����Ʈ
	vector<CMonster*> m_MonList;	  //<--- �޸� Ǯ ���
	vector<CBossMon*> m_BossList;	  //<--- ���� ���� ����Ʈ
	RECT   m_cRT;					  //���� ����

	int	   m_SpawnLim = 6;            //�ʵ忡 �����ϴ� ���� ������ ���� ����

public:
	CMonster_Mgr();
	~CMonster_Mgr();

public:
	void MonMgr_Init(HWND a_hWnd);
	void MonMgr_Update(float a_DeltaTime, HWND a_hWnd, CHero& a_Hero);
	void MonMgr_Render(HDC a_hDC);
	void MonMgr_Destroy();

	void SpawnMonster(float a_DeltaTime, HWND a_hWnd);
	void TakeDamage_MonMgr(CBullet* a_RefBullet);

	void ReSrcClear();	//���带 �Ѿ �� �ʵ忡 ������ �ִ� ��� ���� ���� �Լ�

	CT_Type GetSpMonType();  //���̵��� ���� ������ ���� ������ ������ �ִ� �Լ�
	float GetSpTimeLevel();  //���̵��� ���� ���� ���� �ֱ⸦ ������ �ִ� �Լ� 

};

extern CMonster_Mgr g_Mon_Mgr;
//----------------------------- CMonster_Mgr
