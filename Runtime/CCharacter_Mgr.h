#pragma once
#include "CCharacter.h"

#include <vector>
using namespace std;

class CCharacter_Mgr
{
	HWND m_hWnd;
	vector<Image*> m_ImgList;		//캐릭터 별 애니 없는 이미지 로딩 변수
	CCharacter m_CharList[7];		//캐릭터 말 7마리

	float m_RandTic;				//주기적으로 말들의 속도를 변경 시켜주기 위한 변수

public:
	CCharacter_Mgr();
	~CCharacter_Mgr();

public:
	void CM_Init(HWND a_hWnd);		//초기화 함수
	void CM_ReSetPos();
	void CM_UpDate(double a_DeltaTime, RECT *a_rt = NULL,
		bool a_isRaceStart = false, int *a_Ranking = NULL);		 //게임의 연산 담당 부분
	void CM_Render(HDC a_hdc);		//게임의 그리기 부분
	void CM_Destroy();
};

extern CCharacter_Mgr g_Char_Mgr;