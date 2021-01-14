#include "framework.h"
#include "CBullet_Mgr.h"
#include "CBullet.h"
#include "GlobalValue.h"

VecINT2D CBullet_Mgr::m_CenterPos = { 0, 0 };    //정적변수 초기화
Vector2D CBullet_Mgr::m_CamPos = { 0.0f, 0.0f }; //정적변수 초기화

void CBullet_Mgr::BLMgerInit(ID2D1Bitmap* (*D2DLoadBmp)(LPCWSTR FName))
{
	//---------------총알 이미지 로딩
	if (m_MachineGunImg == NULL)
	{
		m_MachineGunImg = D2DLoadBmp(_T(".\\RscTrunk\\Effect\\HeavyMachineGunBullet.png"));
		m_McGunWidth = 40.0f; //m_MachineGunImg->GetSize().width;
		m_McGunHeight = 10.0f; //m_MachineGunImg->GetSize().height;
		//m_Radius = 25.0f;    //총알 충돌 반경
	}

	if (m_RocketImg == NULL)
	{
		m_RocketImg = D2DLoadBmp(_T(".\\RscTrunk\\Weapon\\Missile.png"));
		m_RocketWidth = 100.0f;
		m_RocketHeight = 32.0f;
		//m_Radius = 40.0f; //총알 충돌 반경
	}
	//---------------총알 이미지 로딩

	//----총알 메모리풀로 미리 만들어 놓기    
	CBullet* a_BLNode = NULL;
	for (int aii = 0; aii < 200; aii++)
	{
		a_BLNode = new CBullet();
		m_BullList.push_back(a_BLNode);
	}
	//----총알 메모리풀로 미리 만들어 놓기
}

void CBullet_Mgr::BLMgerUpdate(float a_DwdDeltaTime, 
	DWORD a_DwdLastTime, 
	float(*GetDelta)(int WpType), 
	bool(*CheckColl)(CBullet* a_RefBull, DWORD a_CurTime), 
	void(*GenSmokePtc)(Vector2D a_StartV, DWORD a_CurTime, Vector2D a_DirVec))
{
	//------------------ 모든 총알 이동관련 업데이트 처리
	static bool a_IsColl = false;
	for (int aii = 0; aii < m_BullList.size(); aii++)
	{
		if (m_BullList[aii]->m_BLActive == false)
			continue;

		m_BullList[aii]->BulletUpdate(a_DwdDeltaTime, a_DwdLastTime,
				GetDelta(m_BullList[aii]->WeaponType), GenSmokePtc);

		if (CheckColl == NULL)
			continue;

		////총알이 몬스터에 맞았으면 제거해 준다. 몬스터 제거 총알 제거
		a_IsColl = CheckColl(m_BullList[aii], a_DwdLastTime);  //충돌체크 함수 포인터 
		if (a_IsColl == true)
		{
			m_BullList[aii]->m_BLActive = false;  //총알 제거
		}
	}
	//------------------ 모든 총알 이동관련 업데이트 처리
}

void CBullet_Mgr::BLMgerRender(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush)
{
	//----총알 그려주기
	for (int aii = 0; aii < m_BullList.size(); aii++)
	{
		if (m_BullList[aii]->m_BLActive == false)
			continue;

		m_BullList[aii]->BulletRender(a_pd2dRTarget, a_pBrush);

	}//for (m_Biter = m_StdList.begin(); m_Biter != m_StdList.end(); m_Biter++)
	//----총알 그려주기
}

void CBullet_Mgr::BLMgerDestroy()
{
	//총알 메모리풀 제거하기... 
	for (int aii = 0; aii < m_BullList.size(); aii++)
	{
		if (m_BullList[aii] != NULL)
		{
			delete m_BullList[aii];
			m_BullList[aii] = NULL;
		}
	}
	m_BullList.clear();
	//총알 메모리풀 제거하기...

	if (m_MachineGunImg != NULL)
	{
		m_MachineGunImg->Release();
		m_MachineGunImg = NULL;
	}

	if (m_RocketImg != NULL)
	{
		m_RocketImg->Release();
		m_RocketImg = NULL;
	}
}

