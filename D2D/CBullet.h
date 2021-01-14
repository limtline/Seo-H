#pragma once

#include "Vector2D.h"
#pragma comment (lib, "d2d1.lib")     //<---Direct2D ����ϱ�...
#include <d2d1.h>

class CBullet
{
public:
	enum EWeaponSlots WeaponType;

	bool	 m_BLActive;      //�Ѿ��� ��Ƽ�� ����
	ID2D1Bitmap* m_BulletImg = NULL; //�Ѿ� �̹���
	float m_TexImgWidth = 40.0f;	 //�Ѿ� �̹��� ���� ������
	float m_TexImgHeight = 10.0f;	 //�Ѿ� �̹��� ���� ������

	float		m_Duration;   //�Ѿ��� ������ Ÿ��(�Ѿ��� ���ʵ��� ���ư��� �� �������� �ð�)
	Vector2D	m_CurPos;     //�Ѿ��� ���� ��ġ
	Vector2D    m_RenderPos;  //�Ѿ� ������ ������ ���� ��ġ ���� ����
	Vector2D	m_DirVec;     //�Ѿ��� ���ư��� ���⺤��
	float		m_DirRot = 0.0f; //�Ѿ��� ���ư��� ���Ⱒ��
	float		m_MoveSpeed;  //�Ѿ��� �̵� �ӵ�
	float       m_Radius;     //�Ѿ� �浹 �ݰ�
	Vector2D	a_CacBVec;    //���� ����

	float       m_SmokeTime = 0.0f;

public:
	CBullet();
	~CBullet();

	bool BulletUpdate(
		float a_DwdDeltaTime, //<---������ ������ DeltaTime 
		DWORD a_DwdLastTime,  //<---������ ���� �ð�
		float a_WpDeltaTime,  //<---���⺰ �̵��� ���� DeltaTime 
		void(*GenSmokePtc)(Vector2D a_StartV, DWORD a_CurTime, Vector2D a_DirVec) = NULL   //<---�̻��� Smoke ������ �Լ� ������
	);
	void BulletRender(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush);
};

