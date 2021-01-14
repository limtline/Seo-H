#include "framework.h"
#include "CBullet.h"
#include "CBullet_Mgr.h"
#include "GlobalValue.h"

CBullet::CBullet()
{
	m_BLActive = false;

	m_Duration = 4.0f;
	m_DirRot   = 0.0f; //�Ѿ��� ���ư��� ���Ⱒ��
	m_MoveSpeed = 1700.0f;  //2000.0f;   //1700�ȼ� / 1��
	m_Radius   = 25.0f; //�Ѿ� �浿 �ݰ�
}

CBullet::~CBullet()
{
}

bool CBullet::BulletUpdate(
	float a_DwdDeltaTime, 
	DWORD a_DwdLastTime, 
	float a_WpDeltaTime, 
	void(*GenSmokePtc)(Vector2D a_StartV, DWORD a_CurTime, Vector2D a_DirVec))
{
	m_Duration = m_Duration - a_DwdDeltaTime;
	if (m_Duration < 0.0f)
	{
		m_BLActive = false;
		return false;  //���� �ð��ڿ� ������ �ش�.
	}

	if (WeaponType == EWeaponSlots::ROCKET_LAUNCHER)
	{
		m_SmokeTime = m_SmokeTime - a_WpDeltaTime;  //a_DwdDeltaTime; 
		if (m_SmokeTime <= 0.0f)
		{
			//�ֱ������� ����ũ ����
			if (GenSmokePtc != NULL)
			{
				GenSmokePtc(m_CurPos, a_DwdLastTime, m_DirVec);
			}
			m_SmokeTime = 0.01f; //�� �ֱ�� 
		}
	}//if (WeaponType == EWeaponSlots::ROCKET_LAUNCHER)

	m_CurPos = m_CurPos + (m_DirVec * (a_DwdDeltaTime * m_MoveSpeed)); //�Ѿ� �̵�ó��
	m_RenderPos = m_RenderPos + (m_DirVec * (a_WpDeltaTime * m_MoveSpeed)); //�Ѿ� �̵�ó��

	return true;
}

void CBullet::BulletRender(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush)
{
	static float a_iXX = 0; //������ �ݿø� ȯ��(�׸���� ���� ��ǥ�̱� ������...)
	static float a_iYY = 0;
	a_iXX = (float)m_RenderPos.x + CBullet_Mgr::m_CenterPos.x - (int)CBullet_Mgr::m_CamPos.x;
	a_iYY = (float)m_RenderPos.y + CBullet_Mgr::m_CenterPos.y - (int)CBullet_Mgr::m_CamPos.y;
	AdjustRenderPosForShake(a_iXX);
	AdjustRenderPosForShake(a_iYY);

	static int x = 0, y = 0;
	static float Rotation = 0.0f, Alpha = 1.0f;

	x = a_iXX - m_TexImgWidth * 0.5f;
	y = a_iYY - m_TexImgHeight * 0.5f;
	Rotation = m_DirVec.GetAngle();
	Alpha = 1.0f;

	static D2D1_POINT_2F center_pos = { 0.0f, 0.0f };
	center_pos.x = a_iXX;
	center_pos.y = a_iYY;

	a_pd2dRTarget->SetTransform(D2D1::Matrix3x2F::Rotation(Rotation, center_pos));

	if (WeaponType == EWeaponSlots::PISTOL)
	{
		a_pBrush->SetColor(D2D1::ColorF(0x00ffff));
		a_pd2dRTarget->FillRectangle(D2D1::RectF(x, y,
			x + m_TexImgWidth, y + m_TexImgHeight), a_pBrush);
		a_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	}//if (WeaponType == EWeaponSlots::PISTOL)
	else
	{
		a_pd2dRTarget->DrawBitmap(m_BulletImg,
			D2D1::RectF(x, y, x + m_TexImgWidth, y + m_TexImgHeight),
			Alpha, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
	}

	a_pd2dRTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}
