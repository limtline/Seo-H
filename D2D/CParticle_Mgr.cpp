#include "framework.h"
#include "CParticle_Mgr.h"
#include "CPtcEmitter.h"
#include "GlobalValue.h"

ID2D1Bitmap* CParticle_Mgr::m_Ptc_MGun_Img = NULL;
ID2D1Bitmap* CParticle_Mgr::m_Smoke_Img = NULL;
ID2D1Bitmap* CParticle_Mgr::m_MissileExp_Img = NULL;

CParticle_Mgr::CParticle_Mgr()
{

}

CParticle_Mgr::~CParticle_Mgr()
{

}

void CParticle_Mgr::PtcMgrInit(ID2D1Bitmap* (*D2DLoadBmp)(LPCWSTR FName))
{
	if (CParticle_Mgr::m_Ptc_MGun_Img == NULL)
	{
		CParticle_Mgr::m_Ptc_MGun_Img = D2DLoadBmp(_T(".\\RscTrunk\\Effect\\Glow.png"));
	}

	if (CParticle_Mgr::m_Smoke_Img == NULL)
	{
		CParticle_Mgr::m_Smoke_Img = D2DLoadBmp(_T(".\\RscTrunk\\Effect\\Fog1.png"));
	}

	if (CParticle_Mgr::m_MissileExp_Img == NULL)
	{
		CParticle_Mgr::m_MissileExp_Img = 
			D2DLoadBmp(_T(".\\RscTrunk\\Effect\\ExplosionSheet.png"));
	}

	//----파티클 에미터 메모리풀로 미리 만들어 놓기    
	CPtcEmitter* a_BLNode = NULL;
	for (int aii = 0; aii < 500; aii++)
	{
		a_BLNode = new CPtcEmitter();
		m_ParticleList.push_back(a_BLNode);
	}
	//----파티클 에미터 메모리풀로 미리 만들어 놓기

	m_MissileExpList.reserve(500);
}

void CParticle_Mgr::PtcMgrUpdate(DWORD a_CurTime, VecINT2D& a_Center, Vector2D& a_CamPos)
{
	for (int aii = 0; aii < m_ParticleList.size(); aii++)
	{
		if (m_ParticleList[aii]->m_EmtActive == false)
			continue;

		m_ParticleList[aii]->Emt_Update(a_CurTime, a_Center, a_CamPos);
	}
}

void CParticle_Mgr::PtcMgrRender(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush)
{
	//----파티클 에미터 그려주기
	for (int aii = 0; aii < m_ParticleList.size(); aii++)
	{
		if (m_ParticleList[aii]->m_EmtActive == false)
			continue;

		m_ParticleList[aii]->Emt_Render(a_pd2dRTarget, a_pBrush);
	}//for (int aii = 0; aii < m_ParticleList.size(); aii++)
	//----파티클 에미터 그려주기
}

void CParticle_Mgr::PtcMgrDestroy()
{
	m_MissileExpList.clear();
	//파티클 에미터 메모리풀 제거하기...
	for (int aii = 0; aii < m_ParticleList.size(); aii++)
	{
		if (m_ParticleList[aii] != NULL)
		{
			delete m_ParticleList[aii];
			m_ParticleList[aii] = NULL;
		}
	}
	m_ParticleList.clear();
	//파티클 에미터 메모리풀 제거하기...
	if (CParticle_Mgr::m_Ptc_MGun_Img != NULL)
	{
		DeleteObject(CParticle_Mgr::m_Ptc_MGun_Img);//brush object 제거
		CParticle_Mgr::m_Ptc_MGun_Img = NULL;
	}

	if (CParticle_Mgr::m_Smoke_Img != NULL)
	{
		DeleteObject(CParticle_Mgr::m_Smoke_Img); //brush object 제거
		CParticle_Mgr::m_Smoke_Img = NULL;
	}

	if (CParticle_Mgr::m_MissileExp_Img != NULL)
	{
		DeleteObject(CParticle_Mgr::m_MissileExp_Img);
		CParticle_Mgr::m_MissileExp_Img = NULL;
	}
}

