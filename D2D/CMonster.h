#pragma once

#pragma comment (lib, "d2d1.lib")     //<---Direct2D 사용하기...
//WIC라는 라이브러리의 이미지로부터 이미지 데이터를 만들 수 있다.
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

class CHero;  //전방선언

class CMonster
{
public:
	CT_Type m_CharicType;			  //캐릭터 종류가 "몬스터"인지? "나나미"인지?
	int     m_SpawnIdx;               //vector<SpawnPos>  m_SpawnPos;의 인덱스

	ID2D1Bitmap* m_SocketImg;		  //애니 소켓처럼 사용할 포인터
	bool	     m_isActive;
	float        m_MaxHP = 100.0f;
	float        m_CurHP = 100.0f;

	////--- 몬스터 좌표 계산용 변수들....
	Vector2D     m_CurPos;
	float        m_Speed = 150.0f;
	Vector2D     m_DirVec;
	float        m_HalfColl = 17;  //몬스터의 충돌 반경
	////--- 몬스터 좌표 계산용 변수들....

	//----------절대좌표 이동 계산용 변수
	VecINT2D        m_ScreenHalf;       //화면 반 사이즈 저장용 변수
	Vector2D        m_CamPos = { 0.0f, 0.0f };           //현재 위치
	Vector2D        m_RenderPos;        //랜더링 위치
	//----------절대좌표 이동 계산용 변수

	//---------- 몬스터 AI 변수들...
	CHero* m_AggroTarget = NULL;
	enum MonAIState m_AIState; 
	Vector2D   a_CacVLen = { 0.0f, 0.0f };
	float	   a_CacDist = 0.0f;
	bool	   m_bMvPtOnOff = false;            //Patrol Move
	float      m_MoveVelocity = 50.0f;
	float	   m_NowStep = 0.0f;
	Vector2D   m_PatrolTarget = { 0.0f, 0.0f }; //Patrol시 움직여야될 다음 목표 좌표
	Vector2D   a_CacEndVec = { 0.0f, 0.0f };
	Vector2D   m_DirMvVec = { 0.0f, 0.0f };     //Patrol시 움직여야될 방향 벡터
	double	   m_MoveDurTime = 0.0;     //목표점까지 도착하는데 걸리는 시간
	double	   m_AddTimeCount = 0.0;    //누적시간 카운트 
	float	   m_WaitTime = 0.0f;       //Patrol시에 목표점에 도착하면 잠시 대기시키기 위한 랜덤 시간변수
	int		   a_AngleRan;
	int		   a_LengthRan;
	Vector2D   a_DirPtVec = { 0.0f, 0.0f };
	Vector2D   m_BasePos = { 0.0f, 0.0f };  //몬스터의 초기 스폰 위치(기준점이 된다.)
	float 	   a_CacAngle = 0.0f;
	Vector2D   a_Vect;
	Vector2D   m_MoveDir = { 0.0f, 0.0f };   //평면 진행 방향
	Vector2D   a_MoveNextStep = { 0.0f, 0.0f };
	float	   m_TraceDist = 200.0f;          //추적거리
	//---------- 몬스터 AI 변수들...

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

