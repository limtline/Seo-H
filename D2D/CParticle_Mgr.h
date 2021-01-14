#pragma once

#pragma comment (lib, "d2d1.lib")     //<---Direct2D 사용하기...
#include <d2d1.h>
#include "Vector2D.h"
#include <vector>

using namespace std;

class CPtcEmitter;  //전방선언

class CParticle_Mgr
{
public:
	vector<CPtcEmitter*> m_ParticleList;    //<--- 파티클 에미터 풀 방식
	vector<CPtcEmitter*> m_MissileExpList;  //<--- 미사일 폭파 메모리 풀은 따로 관리하는 것도 고려한다.

public:
	static ID2D1Bitmap* m_Ptc_MGun_Img;
	static ID2D1Bitmap* m_Smoke_Img;
	static ID2D1Bitmap* m_MissileExp_Img;

public:
	CParticle_Mgr();
	~CParticle_Mgr();

public:
	void PtcMgrInit(ID2D1Bitmap* (*D2DLoadBmp)(LPCWSTR FName) = NULL);
	void PtcMgrUpdate(DWORD a_CurTime, VecINT2D& a_Center, Vector2D& a_CamPos);
	void PtcMgrRender(ID2D1HwndRenderTarget* a_pd2dRTarget, 
					  ID2D1SolidColorBrush* a_pBrush);
	void PtcMgrDestroy();
	//---- Missile 폭발 전용 렌더링
	void MslExpMgrRender(ID2D1HwndRenderTarget* a_pd2dRTarget, 
							ID2D1SolidColorBrush* a_pBrush);
	//---- Missile 폭발 전용 렌더링

	void Explosion_MGun(Vector2D a_StartV, DWORD a_CurTime); 
	void Explosion_Missile(Vector2D a_StartV, DWORD a_CurTime, float a_RocketRot = 0.0f);
	void Explosion_Die(Vector2D a_StartV, DWORD a_CurTime);
	void static GenSmokeParticle(Vector2D a_StartV, DWORD a_CurTime, 
								 Vector2D a_DirVec); //연기 파티클 만들기...
};

extern CParticle_Mgr g_Particle_Mgr;