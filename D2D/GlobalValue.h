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

//---------ī�޶� ����...
struct CameraShaker
{
	bool Enabled = false;
	double EndTime; // �����½ð�
	double LifeTime;
	float Power;
};

extern CameraShaker CameraShake;
extern DWORD        g_vLastTime;
void AdjustRenderPosForShake(float& Pos);  //���÷��� ȣ��
void ShakeScreen(float Power, float Duration); //���� ���� �Լ�
//UI �� ���� ��� ������Ʈ�� �����ؼ� ���� �־�� �Ѵ�.
//---------ī�޶� ����...