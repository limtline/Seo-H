#include "framework.h"
#include "SpawnPos.h"

SpawnPos::SpawnPos()
{

}

SpawnPos::~SpawnPos()
{

}

bool SpawnPos::Update_SpPos(float a_DeltaTime)
{
	static float OldDelay = 0.0f;
	OldDelay = m_SpDelay;
	m_SpDelay = m_SpDelay - a_DeltaTime;
	if (m_SpDelay <= 0.0f)
	{
		m_SpDelay = 0.0f;
		if (0.0f < OldDelay)
		{
			//메모리풀에서 몬스터 한마리 스폰시키고 몬스터쪽에 인덱스를 갖게 한다.
			return true;
		}
	}//if (m_SpDelay <= 0.0f)

	return false;
}

void SpawnPos::Render_SpPos(ID2D1HwndRenderTarget* a_pd2dRTarget, 
							ID2D1SolidColorBrush* a_pBrush, 
							VecINT2D& a_ScreenHalf, 
							Vector2D& a_CamPos)
{
	if (m_SocketImg == NULL)
		return;

	static Vector2D  m_RenderPos;
	//m_RenderPos.x = m_CurPos.x - (int)m_CamPos.x + m_ScreenHalf.x;  //이론적
	//m_RenderPos.y = m_CurPos.y - (int)m_CamPos.y + m_ScreenHalf.y;
	m_RenderPos.x = m_Pos.x + a_ScreenHalf.x - (int)a_CamPos.x;  //이쪽 계산이 좀 더 부드럽게 움직인다.
	m_RenderPos.y = m_Pos.y + a_ScreenHalf.y - (int)a_CamPos.y;

	static VecINT2D img_size;
	static VecINT2D img_Half;
	img_size.x = m_SocketImg->GetSize().width;
	img_size.y = m_SocketImg->GetSize().height;
	img_Half.x = img_size.x / 2.0f;
	img_Half.y = img_size.y / 2.0f;

	//------ 캐릭터 Render
	a_pd2dRTarget->DrawBitmap(m_SocketImg,
		D2D1::RectF(m_RenderPos.x - img_Half.x,
			m_RenderPos.y - img_Half.y,
			m_RenderPos.x + img_Half.x,
			m_RenderPos.y + img_Half.y)
	);
	//------ 캐릭터 Render
}
