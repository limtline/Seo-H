#pragma once

#include "Vector2D.h"
#include <d2d1.h>

enum MonAIState
{
	MAI_Idle,          //숨쉬기 상태
	MAI_Patrol,        //패트롤 상태
	MAI_AggroTrace,    //적으로부터 공격을 당했을 때 추적 상태
	MAI_NormalTrace,   //일반 추적 상태
	MAI_ReturnPos,     //추적 놓쳤을 때 재자리로 돌아오는 상태
	MAI_Attack,        //공격 상태
};

class SpawnPos
{
public:
	Vector2D     m_Pos = { 0.0f, 0.0f };
	float        m_SpDelay = 0.1f;
	ID2D1Bitmap* m_SocketImg;		  //애니 소켓처럼 사용할 포인터

public:
	SpawnPos();
	~SpawnPos();

	bool Update_SpPos(float a_DeltaTime);
	void Render_SpPos(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush,
		VecINT2D& a_ScreenHalf, Vector2D& a_CamPos);
};