void CBullet_Mgr::SpawnPistol(Vector2D a_StartV, Vector2D a_TargetV)
{
	static Vector2D SpawnLocation;
	static float Ang = 0.0f;

	//-------------- void Weapon_Pistol::HandleShoot
	static Vector2D a_CacDirVec;
	a_CacDirVec = a_TargetV - a_StartV;
	a_CacDirVec.Normalize();

	SpawnLocation = a_StartV + a_CacDirVec * 40.0f; //50.0; //* 20.0f;
	Ang = a_CacDirVec.GetAngle(); //벡터를 각도로 환산
	//a_CacDirVec = Vector2D::GetNormalFromRotation(Ang); //각도를 벡터로 환산
	//-------------- void Weapon_Pistol::HandleShoot

	for (int aii = 0; aii < m_BullList.size(); aii++)
	{
		if (m_BullList[aii]->m_BLActive == false)
		{
			m_BullList[aii]->WeaponType = EWeaponSlots::PISTOL;
			m_BullList[aii]->m_TexImgWidth = m_PistolWidth;
			m_BullList[aii]->m_TexImgHeight = m_PistolHeight;
			m_BullList[aii]->m_Radius = 10.0f; //총알 충돌 반경
			m_BullList[aii]->m_MoveSpeed = 1000.0f;

			m_BullList[aii]->m_CurPos.x = SpawnLocation.x;
			m_BullList[aii]->m_CurPos.y = SpawnLocation.y;
			m_BullList[aii]->m_RenderPos.x = SpawnLocation.x;
			m_BullList[aii]->m_RenderPos.y = SpawnLocation.y;

			m_BullList[aii]->m_DirVec = a_CacDirVec;
			m_BullList[aii]->m_DirRot = Ang;
			m_BullList[aii]->m_BLActive = true;
			m_BullList[aii]->m_Duration = 2.0f;

			return;
		}//if
	}//for

	static CBullet* a_BNode = NULL;
	a_BNode = new CBullet();

	a_BNode->WeaponType = EWeaponSlots::PISTOL;
	a_BNode->m_TexImgWidth = m_PistolWidth;
	a_BNode->m_TexImgHeight = m_PistolHeight;
	a_BNode->m_Radius = 10.0f; //총알 충돌 반경
	a_BNode->m_MoveSpeed = 1000.0f;

	a_BNode->m_CurPos.x = SpawnLocation.x;
	a_BNode->m_CurPos.y = SpawnLocation.y;
	a_BNode->m_RenderPos.x = SpawnLocation.x;
	a_BNode->m_RenderPos.y = SpawnLocation.y;

	a_BNode->m_DirVec = a_CacDirVec;
	a_BNode->m_DirRot = Ang;
	a_BNode->m_BLActive = true;
	a_BNode->m_Duration = 2.0f;

	m_BullList.push_back(a_BNode);
}

void CBullet_Mgr::SpawnMachineGun(Vector2D a_StartV, Vector2D a_TargetV)
{
	static Vector2D SpawnLocation;
	static float Ang = 0.0f;

	//-------------- void Weapon_HeavyMachineGun::HandleShoot
	static Vector2D a_CacDirVec;
	a_CacDirVec = a_TargetV - a_StartV;
	a_CacDirVec.Normalize();

	SpawnLocation = a_StartV + a_CacDirVec * 50.0; 

	Ang = a_CacDirVec.GetAngle(); //벡터를 각도로 환산
	Ang += RandInt(-1000, 1000) / 500.0f; // +- 2도
	a_CacDirVec = Vector2D::GetNormalFromRotation(Ang); //각도를 벡터로 환산
	a_CacDirVec.Normalize();

	SpawnLocation = SpawnLocation + Vector2D(RandInt(-5, 5), RandInt(-5, 5));
	//-------------- void Weapon_HeavyMachineGun::HandleShoot

	for (int aii = 0; aii < m_BullList.size(); aii++)
	{
		if (m_BullList[aii]->m_BLActive == false)
		{
			m_BullList[aii]->WeaponType = EWeaponSlots::HEAVY_MACHINE_GUN;
			m_BullList[aii]->m_BulletImg = m_MachineGunImg;
			m_BullList[aii]->m_TexImgWidth = m_McGunWidth;
			m_BullList[aii]->m_TexImgHeight = m_McGunHeight;
			m_BullList[aii]->m_Radius = 25.0f; //총알 충동 반경
			m_BullList[aii]->m_MoveSpeed = 1700.0f;

			m_BullList[aii]->m_CurPos.x = SpawnLocation.x;
			m_BullList[aii]->m_CurPos.y = SpawnLocation.y;
			m_BullList[aii]->m_RenderPos.x = SpawnLocation.x;
			m_BullList[aii]->m_RenderPos.y = SpawnLocation.y;

			m_BullList[aii]->m_DirVec = a_CacDirVec;
			m_BullList[aii]->m_DirRot = Ang;
			m_BullList[aii]->m_BLActive = true;
			m_BullList[aii]->m_Duration = 2.0f;
			//m_BullList[aii]->m_BL_Type = a_BLTP;

			return;
		} //if
	} //for

	static CBullet* m_BLNode = NULL;
	m_BLNode = new CBullet();

	m_BLNode->WeaponType = EWeaponSlots::HEAVY_MACHINE_GUN;
	m_BLNode->m_BulletImg = m_MachineGunImg;
	m_BLNode->m_TexImgWidth = m_McGunWidth;
	m_BLNode->m_TexImgHeight = m_McGunHeight;
	m_BLNode->m_Radius = 25.0f; //총알 충동 반경
	m_BLNode->m_MoveSpeed = 1700.0f;

	m_BLNode->m_CurPos.x = SpawnLocation.x;
	m_BLNode->m_CurPos.y = SpawnLocation.y;
	m_BLNode->m_RenderPos.x = SpawnLocation.x;
	m_BLNode->m_RenderPos.y = SpawnLocation.y;

	m_BLNode->m_DirVec = a_CacDirVec;
	m_BLNode->m_DirRot = Ang;
	m_BLNode->m_BLActive = true;
	m_BLNode->m_Duration = 2.0f;

	m_BullList.push_back(m_BLNode);
}

