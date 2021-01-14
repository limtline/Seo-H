#pragma once

enum EWeaponSlots
{
	PISTOL = 0,
	HEAVY_MACHINE_GUN = 1,
	ROCKET_LAUNCHER = 2,
	ENEMY_CHASER = 3,
};

extern EWeaponSlots WeaponSlot;

void  GDeltaUpdate(float a_DeltaTime);
float GetMvDelta(int WpType);
int   RandInt(int min, int max);
int	  ChangeWeapon();

//---------카메라 흔들기...
struct CameraShaker
{
	bool Enabled = false;
	double EndTime; // 끝나는시간
	double LifeTime;
	float Power;
};

extern CameraShaker CameraShake;
extern DWORD        g_vLastTime;
void AdjustRenderPosForShake(float& Pos);  //매플레임 호출
void ShakeScreen(float Power, float Duration); //흔들기 시작 함수
//UI 빼 놓고 모든 오브젝트를 포함해서 흔들어 주어야 한다.
//---------카메라 흔들기...