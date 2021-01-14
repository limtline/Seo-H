#pragma once

#pragma comment (lib, "d2d1.lib")     //<---Direct2D ����ϱ�...
//WIC��� ���̺귯���� �̹����κ��� �̹��� �����͸� ���� �� �ִ�.
#include <d2d1.h>
#include "Vector2D.h"

enum CT_Type   //CharicType
{
	CT_None = 0,
	CT_Nanami,
	CT_Zombi1,
	//CT_Zombi2,
	//CT_Zombi3,
	//CT_Zombi4,
	//CT_Zombi6,
	//CT_Zombi8,
	//CT_Boss,
	CTT_Length,
};

class CHero;  //���漱��

class CMonster
{
public:
	CT_Type m_CharicType;			  //ĳ���� ������ "����"����? "������"����?
	int     m_SpawnIdx;               //vector<SpawnPos>  m_SpawnPos;�� �ε���

	ID2D1Bitmap* m_SocketImg;		  //�ִ� ����ó�� ����� ������
	bool	     m_isActive;
	float        m_MaxHP = 100.0f;
	float        m_CurHP = 100.0f;

	////--- ���� ��ǥ ���� ������....
	Vector2D     m_CurPos;
	float        m_Speed = 150.0f;
	Vector2D     m_DirVec;
	float        m_HalfColl = 17;  //������ �浹 �ݰ�
	////--- ���� ��ǥ ���� ������....

	//----------������ǥ �̵� ���� ����
	VecINT2D        m_ScreenHalf;       //ȭ�� �� ������ ����� ����
	Vector2D        m_CamPos = { 0.0f, 0.0f };           //���� ��ġ
	Vector2D        m_RenderPos;        //������ ��ġ
	//----------������ǥ �̵� ���� ����

	//---------- ���� AI ������...
	CHero* m_AggroTarget = NULL;
	enum MonAIState m_AIState; 
	Vector2D   a_CacVLen = { 0.0f, 0.0f };
	float	   a_CacDist = 0.0f;
	bool	   m_bMvPtOnOff = false;            //Patrol Move
	float      m_MoveVelocity = 50.0f;
	float	   m_NowStep = 0.0f;
	Vector2D   m_PatrolTarget = { 0.0f, 0.0f }; //Patrol�� �������ߵ� ���� ��ǥ ��ǥ
	Vector2D   a_CacEndVec = { 0.0f, 0.0f };
	Vector2D   m_DirMvVec = { 0.0f, 0.0f };     //Patrol�� �������ߵ� ���� ����
	double	   m_MoveDurTime = 0.0;     //��ǥ������ �����ϴµ� �ɸ��� �ð�
	double	   m_AddTimeCount = 0.0;    //�����ð� ī��Ʈ 
	float	   m_WaitTime = 0.0f;       //Patrol�ÿ� ��ǥ���� �����ϸ� ��� ����Ű�� ���� ���� �ð�����
	int		   a_AngleRan;
	int		   a_LengthRan;
	Vector2D   a_DirPtVec = { 0.0f, 0.0f };
	Vector2D   m_BasePos = { 0.0f, 0.0f };  //������ �ʱ� ���� ��ġ(�������� �ȴ�.)
	float 	   a_CacAngle = 0.0f;
	Vector2D   a_Vect;
	Vector2D   m_MoveDir = { 0.0f, 0.0f };   //��� ���� ����
	Vector2D   a_MoveNextStep = { 0.0f, 0.0f };
	float	   m_TraceDist = 200.0f;          //�����Ÿ�
	//---------- ���� AI ������...

	bool	   m_InScRect = false;

public:
	CMonster();
	~CMonster();

public:
	void Init_Unit();
	void Update_Unit(double a_DeltaTime, VecINT2D& a_Center, Vector2D& a_CamPos, CHero& a_Hero);
	void Render_Unit(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush);
	void Destroy_Unit();

	void Spawn(float a_XX, float a_YY);
	void MonsterAI(float a_DeltaTime, CHero& a_Hero);
	bool CheckAABB(Vector2D& AACen, Vector2D& AAHLen, Vector2D& BBCen, Vector2D& BBHLen);
};

