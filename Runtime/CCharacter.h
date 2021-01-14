#pragma once

//--------------------- GDI+ Image ����� ���� ��� �߰�
#include <ole2.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;
//--------------------- GDI+ Image ����� ���� ��� �߰�

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
			//--- GDI+ ����
			if (m_ImgList[i] != NULL)
			{
				delete m_ImgList[i];
				m_ImgList[i] = NULL;
			}
			//--- GDI+ ����
		}

		m_ImgList.clear();
	}

	void LoadTexImg(const TCHAR *a_FName)
	{
		//�ش� ��ο��� �̹��� �ҷ�����
		Image *a_pBitmap = Image::FromFile(a_FName);
		m_ImgList.push_back(a_pBitmap);
	}
};
//-------------------------------------------class CMtion


class CCharacter
{
	int m_TrackNumber;				//����ȣ

	Image *m_SocketImg;				//�ִ� ����ó�� ����� ������
	INT m_ImgSizeX;					//�̹��� ���� ������
	INT m_ImgSizeY;					//�̹��� ���� ������

public:
	//-----------ĳ���� �̵� ���� ����
	POINT m_CurPos;					//������ġ
	double m_CacAddPosX;			//double�� �ӵ� ���� ����
	double m_MoveSpeed = 170.0;		//�̵��ӵ�
	//-----------ĳ���� �̵� ���� ����

	int m_Ranking;					//������ ���

	//-----------�۾� ����� ����
	static HFONT m_hFont, m_hArrvFont;
	HFONT m_OldFont;
	TCHAR m_TmpStr[128];
	//-----------�۾� ����� ����

	//-----------�ִϸ��̼� ���� ����
	AniState m_CurAniState;			//���� �ִϸ��̼� ����
	vector<CMotion*> m_MotionList;

	int m_NowImgCount;				//�ִϼ���ó�� ����� �̹��� ī��Ʈ
	float m_EachAniDelay;			//�÷��� ���� �ð�
	float m_AniTickCount;			//���� �÷��ӱ��� �ð� Add
	int m_CurAniInx;				//���� Ani Index
	//-----------�ִϸ��̼� ���� ����

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