void CBullet_Mgr::SpawnRocket(Vector2D a_StartV, Vector2D a_TargetV)
{
	static Vector2D SpawnLocation;
	static float Ang = 0.0f;

	//-------------- void Weapon_RocketLauncher::HandleShoot
	static Vector2D a_CacDirVec;
	a_CacDirVec = a_TargetV - a_StartV;
	a_CacDirVec.Normalize();

	SpawnLocation = a_StartV + a_CacDirVec * 50.0; //* 20.0f;
	Ang = a_CacDirVec.GetAngle(); //벡터를 각도로 환산
	SpawnLocation = SpawnLocation + Vector2D(RandInt(-5, 5), RandInt(-5, 5));
	//-------------- void Weapon_RocketLauncher::HandleShoot

	for (int aii = 0; aii < m_BullList.size(); aii++)
	{
		if (m_BullList[aii]->m_BLActive == false)
		{
			m_BullList[aii]->WeaponType = EWeaponSlots::ROCKET_LAUNCHER;
			m_BullList[aii]->m_BulletImg = m_RocketImg;
			m_BullList[aii]->m_TexImgWidth = m_RocketWidth;
			m_BullList[aii]->m_TexImgHeight = m_RocketHeight;
			m_BullList[aii]->m_Radius = 40.0f; //총알 충돌 반경
			m_BullList[aii]->m_MoveSpeed = 1700.0f;

			m_BullList[aii]->m_CurPos.x = SpawnLocation.x;
			m_BullList[aii]->m_CurPos.y = SpawnLocation.y;
			m_BullList[aii]->m_RenderPos.x = SpawnLocation.x;
			m_BullList[aii]->m_RenderPos.y = SpawnLocation.y;

			m_BullList[aii]->m_DirVec = a_CacDirVec;
			m_BullList[aii]->m_DirRot = Ang;
			m_BullList[aii]->m_BLActive = true;
			m_BullList[aii]->m_Duration = 2.0f;

			return;
		}//if
	}//for

	static CBullet* m_BLNode = NULL;
	m_BLNode = new CBullet();

	m_BLNode->WeaponType = EWeaponSlots::ROCKET_LAUNCHER;
	m_BLNode->m_BulletImg = m_RocketImg;
	m_BLNode->m_TexImgWidth = m_RocketWidth;
	m_BLNode->m_TexImgHeight = m_RocketHeight;
	m_BLNode->m_Radius = 40.0f; //총알 충돌 반경
	m_BLNode->m_MoveSpeed = 1700.0f;

	m_BLNode->m_CurPos.x = SpawnLocation.x;
	m_BLNode->m_CurPos.y = SpawnLocation.y;
	m_BLNode->m_RenderPos.x = SpawnLocation.x;
	m_BLNode->m_RenderPos.y = SpawnLocation.y;

	m_BLNode->m_DirVec = a_CacDirVec;
	m_BLNode->m_DirRot = Ang;
	m_BLNode->m_BLActive = true;
	m_BLNode->m_Duration = 2.0f;

	m_BullList.push_back(m_BLNode);
}

CBullet_Mgr g_Bullet_Mgr;