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
	if (GetFocus() == NULL) //현재 윈도우가 활성화되어 있지 않으면 주인공 컨트롤 제외
		return;
	   
	//------- 마우스 피킹이 있는 경우 처리
	if (m_bMoveOnOff == true)
	{
		static double a_CacStep;
		static Vector2D a_CacEndVec;

		// 가야할 방향에 시간 값을 곱해서 조금씩 이동하게 한다
		a_CacStep = a_DeltaTime * m_MoveSpeed; //이번에 한걸음 길이 (보폭)

		a_CacEndVec = m_TargetPos - m_CurPos;
		if (a_CacEndVec.Magnitude() <= a_CacStep)  //목표점까지의 거리보다 보폭이 크거나 같으면 도착으로 본다.
		{
			m_bMoveOnOff = false;
			//m_CurPos = m_TargetPos;  //<--이게 주인공의 좌표
		}
		else
		{
			m_DirVec = a_CacEndVec;
			m_DirVec.Normalize();
			m_CurPos = m_CurPos + (m_DirVec * a_CacStep);
		}
	}
	//------- 마우스 피킹이 있는 경우 처리

	//----------------- 동시키 처리 방법
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
	//----------------- 동시키 처리 방법 

	if (a_KDirVec.x == 0 && a_KDirVec.y == 0)
	{
		//나중에 숨쉬기 애니 상태 전환
	}
	else
	{
		m_bMoveOnOff = false;

		a_KDirVec.Normalize(); //길이가 1인 벡터
		m_CurPos = m_CurPos + (a_KDirVec * (a_DeltaTime * m_MoveSpeed));
	}

}

void CHero::Render_Unit(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush)
{
	//절대좌표를 스크린 좌표로 변환해서 그려야 한다.
	//m_RenderPos.x = m_CurPos.x - (int)m_CamPos.x + m_ScreenHalf.x;  //이론적
	//m_RenderPos.y = m_CurPos.y - (int)m_CamPos.y + m_ScreenHalf.y;
	m_RenderPos.x = m_ScreenHalf.x + (int)(m_CurPos.x - m_CamPos.x);  //이쪽 계산이 좀 더 부드럽게 움직인다.
	m_RenderPos.y = m_ScreenHalf.y + (int)(m_CurPos.y - m_CamPos.y);

	static D2D1_SIZE_F img_size;
	static Vector2D img_Half;
	static Vector2D Cac_Pos;
	img_size = m_SocketImg->GetSize();
	img_Half.x = img_size.width / 2.0f;
	img_Half.y = img_size.height / 2.0f;

	static float a_iXX = 0; //정수로 반올림 환산(그리기는 정수 좌표이기 때문에...)
	static float a_iYY = 0;
	a_iXX = m_RenderPos.x;  
	a_iYY = m_RenderPos.y;  
	AdjustRenderPosForShake(a_iXX);
	AdjustRenderPosForShake(a_iYY);

	//------ 캐릭터 Render
	a_pd2dRTarget->DrawBitmap(m_SocketImg,
		D2D1::RectF(a_iXX - img_Half.x,
			a_iYY - img_Half.y,
			a_iXX + img_Half.x,
			a_iYY + img_Half.y)
	);
	//------ 캐릭터 Render
}

void CHero::Destroy_Unit()
{
}

void CHero::Abs_Update(VecINT2D& a_ScHalf, Vector2D& a_CamPos)
{
	m_ScreenHalf = a_ScHalf; //Main DC 의 가로 세로 반사이즈
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

	m_TargetPos = a_TgPos;       // 목표점
}

void CHero::TakeDamage(float a_Damage)
{
}
