#include "framework.h"
#include "CMonster.h"
#include "SpawnPos.h"
#include "CMonster_Mgr.h"
#include "CHero.h"
#include "GlobalValue.h"

CMonster::CMonster()
{
	m_isActive = false;
	m_SpawnIdx = -1;   //���� �� -1�� �ʱ�ȭ

	m_MaxHP = 400.0f;
	m_CurHP = 400.0f;

	m_Speed = 150.0f;
	m_HalfColl = 20;

	m_AIState = MAI_Patrol;
}

CMonster::~CMonster()
{
}

void CMonster::Init_Unit()
{
}

void CMonster::Update_Unit(double a_DeltaTime, VecINT2D& a_Center, Vector2D& a_CamPos, CHero& a_Hero)
{
	m_ScreenHalf.x = a_Center.x; //Main DC �� ���� ���� �ݻ�����
	m_ScreenHalf.y = a_Center.y;

	m_CamPos.x = a_CamPos.x;
	m_CamPos.y = a_CamPos.y;

	MonsterAI(a_DeltaTime, a_Hero);

	static Vector2D a_Sc_HLen;
	static Vector2D a_Obj_HLen;

	a_Sc_HLen.x = m_ScreenHalf.x; // +40;
	a_Sc_HLen.y = m_ScreenHalf.y; // +40;

	if (m_SocketImg != NULL)
	{
		a_Obj_HLen.x = m_SocketImg->GetSize().width / 2.0f;   //���� �� ������
		a_Obj_HLen.y = m_SocketImg->GetSize().height / 2.0f;  //���� �� ������ 
	}
	else
	{
		a_Obj_HLen.x = 50.0f;
		a_Obj_HLen.y = 50.0f;
	}

	m_InScRect = CheckAABB(m_CamPos, a_Sc_HLen, m_CurPos, a_Obj_HLen);
}

void CMonster::Render_Unit(ID2D1HwndRenderTarget* a_pd2dRTarget, ID2D1SolidColorBrush* a_pBrush)
{
	if (m_SocketImg == NULL)
		return;

	//m_RenderPos.x = m_CurPos.x - (int)m_CamPos.x + m_ScreenHalf.x;  //�̷���
	//m_RenderPos.y = m_CurPos.y - (int)m_CamPos.y + m_ScreenHalf.y;
	m_RenderPos.x = m_CurPos.x + m_ScreenHalf.x - (int)m_CamPos.x;  //���� ����� �� �� �ε巴�� �����δ�.
	m_RenderPos.y = m_CurPos.y + m_ScreenHalf.y - (int)m_CamPos.y;

	static VecINT2D img_size;
	static VecINT2D img_Half;
	img_size.x = m_SocketImg->GetSize().width;
	img_size.y = m_SocketImg->GetSize().height;
	img_Half.x = img_size.x / 2.0f;
	img_Half.y = img_size.y / 2.0f;

	//---- HP Bar Render
	static float FigureAlpha = 1.0f;  //���� ������
	static float a_CacMXX = 0.0f;
	static float a_CacMYY = (int)(img_Half.x * 0.8f);
	static float a_HpBSize = 50;
	static float a_CurHpSize = 0;
	a_CacMXX = a_HpBSize / 2.0f;
	a_CurHpSize = a_HpBSize * ((float)m_CurHP / (float)m_MaxHP);

	a_CacMYY = (int)(img_Half.y * 0.67f); //0.83f);

	static float a_iXX = 0; //������ �ݿø� ȯ��(�׸���� ���� ��ǥ�̱� ������...)
	static float a_iYY = 0;
	a_iXX = m_RenderPos.x;  //(int)m_CurPos.x;
	a_iYY = m_RenderPos.y;  //(int)m_CurPos.y;
	AdjustRenderPosForShake(a_iXX);
	AdjustRenderPosForShake(a_iYY);

	a_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red, FigureAlpha));
	a_pd2dRTarget->FillRectangle(D2D1::RectF(a_iXX - a_CacMXX, a_iYY - a_CacMYY,
		a_iXX - a_CacMXX + a_CurHpSize, a_iYY - (a_CacMYY + 9.0f)), a_pBrush);

	a_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White, FigureAlpha));
	a_pd2dRTarget->DrawRectangle(D2D1::RectF(a_iXX - a_CacMXX, a_iYY - a_CacMYY,
		a_iXX - a_CacMXX + a_HpBSize, a_iYY - (a_CacMYY + 9.0f)), a_pBrush);

	a_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	////---- HP Bar Render

	//------ ĳ���� Render
	a_pd2dRTarget->DrawBitmap(m_SocketImg,
		D2D1::RectF(a_iXX - img_Half.x,
			a_iYY - img_Half.y,
			a_iXX + img_Half.x,
			a_iYY + img_Half.y)
	);
	//------ ĳ���� Render
}