void CParticle_Mgr::MslExpMgrRender(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush)
{
	//---- Missile 폭발 전용 렌더링
 //순서를 위쪽으로 랜더링...
 //위쪽으로 쌓이도록 렌더링...  (이쪽 랜더링은 따로 관리하는 것 고려하자....)
	if (!m_MissileExpList.empty())
	{
		static vector<CPtcEmitter*>::iterator a_iter;
		for (a_iter = m_MissileExpList.begin(); a_iter != m_MissileExpList.end(); )
		{
			(*a_iter)->MslExpRender(a_pd2dRTarget, a_pBrush);

			if ((*a_iter)->m_EmtActive == false) //이러면 제거해 준다.
			{
				a_iter = m_MissileExpList.erase(a_iter);
			}
			else
			{
				a_iter++;
			}
		}
		//TCHAR a_tsr[128];
		//_stprintf(a_tsr, _T("-------------- %d : %d\n"), m_MissileExpList.size(), m_MissileExpList.capacity());
		//OutputDebugString(a_tsr);
	}//if (!m_MissileExpList.empty())
}

void CParticle_Mgr::Explosion_MGun(Vector2D a_StartV, DWORD a_CurTime)
{
	static CPtcEmitter* a_PtcEmtMchGun = NULL;
	static int aii = 0;
	for (int i = 0; i < 10; i++)
	{
		//------------------------------------- Find ParticleEmitter
		a_PtcEmtMchGun = NULL;
		aii = 0;
		for (aii = 0; aii < m_ParticleList.size(); aii++)
		{
			if (m_ParticleList[aii]->m_EmtActive == false)
			{
				a_PtcEmtMchGun = m_ParticleList[aii];
				break;
			}
		}

		if (a_PtcEmtMchGun == NULL)
		{
			a_PtcEmtMchGun = new CPtcEmitter();
			m_ParticleList.push_back(a_PtcEmtMchGun);
		}

		if (a_PtcEmtMchGun != NULL)
		{
			a_PtcEmtMchGun->m_PtcImage = CParticle_Mgr::m_Ptc_MGun_Img; //머신건 파티클 이미지 대입
			a_PtcEmtMchGun->m_MslExp_Img = NULL;
			a_PtcEmtMchGun->SpawnPosition = a_StartV;
			a_PtcEmtMchGun->StartSize = Vector2D(3.0f, 1.0f);
			a_PtcEmtMchGun->EndSize = Vector2D(RandInt(5, 10), RandInt(2, 4));
			a_PtcEmtMchGun->LifeTime = RandInt(5.0f, 9.0f) * 20.0f; //(RandInt(5.0f, 9.0f) / 50.0f) * 1000.0f;
			a_PtcEmtMchGun->Velocity = Vector2D::GetNormalFromRotation(RandInt(0, 360)) * RandInt(20.0f, 60.0f); //각도를 벡터로 환산
			//a_PtcEmtMchGun->ParticleColor = Direct2DHelper::RGBA(1.0f, 0.0f, 0.0f);
			a_PtcEmtMchGun->m_DirRot = a_PtcEmtMchGun->Velocity.GetAngle(); //<--- 속도 방향으로 회전 하도록...	
			a_PtcEmtMchGun->StartAlpha = 1.0f;
			a_PtcEmtMchGun->EndAlpha = 1.0f;
			a_PtcEmtMchGun->CreatedTime = a_CurTime;  //timeGetTime() / 1000.0f;
			a_PtcEmtMchGun->m_EmtActive = true;
		}
		//------------------------------------- Find ParticleEmitter
	}//for (int i = 0; i < 10; i++)
}

