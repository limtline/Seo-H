#pragma once

#pragma comment (lib, "d2d1.lib")  //<---Direct2D ����ϱ�...
#pragma comment (lib, "Dwrite.lib") //��Ʈ ����� ���Ͽ�...
#pragma comment (lib, "winmm.lib") //timeGetTime()  �Լ� ����� ���Ͽ�...
#pragma comment (lib, "WindowsCodecs.lib") //Direct2D�� �̹����� �ε��ϴ� �Լ��� ���� ������
//�ܺ� ���̺귯���� WIC��� ���̺귯���� �̹����κ��� �̹��� �����͸� ���� �� �ִ�. 

#include <d2d1.h>                  //<---Direct2D ����ϱ�....
#include <d2d1helper.h>            //<---Direct2D �����ϴ� ���� �Լ� ����� ����...
#include <dwrite.h>                //��Ʈ ����� ���Ͽ�...

#include <wincodec.h>			   //�̹��� �ε��� ���Ͽ�...
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

	ID2D1SolidColorBrush* m_Brush = NULL;   //<--���� �׸��� ���� �뵵

	//DWrite�� Factory�� �ϳ� �� �ʿ��Ѵ�.
	IDWriteFactory* m_pDWriteFactory = NULL;
	IDWriteTextFormat* m_pTextFormat = NULL;

	//--------- DeltaTime ���� ����
	float		m_DeltaTime;
	DWORD		m_LastTime;
	DWORD		m_CurTime;
	//--------- DeltaTime ���� ����

	//--------------FPS ��¿� ����
	DWORD		m_LastTick = 0;
	int			m_FlipCount = 0;
	int			m_OldFlipCount = 0;
	//--------------FPS ��¿� ����

	IWICImagingFactory* m_pImageFactory = NULL;

	class CHero*  m_Hero;
	//ī�޶��� ������ǥ�� ������Ʈ�� ������ǥ�� ���� ����ϰ�
	//ȭ���� �ݻ����ŭ ���ؼ� �׷��ָ� �ȴ�.
	//( ������ �������� ���� ȭ���� �ݻ����ŭ�� ���ؼ� �׷��� �Ѵ�.
	//  �ֳ��ϸ� ȭ���� Left, Top�� 0, 0 ��ǥ�̱� ������...
	//	�׻� ī�޶�� ȭ���� �߾��� ��ġ�ϱ� �����̴�. )
	VecINT2D m_ScreenHalf;  //ȭ���� �ݻ����� ��� ����� �ش�.
	//ī�޶��� ������ǥ(��ü ���� ������ ī�޶��� ��ǥ_
	Vector2D m_CamPos = { 0.0f, 0.0f };	

	//-------------- Edit ���
	bool  m_IsMonEdit = false;
public:
	TCHAR m_szTitle[128] = { _T('\0') };                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
	//-------------- Edit ���

public:
	void MainInit(HWND a_hWnd);
	void MainUpdate();
	void MainRender(HWND a_hWnd);
	void MainDestroy();

	void LimitMoveCam();

	static bool CheckCollision(CBullet* a_RefBull, DWORD a_CurTime);

	//�̹��� �ε� �Լ� ����
	static ID2D1Bitmap* D2DLoadImg(LPCWSTR fileName);
	static HRESULT D2DLoadBitmap(LPCWSTR fileName, ID2D1RenderTarget* a_pRenderTarget,
		IWICImagingFactory* a_pImgFactory, ID2D1Bitmap** bitmap);

	void LoadMonSpPos(); //Spawn
	void SaveMonSpPos(); //Spawn

	void On_MessageHook(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

extern CMyMain g_MyMain;

