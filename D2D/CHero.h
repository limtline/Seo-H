#pragma once

#include "Vector2D.h"
#pragma comment (lib, "d2d1.lib")     //<---Direct2D 사용하기...
#include <d2d1.h>

class CHero
{
public:
	CHero();
	~CHero();

public:
	ID2D1Bitmap* m_SocketImg = NULL;	//애니 소켓처럼 사용할 포인터

	//----------캐릭터 이동 계산용 변수
	Vector2D		m_CurPos = { 0.0f, 0.0f };           //화면 중점 위치
	double			m_MoveSpeed;        //이동 속도
	//----------캐릭터 이동 계산용 변수

	//----------절대좌표 이동 계산용 변수
	VecINT2D		m_ScreenHalf;       //화면 반 사이즈 저장용 변수
	Vector2D		m_CamPos = { 0.0f, 0.0f };           //현재 위치
	Vector2D		m_RenderPos;        //랜더링 위치
	//----------절대좌표 이동 계산용 변수

	//피킹 관련 변수들...
	Vector2D		m_DirVec;
	bool			m_bMoveOnOff = false;
	Vector2D		m_TargetPos;
	//피킹 관련 변수들...

	float			m_HalfColl = 20.0f;    //주인공의 충돌 반경

public:
	void Init_Unit(ID2D1Bitmap* (*D2DLoadBmp)(LPCWSTR FName) = NULL);
	void Update_Unit(double a_DeltaTime);
	void Render_Unit(ID2D1HwndRenderTarget* a_pd2dRTarget, 
					 ID2D1SolidColorBrush* a_pBrush);
	void Destroy_Unit();

	void Abs_Update(VecINT2D& a_ScHalf, Vector2D& a_CamPos);
	void MsPicking(Vector2D a_TgPos);

	void TakeDamage(float a_Damage = 10.0f);
};