void CParticle_Mgr::Explosion_Missile(Vector2D a_StartV, DWORD a_CurTime, float a_RocketRot)
{
	static CPtcEmitter* a_PtcEmtMsl = NULL;
	static int aa = 0;
	a_PtcEmtMsl = NULL;
	aa = 0;
	for (aa = 0; aa < m_ParticleList.size(); aa++)
	{
		if (m_ParticleList[aa]->m_EmtActive == false)
		{
			a_PtcEmtMsl = m_ParticleList[aa];
			break;
		}
	}

	if (a_PtcEmtMsl == NULL)
	{
		a_PtcEmtMsl = new CPtcEmitter();
		m_ParticleList.push_back(a_PtcEmtMsl);
	}

	if (a_PtcEmtMsl != NULL)
	{
		a_PtcEmtMsl->m_PtcImage = NULL; //머신건 파티클 이미지 대입
		a_PtcEmtMsl->m_MslExp_Img = CParticle_Mgr::m_MissileExp_Img; //미사일 폭발 이미지 대입
		a_PtcEmtMsl->SetUV(4, 4);
		a_PtcEmtMsl->ImgRealWidth = a_PtcEmtMsl->m_MslExp_Img->GetSize().width;
		a_PtcEmtMsl->ImgRealHeight = a_PtcEmtMsl->m_MslExp_Img->GetSize().height;
		a_PtcEmtMsl->SpawnPosition = a_StartV;
		//a_PtcEmtMsl->StartSize = Vector2D(10.0f, 10.0f);
		//a_PtcEmtMsl->EndSize = Vector2D(RandInt(7, 15), RandInt(2, 3));
		a_PtcEmtMsl->LifeTime = 1.0f;  //<--- 1초 뒤에 자동 삭제 되도록 한다.
		//a_PtcEmtMsl->Velocity = Vector2D::GetNormalFromRotation(a_RocketRot + RandInt(-150, 150) / 10.0f) * RandInt(50.0f, 500.0f); //각도를 벡터로 환산	
		//a_PtcEmtMsl->m_DirRot = a_PtcEmtMsl->Velocity.GetAngle(); //<--- 속도 방향으로 회전 하도록...	
		//a_PtcEmtMsl->StartAlpha = 1.0f;
		//a_PtcEmtMsl->EndAlpha = 1.0f;
		a_PtcEmtMsl->CreatedTime = a_CurTime;  //timeGetTime() / 1000.0f;
		a_PtcEmtMsl->m_EmtActive = true;

		m_MissileExpList.push_back(a_PtcEmtMsl);
	}

	static CPtcEmitter* a_PtcEmtMchGun = NULL;
	static int aii = 0;
	a_PtcEmtMchGun = NULL;
	aii = 0;
	for (int i = 0; i < 10; i++)
	{
		//------------------------------------- Find ParticleEmitter
		a_PtcEmtMchGun = NULL;
		aii = 0;
		for (aii = 0; aii < m_ParticleList.size(); aii++)
		{
			if (m_ParticleList[aii]->m_EmtActive == false)
			{
				a_PtcEmtMchGun = m_ParticleList[aii];
				break;
			}
		}

		if (a_PtcEmtMchGun == NULL)
		{
			a_PtcEmtMchGun = new CPtcEmitter();
			m_ParticleList.push_back(a_PtcEmtMchGun);
		}

		if (a_PtcEmtMchGun != NULL)
		{
			a_PtcEmtMchGun->m_PtcImage = CParticle_Mgr::m_Ptc_MGun_Img; //머신건 파티클 이미지 대입
			a_PtcEmtMchGun->m_MslExp_Img = NULL;
			a_PtcEmtMchGun->SpawnPosition = a_StartV;
			a_PtcEmtMchGun->StartSize = Vector2D(10.0f, 10.0f);
			a_PtcEmtMchGun->EndSize = Vector2D(RandInt(7, 15), RandInt(2, 3));
			a_PtcEmtMchGun->LifeTime = RandInt(5.0f, 9.0f) * 100.0f;
			a_PtcEmtMchGun->Velocity = Vector2D::GetNormalFromRotation(a_RocketRot + RandInt(-150, 150) / 10.0f) * RandInt(50.0f, 500.0f); //각도를 벡터로 환산																															   
																																						 //a_PtcEmtMchGun->ParticleColor = Direct2DHelper::RGBA(1.0f, 0.0f, 0.0f);
			a_PtcEmtMchGun->m_DirRot = a_PtcEmtMchGun->Velocity.GetAngle(); //<--- 속도 방향으로 회전 하도록...	
			a_PtcEmtMchGun->StartAlpha = 1.0f;
			a_PtcEmtMchGun->EndAlpha = 1.0f;
			a_PtcEmtMchGun->CreatedTime = a_CurTime;  //timeGetTime() / 1000.0f;
			a_PtcEmtMchGun->m_EmtActive = true;
		}
		//------------------------------------- Find ParticleEmitter
	}//for (int i = 0; i < 10; i++)
}

