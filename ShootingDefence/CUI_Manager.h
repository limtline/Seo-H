#pragma once

#include <functional>  //function
using namespace std;   //function

#include "Vector2D.h"

//----- GDI+ Image 사용을 위한 헤더 추가
#include <ole2.h>  
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;
//----- GDI+ Image 사용을 위한 헤더 추가

//--------------------------- CButton class
class CButton
{
public:
	bool        m_isBtnDown;
	Image*        m_BtnUpImg;
	Image*        m_BtnDownImg;

	Vector2D    m_LT_Pos;   //Left Top Position
	Vector2D    m_WH_Size;  //Width Height

	ColorMatrix m_DownCM;   //DownImg가 없을 때 
	function<void(void)> ClickCallback = NULL; //<---STL에서 제공해 주는 함수 포인터 변수 생성

public:
	CButton();
	~CButton();
	void SetBtnRect(float a_LeftPos, float a_TopPos,
		float a_WSize = 0.0f, float a_HSize = 0.0f);

	void Load_Rsc(const TCHAR* a_BtnUpStr, const TCHAR* a_BtnDownStr = NULL);
	void BtnRender(HDC a_hDC);
	void Destroy();

	void LBtnDown(Vector2D a_MosPos);
	bool LBtnUp();
	void OnClick_AddListener(function<void(void)> Callback = NULL); //PointerUp
};
//--------------------------- CButton class

//--------------------------- CUI_Manager class
class CUI_Manager
{
	float  m_DeltaTime = 0.0f;

	HPEN h_Pen;
	HPEN h_OldPen;
	HBRUSH h_Brush;
	HBRUSH h_OldBrush;

	TCHAR m_StrBuff[128];
	HFONT m_OldFont;
	HFONT hFont_100 = NULL;
	HFONT hFont_50 = NULL;
	HFONT m_Font_26Godic = NULL;

	Image* m_GUIBack = NULL;
	Image* m_HP_Icon = NULL;

	//----- Shop
public:
	bool         m_ShopOnOff;

private:
	Image*         m_ShopBase;
	ColorMatrix  m_AlphaCM;

	HFONT         m_HumanF;

	CButton      m_CloseBtn;  //닫기 버튼

	Image*         m_ShopBG[3];
	Image*         m_Image[3];
	Image*         m_CoinImg[3];
	CButton      m_AddBtn[3];   //Add버튼
	//----- Shop


public:
	CUI_Manager();
	~CUI_Manager();

public:
	void UIMgr_Init();          //초기화 함수
	void UIMgr_Update(HWND a_hWnd, float a_DeltaTime,
		void(*NextLevelFunc)() = NULL, void(*ReSetGameFunc)() = NULL);    //게임의 연산 담당부분
	void UIMgr_Render(HDC a_MainDC, RECT& a_RT);     //게임의 그리기 부분
	void UIMgr_Destroy();       //종료 처리 부분

	void UserInfoPanel(HDC a_MainDC, Graphics& graphics, RECT& a_RT);

	void StoreInit();
	void StoreRender(HDC a_MainDC, Graphics& graphics, RECT& a_RT);
	void StoreDestroy();

	void LBtnDown(HWND a_hWnd);
	void LBtnUp();

};
//--------------------------- CUI_Manager class

extern CUI_Manager g_GUI_Mgr;