#pragma once

#include "Vector2D.h"
#pragma comment (lib, "d2d1.lib")     //<---Direct2D ����ϱ�...
#include <d2d1.h>

#include <vector>
using namespace std;

class CBullet;		//<--���漱��

class CBullet_Mgr
{
public:
	vector<CBullet*> m_BullList;  //<--- �Ѿ� Ǯ ���

	static VecINT2D m_CenterPos;
	static Vector2D m_CamPos;

public:
	float m_PistolWidth = 30.0f;
	float m_PistolHeight = 5.0f;

	ID2D1Bitmap* m_MachineGunImg = NULL;
	float m_McGunWidth = 40.0f;
	float m_McGunHeight = 10.0f;

	ID2D1Bitmap* m_RocketImg = NULL;
	float m_RocketWidth = 100.0f;
	float m_RocketHeight = 32.0f;

public:
	void BLMgerInit(ID2D1Bitmap* (*D2DLoadBmp)(LPCWSTR FName) = NULL);
	void BLMgerUpdate(
		float a_DwdDeltaTime,					//<---������ ������ ��Ÿ Ÿ��
		DWORD a_DwdLastTime,                    //<---������ ������ ���� �ð�
		float(*GetDelta)(int WpType) = NULL,    //<---���⺰ �̵��� ���� DeltaTime ������
		bool(*CheckColl)(CBullet* a_RefBull, DWORD a_CurTime) = NULL,   //<---�浹 ó���� ���� �Լ� ������
		void(*GenSmokePtc)(Vector2D a_StartV, DWORD a_CurTime, Vector2D a_DirVec) = NULL   //<---�̻��� Smoke ������ �Լ� ������
	);//
	void BLMgerRender(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush);
	void BLMgerDestroy();

	void SpawnPistol(Vector2D a_StartV, Vector2D a_TargetV);
	void SpawnMachineGun(Vector2D a_StartV, Vector2D a_TargetV);
	void SpawnRocket(Vector2D a_StartV, Vector2D a_TargetV);
};

extern CBullet_Mgr g_Bullet_Mgr;
