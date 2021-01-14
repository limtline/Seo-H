#pragma once

#include "Vector2D.h"
#include <d2d1.h>

enum MonAIState
{
	MAI_Idle,          //������ ����
	MAI_Patrol,        //��Ʈ�� ����
	MAI_AggroTrace,    //�����κ��� ������ ������ �� ���� ����
	MAI_NormalTrace,   //�Ϲ� ���� ����
	MAI_ReturnPos,     //���� ������ �� ���ڸ��� ���ƿ��� ����
	MAI_Attack,        //���� ����
};

class SpawnPos
{
public:
	Vector2D     m_Pos = { 0.0f, 0.0f };
	float        m_SpDelay = 0.1f;
	ID2D1Bitmap* m_SocketImg;		  //�ִ� ����ó�� ����� ������

public:
	SpawnPos();
	~SpawnPos();

	bool Update_SpPos(float a_DeltaTime);
	void Render_SpPos(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush,
		VecINT2D& a_ScreenHalf, Vector2D& a_CamPos);
};

