#pragma once

#include "Vector2D.h"
#pragma comment (lib, "d2d1.lib")     //<---Direct2D 사용하기...
#include <d2d1.h>

class CBackGround
{
private:
	ID2D1Bitmap* m_pBGBmp = NULL;

	//----------절대좌표 이동 계산용 변수
	VecINT2D		m_ScreenHalf;       //화면 반 사이즈 저장용 변수
	Vector2D		m_CamPos = { 0.0f, 0.0f };           //현재 위치
	//----------절대좌표 이동 계산용 변수

	VecINT2D m_ImgSize;
	VecINT2D m_TileStPos;	//Tile Start Pos
	int X_Tile = 3;
	int Y_Tile = 3;

public:
	VecINT2D m_WorldHfSize; //Half

public:
	void BGMgrInit(ID2D1Bitmap* (*MyLoadBitmap)(LPCWSTR fileName) = NULL);
	void BGMgrUpdate(float a_DeltaTime, VecINT2D& a_Center, Vector2D& a_CamPos);
	void BGMgrRender(ID2D1HwndRenderTarget* a_pd2dRTarget);
	void BGMgrDestroy();
};

extern CBackGround g_BG_Mgr;
