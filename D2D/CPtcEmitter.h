#pragma once

#pragma comment (lib, "d2d1.lib")     //<---Direct2D 사용하기..
#include <d2d1.h>
#include "Vector2D.h"

class CPtcEmitter
{
public:
	bool	    m_EmtActive;      //파티클 에미터의 액티브 상태

	ID2D1Bitmap* m_PtcImage = NULL; //파티클 에미터 이미지

	float		m_DirRot = 0.0f; //파티클 에미터의 날아가는 방향각도
	float		LifeTime;        //파티클 에미터 라이프 타임
	Vector2D    SpawnPosition;   //파티클 에미터 초기 시작 위치
	Vector2D    StartSize;       //파티클 에미터 초기 사이즈
	Vector2D    EndSize;	     //파티클 에미터 마지막 사이즈
	Vector2D    Velocity;        //파티클 에미터의 이동 속도
	float		StartAlpha = 1.0f;
	float		EndAlpha = 1.0f;

	DWORD		CreatedTime = 0.0f;
	DWORD		m_LastTime = 0.0f;

	//---- Missile 폭발 전용 이미지 정보
	ID2D1Bitmap* m_MslExp_Img = NULL; //Missile 폭발 전용 이미지 
	int UVX = 1;
	int UVY = 1;
	float UVPer = 0.0f;
	float ImgRealWidth = 256.0f;
	float ImgRealHeight = 256.0f;
	D2D1_RECT_F UVRect;
	//---- Missile 폭발 전용 이미지 정보

	//----------절대좌표 이동 계산용 변수
	VecINT2D        m_ScreenHalf;       //화면 반 사이즈 저장용 변수
	Vector2D        m_CamPos = { 0.0f, 0.0f };           //현재 위치
	Vector2D        m_RenderPos;        //랜더링 위치
	//----------절대좌표 이동 계산용 변수

public:
	CPtcEmitter();
	~CPtcEmitter();

	void Emt_Update(DWORD a_LastTime, VecINT2D& a_Center, Vector2D& a_CamPos);
	void Emt_Render(ID2D1HwndRenderTarget* a_pd2dRTarget, 
					ID2D1SolidColorBrush* a_pBrush);

	//---- Missile 폭발 전용 이미지 정보
	void SetUV(int UV_X, int UV_Y)
	{
		UVX = UV_X;
		UVY = UV_Y;
	}
	// Index번째 UV사각형 좌표
	void CalculateUVRect(float per);

	void MslExpRender(ID2D1HwndRenderTarget* a_pd2dRTarget, 
					     ID2D1SolidColorBrush* a_pBrush); //미사일 폭발 전용 이펙트 렌더링 함수
	//---- Missile 폭발 전용 이미지 정보
};