void CParticle_Mgr::Explosion_Die(Vector2D a_StartV, DWORD a_CurTime)
{
	static CPtcEmitter* a_PtcEmtMchGun = NULL;
	static int aii = 0;
	a_PtcEmtMchGun = NULL;
	aii = 0;
	for (int i = 0; i < 40; i++)
	{
		//------------------------------------- Find ParticleEmitter
		a_PtcEmtMchGun = NULL;
		aii = 0;
		for (aii = 0; aii < m_ParticleList.size(); aii++)
		{
			if (m_ParticleList[aii]->m_EmtActive == false)
			{
				a_PtcEmtMchGun = m_ParticleList[aii];
				break;
			}
		}

		if (a_PtcEmtMchGun == NULL)
		{
			a_PtcEmtMchGun = new CPtcEmitter();
			m_ParticleList.push_back(a_PtcEmtMchGun);
		}

		if (a_PtcEmtMchGun != NULL)
		{
			a_PtcEmtMchGun->m_PtcImage = CParticle_Mgr::m_Ptc_MGun_Img; //머신건 파티클 이미지 대입
			a_PtcEmtMchGun->m_MslExp_Img = NULL;
			a_PtcEmtMchGun->SpawnPosition = a_StartV;
			a_PtcEmtMchGun->StartSize = Vector2D(10.0f, 10.0f);
			a_PtcEmtMchGun->EndSize = Vector2D(RandInt(20, 50), RandInt(2, 5));
			a_PtcEmtMchGun->LifeTime = RandInt(5.0f, 9.0f) * 100.0f;
			a_PtcEmtMchGun->Velocity = Vector2D::GetNormalFromRotation(i * 9) * RandInt(50.0f, 300.0f);
			//a_PtcEmtMchGun->ParticleColor = Direct2DHelper::RGBA(1.0f, 0.0f, 0.0f);
			a_PtcEmtMchGun->m_DirRot = a_PtcEmtMchGun->Velocity.GetAngle(); //<--- 속도 방향으로 회전 하도록...	
			a_PtcEmtMchGun->StartAlpha = 1.0f;
			a_PtcEmtMchGun->EndAlpha = 1.0f;
			a_PtcEmtMchGun->CreatedTime = a_CurTime;  //timeGetTime() / 1000.0f;
			a_PtcEmtMchGun->m_EmtActive = true;
		}
		//------------------------------------- Find ParticleEmitter
	}
}

void CParticle_Mgr::GenSmokeParticle(Vector2D a_StartV, DWORD a_CurTime, Vector2D a_DirVec)
{
	static CPtcEmitter* a_PtcEmtMchGun = NULL;
	static int aii = 0;
	a_PtcEmtMchGun = NULL;
	aii = 0;
	for (aii = 0; aii < g_Particle_Mgr.m_ParticleList.size(); aii++)
	{
		if (g_Particle_Mgr.m_ParticleList[aii]->m_EmtActive == false)
		{
			a_PtcEmtMchGun = g_Particle_Mgr.m_ParticleList[aii];
			break;
		}
	}

	if (a_PtcEmtMchGun == NULL)
	{
		a_PtcEmtMchGun = new CPtcEmitter();
		g_Particle_Mgr.m_ParticleList.push_back(a_PtcEmtMchGun);
	}

	if (a_PtcEmtMchGun != NULL)
	{
		a_PtcEmtMchGun->m_PtcImage = CParticle_Mgr::m_Smoke_Img; //머신건 파티클 이미지 대입
		a_PtcEmtMchGun->m_MslExp_Img = NULL;
		a_PtcEmtMchGun->SpawnPosition = a_StartV;
		a_PtcEmtMchGun->StartSize = Vector2D(10.0f, 10.0f);
		a_PtcEmtMchGun->EndSize = Vector2D(RandInt(30, 50), RandInt(30, 50));
		a_PtcEmtMchGun->LifeTime = 1.0F * 1000.0f;
	
		a_PtcEmtMchGun->Velocity = Vector2D(-RandInt(30, 60), RandInt(-20, 30));
		a_PtcEmtMchGun->StartAlpha = 1.0f;
		a_PtcEmtMchGun->EndAlpha = 0.0f;
		a_PtcEmtMchGun->CreatedTime = a_CurTime;
		a_PtcEmtMchGun->m_EmtActive = true;
	}
}

CParticle_Mgr g_Particle_Mgr;