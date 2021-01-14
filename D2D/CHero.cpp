#include "framework.h"
#include "CHero.h"
#include "GlobalValue.h"

CHero::CHero()
{
	m_MoveSpeed = 200.0;

	m_bMoveOnOff = false;
}

CHero::~CHero()
{
}

void CHero::Init_Unit(ID2D1Bitmap* (*D2DLoadBmp)(LPCWSTR FName))
{
	if (D2DLoadBmp != NULL)
	{
		m_SocketImg = D2DLoadBmp(_T(".\\RscTrunk\\Nanami\\front_Idle\\n001.png"));
	}
}

void CHero::Update_Unit(double a_DeltaTime)
{
	if (GetFocus() == NULL) //���� �����찡 Ȱ��ȭ�Ǿ� ���� ������ ���ΰ� ��Ʈ�� ����
		return;
	   
	//------- ���콺 ��ŷ�� �ִ� ��� ó��
	if (m_bMoveOnOff == true)
	{
		static double a_CacStep;
		static Vector2D a_CacEndVec;

		// ������ ���⿡ �ð� ���� ���ؼ� ���ݾ� �̵��ϰ� �Ѵ�
		a_CacStep = a_DeltaTime * m_MoveSpeed; //�̹��� �Ѱ��� ���� (����)

		a_CacEndVec = m_TargetPos - m_CurPos;
		if (a_CacEndVec.Magnitude() <= a_CacStep)  //��ǥ�������� �Ÿ����� ������ ũ�ų� ������ �������� ����.
		{
			m_bMoveOnOff = false;
			//m_CurPos = m_TargetPos;  //<--�̰� ���ΰ��� ��ǥ
		}
		else
		{
			m_DirVec = a_CacEndVec;
			m_DirVec.Normalize();
			m_CurPos = m_CurPos + (m_DirVec * a_CacStep);
		}
	}
	//------- ���콺 ��ŷ�� �ִ� ��� ó��

	//----------------- ����Ű ó�� ���
	static Vector2D a_KDirVec;

	a_KDirVec.x = 0.0f;
	a_KDirVec.y = 0.0f;

	if ((GetAsyncKeyState(VK_LEFT) & 0x8000) ||
		(GetAsyncKeyState('A') & 0x8000))
	{
		a_KDirVec.x -= 1.0f;
	}
	if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) ||
		(GetAsyncKeyState('D') & 0x8000))
	{
		a_KDirVec.x += 1.0f;
	}
	if ((GetAsyncKeyState(VK_UP) & 0x8000) ||
		(GetAsyncKeyState('W') & 0x8000))
	{
		a_KDirVec.y -= 1.0f;
	}
	if ((GetAsyncKeyState(VK_DOWN) & 0x8000) ||
		(GetAsyncKeyState('S') & 0x8000))
	{
		a_KDirVec.y += 1.0f;
	}
	//----------------- ����Ű ó�� ��� 

	if (a_KDirVec.x == 0 && a_KDirVec.y == 0)
	{
		//���߿� ������ �ִ� ���� ��ȯ
	}
	else
	{
		m_bMoveOnOff = false;

		a_KDirVec.Normalize(); //���̰� 1�� ����
		m_CurPos = m_CurPos + (a_KDirVec * (a_DeltaTime * m_MoveSpeed));
	}

}

void CHero::Render_Unit(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush)
{
	//������ǥ�� ��ũ�� ��ǥ�� ��ȯ�ؼ� �׷��� �Ѵ�.
	//m_RenderPos.x = m_CurPos.x - (int)m_CamPos.x + m_ScreenHalf.x;  //�̷���
	//m_RenderPos.y = m_CurPos.y - (int)m_CamPos.y + m_ScreenHalf.y;
	m_RenderPos.x = m_ScreenHalf.x + (int)(m_CurPos.x - m_CamPos.x);  //���� ����� �� �� �ε巴�� �����δ�.
	m_RenderPos.y = m_ScreenHalf.y + (int)(m_CurPos.y - m_CamPos.y);

	static D2D1_SIZE_F img_size;
	static Vector2D img_Half;
	static Vector2D Cac_Pos;
	img_size = m_SocketImg->GetSize();
	img_Half.x = img_size.width / 2.0f;
	img_Half.y = img_size.height / 2.0f;

	static float a_iXX = 0; //������ �ݿø� ȯ��(�׸���� ���� ��ǥ�̱� ������...)
	static float a_iYY = 0;
	a_iXX = m_RenderPos.x;  
	a_iYY = m_RenderPos.y;  
	AdjustRenderPosForShake(a_iXX);
	AdjustRenderPosForShake(a_iYY);

	//------ ĳ���� Render
	a_pd2dRTarget->DrawBitmap(m_SocketImg,
		D2D1::RectF(a_iXX - img_Half.x,
			a_iYY - img_Half.y,
			a_iXX + img_Half.x,
			a_iYY + img_Half.y)
	);
	//------ ĳ���� Render
}

void CHero::Destroy_Unit()
{
}

void CHero::Abs_Update(VecINT2D& a_ScHalf, Vector2D& a_CamPos)
{
	m_ScreenHalf = a_ScHalf; //Main DC �� ���� ���� �ݻ�����
	m_CamPos = a_CamPos;
}

void CHero::MsPicking(Vector2D a_TgPos)
{
	Vector2D a_CacVec = a_TgPos - m_CurPos;
	if (a_CacVec.Magnitude() < 4.0f)
	{
		return;
	}

	m_bMoveOnOff = true;

	m_TargetPos = a_TgPos;       // ��ǥ��
}

void CHero::TakeDamage(float a_Damage)
{
}
