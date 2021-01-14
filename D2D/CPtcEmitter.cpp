#include "framework.h"
#include "CPtcEmitter.h"
#include "GlobalValue.h"

CPtcEmitter::CPtcEmitter()
{
	m_EmtActive = false;
}

CPtcEmitter::~CPtcEmitter()
{

}

void CPtcEmitter::Emt_Update(DWORD a_LastTime, VecINT2D& a_Center, Vector2D& a_CamPos)
{
	m_ScreenHalf.x = a_Center.x; //Main DC 의 가로 세로 반사이즈
	m_ScreenHalf.y = a_Center.y;

	m_CamPos.x = a_CamPos.x;
	m_CamPos.y = a_CamPos.y;

	m_LastTime = a_LastTime;
}

void CPtcEmitter::Emt_Render(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush)
{
	if (m_PtcImage == NULL)
		return;

	m_RenderPos.x = SpawnPosition.x + m_ScreenHalf.x - (int)m_CamPos.x;  //이쪽 계산이 좀 더 부드럽게 움직인다.
	m_RenderPos.y = SpawnPosition.y + m_ScreenHalf.y - (int)m_CamPos.y;

	float PER = (m_LastTime - CreatedTime) / LifeTime;

	Vector2D SimulatedPos = m_RenderPos + (Velocity * PER);
	Vector2D SimulatedSize = StartSize + (EndSize - StartSize) * PER;

	SimulatedPos.x = m_RenderPos.x + (Velocity.x * PER);
	SimulatedPos.y = m_RenderPos.y + (Velocity.y * PER);

	SimulatedSize.x = StartSize.x + (EndSize.x - StartSize.x) * PER;
	SimulatedSize.y = StartSize.y + (EndSize.y - StartSize.y) * PER;

	float Alpha_Per = StartAlpha + ((EndAlpha - StartAlpha) * PER);

	// 1.0 -> 0.0f; 
	float x = SimulatedPos.x - SimulatedSize.x * 0.5f;
	float y = SimulatedPos.y - SimulatedSize.y * 0.5f;

	AdjustRenderPosForShake(x);
	AdjustRenderPosForShake(y);

	D2D1_POINT_2F center_pos = { x + SimulatedSize.x * 0.5f, y + SimulatedSize.y * 0.5f };
	a_pd2dRTarget->SetTransform(D2D1::Matrix3x2F::Rotation(m_DirRot, center_pos));

	a_pd2dRTarget->DrawBitmap(m_PtcImage,
		D2D1::RectF(x, y, x + SimulatedSize.x, y + SimulatedSize.y), 
		Alpha_Per, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);

	if (PER > 1.0f)
	{
		m_EmtActive = false;  //제거
	}

	a_pd2dRTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void CPtcEmitter::CalculateUVRect(float per)
{
	UVPer = per;

	int Index = (int)(UVPer * (UVX * UVY));

	int RWidth = ImgRealWidth / UVX;
	int RHeight = ImgRealHeight / UVY;

	// 인덱스 0부터 시작

	// 4 X 4 에서 4라면? 사실은 5.
	/*
	a a a a
	a a a a
	a a a a
	a a a a
	*/

	int XX = Index % UVX; // 0 ,1,2,3
	int YY = 0;
	if (Index != 0)
	{
		YY = Index / UVX;
	}
	UVRect.left = XX * RWidth;
	UVRect.top = YY * RHeight;

	UVRect.right = (XX + 1) * RWidth;
	UVRect.bottom = (YY + 1) * RHeight;
}

void CPtcEmitter::MslExpRender(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush)
{
	if (m_MslExp_Img == NULL)
		return;

	//GetGameTimeSinceCreation() == m_LastTime
	static float a_PassTime = 0.0f;
	a_PassTime = (float)((m_LastTime - CreatedTime) * 0.001);

	CalculateUVRect(a_PassTime); //MainImage_ExplodeSheet->SetUVPer(GetGameTimeSinceCreation());
	//MainImage_ExplodeSheet->HandleRender(GetWorld()->Direct2DManager->RenderTarget, Loc.X - 120, Loc.Y - 120, 240, 240);
	//---->

	m_RenderPos.x = SpawnPosition.x + m_ScreenHalf.x - (int)m_CamPos.x;  //이쪽 계산이 좀 더 부드럽게 움직인다.
	m_RenderPos.y = SpawnPosition.y + m_ScreenHalf.y - (int)m_CamPos.y;

	static float x = 0.0f;
	static float y = 0.0f;
	static float width = 0.0f;
	static float height = 0.0f;
	x = m_RenderPos.x - 110.0f; //120.0f;
	y = m_RenderPos.y - 110.0f; //120.0f;
	width = 220.0f; //240.0f;
	height = 220.0f; //240.0f;

	AdjustRenderPosForShake(x);
	AdjustRenderPosForShake(y);
	//----------- 이미지 그리기
	a_pd2dRTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	a_pd2dRTarget->DrawBitmap(m_MslExp_Img, D2D1::RectF(x, y, x + width, y + height), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, UVRect);
	//----------- 이미지 그리기

	if (a_PassTime > 1.0f)
	{
		m_EmtActive = false;  //제거
	}
}