void CMonster::Destroy_Unit()
{
}

void CMonster::Spawn(float a_XX, float a_YY)
{
	m_CurPos.x = a_XX;
	m_CurPos.y = a_YY;

	m_isActive = true;

	m_CurHP = m_MaxHP;

	m_SocketImg = g_Mon_Mgr.m_ImgList[0];

	m_BasePos = m_CurPos;
	m_bMvPtOnOff = false; //Patrol Move
	m_WaitTime = ((rand() % 251) + 50) / 100.0f;
}

void CMonster::MonsterAI(float a_DeltaTime, CHero& a_Hero)
{
	static float m_AttackTick = 0.0f;

	if (0.0f < m_AttackTick)
		m_AttackTick = m_AttackTick - a_DeltaTime;

	if (m_AIState == MAI_Patrol)
	{
		//---��Ʈ�� ���¶�� �ϴ��� 7m �������� ���� �����ϸ� �����ϰڴٴ� �ڵ�
		a_CacVLen = a_Hero.m_CurPos - m_CurPos;
		a_CacDist = a_CacVLen.Magnitude();
		if (a_CacDist < m_TraceDist) //���ݰŸ�
		{
			m_AIState = MAI_NormalTrace;   //�Ϲ� �������� ���ư��� ���ݹ����ȿ� �ֱ� ������ �ٷ� ������ ���̴�.
			m_AggroTarget = &a_Hero;

			return;
		}
		//---��Ʈ�� ���¶�� �ϴ��� 7m �������� ���� �����ϸ� �����ϰڴٴ� �ڵ�

		if (m_bMvPtOnOff == true)
		{
			m_NowStep = a_DeltaTime * m_MoveVelocity; //�̹��� �Ѱ��� ���� (����)

			a_CacEndVec = m_PatrolTarget - m_CurPos;
			m_DirMvVec = a_CacEndVec;
			m_DirMvVec.Normalize();     //���ͳ��� �浹�Ǵ��� Ÿ������ �̵��ϱ� ���� ó��
			//���ͳ��� �浹�Ǽ� ��ǥ���� �������� ���ϴ� ������ �ذ��ϱ� ���ؼ��� ���������� �����ϴ� �ð��� ���� ��
			//���� �ð���ŭ�� �̵���Ű�� �������� �����Ѵ�.

			m_AddTimeCount = m_AddTimeCount + a_DeltaTime;
			if (m_MoveDurTime <= m_AddTimeCount) //��ǥ���� ������ ������ �����Ѵ�.
			{
				m_bMvPtOnOff = false;
			}
			else
			{
				m_CurPos = m_CurPos + (m_DirMvVec * m_NowStep);
			}
		}
		else
		{
			m_WaitTime = m_WaitTime - a_DeltaTime;
			if (0.0f < m_WaitTime)
			{
				//������ �ִϸ��̼����� �ٲ��ִ� �κ�
			}
			else
			{
				m_WaitTime = 0.0f;
				a_AngleRan = (rand() % 271) + 30;
				a_LengthRan = (rand() % 70) + 30; //a_LengthRan = Random.Range(3, 8);

				a_DirPtVec = m_CurPos - m_BasePos;

				//���͸� ȸ�� ������... a_AngleRan
				if (a_DirPtVec.Magnitude() < 1.0f)
				{
					a_CacAngle = 0.0f;
				}
				else
				{
					a_CacAngle = a_DirPtVec.GetAngle();
				}

				a_CacAngle = a_CacAngle + (float)a_AngleRan;
				a_Vect = Vector2D::GetNormalFromRotation(a_CacAngle); //������ ���ͷ� ȯ��

				m_PatrolTarget = m_BasePos + (a_Vect * (float)a_LengthRan);

				m_DirMvVec = m_PatrolTarget - m_CurPos;
				m_MoveDurTime = m_DirMvVec.Magnitude() / m_MoveVelocity; //�����ϴµ� �ɸ��� �ð�
				m_AddTimeCount = 0.0;
				m_DirMvVec.Normalize();

				m_WaitTime = ((rand() % 301) + 20) / 100.0f; //Random.Range(0.2f, 3.0f);
				m_bMvPtOnOff = true;
			}//else
		}//else
	}//if (m_AIState == MonAIState.MAI_Patrol)

	if (m_AIState == MAI_AggroTrace)
	{
		if (m_AggroTarget != NULL)
		{
			a_CacVLen = m_AggroTarget->m_CurPos - m_CurPos;

			a_CacDist = a_CacVLen.Magnitude();
			m_MoveDir = a_CacVLen;
			m_MoveDir.Normalize();

			if (a_CacDist <= (a_Hero.m_HalfColl + m_HalfColl + 10.0f)) //���ݰŸ�
			{
				m_AIState = MAI_NormalTrace;
			}

			if ((a_Hero.m_HalfColl + m_HalfColl) < a_CacDist) //���ݰŸ� //else //�����Ÿ��̸鼭 ���ݰŸ��� �ƴ� ��... 
			{
				m_NowStep = m_MoveVelocity * 3.5f * a_DeltaTime; //�Ѱ��� ũ��
				a_MoveNextStep = m_MoveDir * m_NowStep;      //�Ѱ��� ����

				m_CurPos = m_CurPos + a_MoveNextStep;
			}//else if (m_AttackDist <= a_CacDist ) //���ݰŸ��� �ƴ� ��  
		}
		else
		{
			m_AIState = MAI_Patrol;
			m_bMvPtOnOff = false;
		}
	}//if (m_AIState == MAI_AggroTrace)

	if (m_AIState == MAI_NormalTrace)
	{
		if (m_AggroTarget != NULL)
		{
			a_CacVLen = m_AggroTarget->m_CurPos - m_CurPos;
			a_CacDist = a_CacVLen.Magnitude();

			if (a_CacDist < m_TraceDist) //�����Ÿ�
			{
				m_MoveDir = a_CacVLen;
				m_MoveDir.Normalize();

				////ĳ���� ������ ȸ��   
				//a_TargetRot = Quaternion.LookRotation(m_MoveDir);
				//transform.rotation = Quaternion.Slerp(transform.rotation, a_TargetRot, Time.deltaTime * m_RotSpeed);
				////ĳ���� ������ ȸ��   

				if (a_CacDist < (a_Hero.m_HalfColl + m_HalfColl + 10.0f)) //���ݰŸ�
				{
					if (m_AttackTick <= 0.0f)  //���� ���� �ȿ� ������ ��� ����
					{
						//���Ͱ� ���ΰ� ���� ���� 
						a_Hero.TakeDamage(10);

						m_AttackTick = 1.0f;
					}
				}
				if ((a_Hero.m_HalfColl + m_HalfColl) < a_CacDist) //���ݰŸ� //else //�����Ÿ��̸鼭 ���ݰŸ��� �ƴ� ��... 
				{
					m_NowStep = m_MoveVelocity * 3.0f * a_DeltaTime; //�Ѱ��� ũ��
					a_MoveNextStep = m_MoveDir * m_NowStep;      //�Ѱ��� ����
					m_CurPos = m_CurPos + a_MoveNextStep;
				}//else if (m_AttackDist <= a_CacDist ) //���ݰŸ��� �ƴ� ��  

			}//if (a_CacDist < m_TraceDist) //�����Ÿ�
			else
			{
				m_AIState = MAI_Patrol;
				m_bMvPtOnOff = false;
			}
		}//if (m_AggroTarget != null)
		else
		{
			m_AIState = MAI_Patrol;
			m_bMvPtOnOff = false;
		}
	}//if (m_AIState == MonAIState.MAI_NormalTrace)
}

bool CMonster::CheckAABB(Vector2D& AACen, Vector2D& AAHLen, Vector2D& BBCen, Vector2D& BBHLen)
{
	static Vector2D a_AA_Min;
	static Vector2D a_AA_Max;
	static Vector2D a_BB_Min;
	static Vector2D a_BB_Max;

	a_AA_Min.x = AACen.x - AAHLen.x;
	a_AA_Min.y = AACen.y - AAHLen.y;
	a_AA_Max.x = AACen.x + AAHLen.x;
	a_AA_Max.y = AACen.y + AAHLen.y;

	a_BB_Min.x = BBCen.x - BBHLen.x;
	a_BB_Min.y = BBCen.y - BBHLen.y;
	a_BB_Max.x = BBCen.x + BBHLen.x;
	a_BB_Max.y = BBCen.y + BBHLen.y;

	if (a_AA_Min.x <= a_BB_Max.x && a_AA_Max.x >= a_BB_Min.x &&
		a_AA_Min.y <= a_BB_Max.y && a_AA_Max.y >= a_BB_Min.y)
	{
		return true;
	}

	return false;
}