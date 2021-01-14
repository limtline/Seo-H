#pragma once

//--------------------- GDI+ Image 사용을 위한 헤더 추가
#include <ole2.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;
//--------------------- GDI+ Image 사용을 위한 헤더 추가

#include <vector>
using namespace std;

enum AniState
{
	None,
	Race_Ready,
	Idle,
	Front_Walk,
	Back_Walk,
	Left_Walk,
	Right_Walk,
	AniLength,
};


//-------------------------------------------class CMtion
class CMotion
{
public:
	vector<Image*> m_ImgList;

public:
	CMotion()
	{
	}
	~CMotion()
	{
		for (int i = 0; i < m_ImgList.size(); i++)
		{
			//--- GDI+ 해제
			if (m_ImgList[i] != NULL)
			{
				delete m_ImgList[i];
				m_ImgList[i] = NULL;
			}
			//--- GDI+ 해제
		}

		m_ImgList.clear();
	}

	void LoadTexImg(const TCHAR *a_FName)
	{
		//해단 경로에서 이미지 불러오기
		Image *a_pBitmap = Image::FromFile(a_FName);
		m_ImgList.push_back(a_pBitmap);
	}
};
//-------------------------------------------class CMtion


class CCharacter
{
	int m_TrackNumber;				//말번호

	Image *m_SocketImg;				//애니 소켓처럼 사용할 포인터
	INT m_ImgSizeX;					//이미지 가로 사이즈
	INT m_ImgSizeY;					//이미지 세로 사이즈

public:
	//-----------캐릭터 이동 계산용 변수
	POINT m_CurPos;					//현재위치
	double m_CacAddPosX;			//double형 속도 계산용 변수
	double m_MoveSpeed = 170.0;		//이동속도
	//-----------캐릭터 이동 계산용 변수

	int m_Ranking;					//도착시 등수

	//-----------글씨 쓰기용 변수
	static HFONT m_hFont, m_hArrvFont;
	HFONT m_OldFont;
	TCHAR m_TmpStr[128];
	//-----------글씨 쓰기용 변수

	//-----------애니메이션 관련 변수
	AniState m_CurAniState;			//현재 애니메이션 상태
	vector<CMotion*> m_MotionList;

	int m_NowImgCount;				//애니소켓처럼 사용할 이미지 카운트
	float m_EachAniDelay;			//플레임 간격 시간
	float m_AniTickCount;			//다음 플레임까지 시간 Add
	int m_CurAniInx;				//진행 Ani Index
	//-----------애니메이션 관련 변수

public:
	CCharacter();
	~CCharacter();

	void CChar_Init(Image *a_LoadImg = NULL);
	void CChar_ReSetPos(int a_TrackNb, LONG a_XX, LONG a_YY);
	void CChar_Update(double a_DeltaTime, RECT *a_rt = NULL,
		bool a_isRaceStart = false, int *a_Ranking = NULL);
	void CChar_Render(HDC a_hdc);
	void CChar_Destroy();

	void LoadAnimList(int Number = 0);
	bool ChangeState(AniState newState);
	void AniFrameUpDate(double a_DeltaTime);
};