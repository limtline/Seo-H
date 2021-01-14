#include "framework.h"
#include "CBackGround.h"

void CBackGround::BGMgrInit(ID2D1Bitmap* (*MyLoadBitmap)(LPCWSTR fileName))
{
	if (MyLoadBitmap != NULL)
	{
		m_pBGBmp = 
			MyLoadBitmap(_T(".\\RscTrunk\\Background\\BackGround3.png"));

		if (m_pBGBmp != NULL)
		{
			m_ImgSize.x = m_pBGBmp->GetSize().width * 4;
			m_ImgSize.y = m_pBGBmp->GetSize().height * 4;

			m_WorldHfSize.x = (int)(m_ImgSize.x * (X_Tile / 2.0f));
			m_WorldHfSize.y = (int)(m_ImgSize.y * (Y_Tile / 2.0f));
		}
	}

}

void CBackGround::BGMgrUpdate(float a_DeltaTime, VecINT2D& a_Center, Vector2D& a_CamPos)
{
	m_ScreenHalf = a_Center; //Main DC 의 가로 세로 반사이즈
	m_CamPos = a_CamPos;
}

void CBackGround::BGMgrRender(ID2D1HwndRenderTarget* a_pd2dRTarget)
{
	m_TileStPos.x = m_ScreenHalf.x - m_WorldHfSize.x;
	m_TileStPos.y = m_ScreenHalf.y - m_WorldHfSize.y;

	static VecINT2D a_SetPos = { 0, 0 };
	static VecINT2D a_CacPos = { 0, 0 };

	for (int yy = 0; yy < Y_Tile; yy++)
	{
		a_SetPos.y = m_TileStPos.y + (m_ImgSize.y * yy);
		for (int xx = 0; xx < X_Tile; xx++)
		{
			a_SetPos.x = m_TileStPos.x + (m_ImgSize.x * xx);

			a_CacPos.x = a_SetPos.x - (int)m_CamPos.x;
			a_CacPos.y = a_SetPos.y - (int)m_CamPos.y;

			a_pd2dRTarget->DrawBitmap(m_pBGBmp,
				D2D1::RectF(a_CacPos.x, a_CacPos.y,
					a_CacPos.x + m_ImgSize.x, a_CacPos.y + m_ImgSize.y));
		}
	}

}

void CBackGround::BGMgrDestroy()
{
	if (m_pBGBmp != NULL)
	{
		m_pBGBmp->Release();
		m_pBGBmp = NULL;
	}
}

CBackGround g_BG_Mgr;