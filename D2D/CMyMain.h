#pragma once

#pragma comment (lib, "d2d1.lib")  //<---Direct2D 사용하기...
#pragma comment (lib, "Dwrite.lib") //폰트 사용을 위하여...
#pragma comment (lib, "winmm.lib") //timeGetTime()  함수 사용을 위하여...
#pragma comment (lib, "WindowsCodecs.lib") //Direct2D는 이미지를 로딩하는 함수가 없기 때문에
//외부 라이브러리인 WIC라는 라이브러리의 이미지로부터 이미지 데이터를 만들 수 있다. 

#include <d2d1.h>                  //<---Direct2D 사용하기....
#include <d2d1helper.h>            //<---Direct2D 제공하는 각종 함수 사용을 위해...
#include <dwrite.h>                //폰트 사용을 위하여...

#include <wincodec.h>			   //이미지 로딩을 위하여...
#include "Vector2D.h"

class CBullet;

class CMyMain
{
public:
	CMyMain();
	~CMyMain();

private:
	HWND m_hWnd = NULL;

	ID2D1Factory* m_pd2dFactory = NULL;	  
	ID2D1HwndRenderTarget* m_pd2dRenderTarget = NULL;

	ID2D1SolidColorBrush* m_Brush = NULL;   //<--도형 그리기 위한 용도

	//DWrite의 Factory가 하나 더 필요한다.
	IDWriteFactory* m_pDWriteFactory = NULL;
	IDWriteTextFormat* m_pTextFormat = NULL;

	//--------- DeltaTime 관련 변수
	float		m_DeltaTime;
	DWORD		m_LastTime;
	DWORD		m_CurTime;
	//--------- DeltaTime 관련 변수

	//--------------FPS 출력용 변수
	DWORD		m_LastTick = 0;
	int			m_FlipCount = 0;
	int			m_OldFlipCount = 0;
	//--------------FPS 출력용 변수

	IWICImagingFactory* m_pImageFactory = NULL;

	class CHero*  m_Hero;
	//카메라의 절대좌표와 오브젝트의 절대좌표의 갭을 계산하고
	//화면의 반사이즈만큼 더해서 그려주면 된다.
	//( 실제로 랜더링할 때는 화면의 반사이즈만큼을 더해서 그려야 한다.
	//  왜냐하면 화면의 Left, Top이 0, 0 좌표이기 때문에...
	//	항상 카메라는 화면의 중앙이 위치하기 때문이다. )
	VecINT2D m_ScreenHalf;  //화면의 반사이즈 계속 계산해 준다.
	//카메라의 절대좌표(전체 지형 기준의 카메라의 좌표_
	Vector2D m_CamPos = { 0.0f, 0.0f };	

	//-------------- Edit 기능
	bool  m_IsMonEdit = false;
public:
	TCHAR m_szTitle[128] = { _T('\0') };                  // 제목 표시줄 텍스트입니다.
	//-------------- Edit 기능

public:
	void MainInit(HWND a_hWnd);
	void MainUpdate();
	void MainRender(HWND a_hWnd);
	void MainDestroy();

	void LimitMoveCam();

	static bool CheckCollision(CBullet* a_RefBull, DWORD a_CurTime);

	//이미지 로딩 함수 선언
	static ID2D1Bitmap* D2DLoadImg(LPCWSTR fileName);
	static HRESULT D2DLoadBitmap(LPCWSTR fileName, ID2D1RenderTarget* a_pRenderTarget,
		IWICImagingFactory* a_pImgFactory, ID2D1Bitmap** bitmap);

	void LoadMonSpPos(); //Spawn
	void SaveMonSpPos(); //Spawn

	void On_MessageHook(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

extern CMyMain g_MyMain;

