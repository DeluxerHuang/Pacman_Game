/*
Core File
�����ƶ����λ�ü��, ��ײ���, ��ͼ����
ָ���ж�
*/

#include "stdafx.h"
#include "PacManMove.h"

//PacMove:
PacMap* PacMove::pMapStage = NULL;

PacMove::PacMove(int row, int col)
{
	m_iFrame = 1;
	pMapStage = NULL;
	
	this->m_iRow = row;
	this->m_iCol = col;
	this->pt.x = m_iCol*(pMapStage->iBarrierSize)+(pMapStage->iBarrierSize/2);
	this->pt.y = m_iRow*(pMapStage->iBarrierSize)+(pMapStage->iBarrierSize/2);
	this->m_iX = pt.x;
	this->m_iY = pt.y;
}

//���ͻ�������װ��Ϊ��������[��][��]
int PacMove::TransPoint(int RealPt)
{
	int temp = 0;
	/*
	���������ת������PacMan��Enemies����ÿһ��λ�����ĵ�
	�����ж�, ���ƶ����������޷��ж�,Ҳ����Ҫ�ж�,
	ֻҪ�ڷ������ʱȷ�����꼴��
	���� ���ʽ�� -�ϰ���ߴ�/2 Ŀ�����������ľ���
	*/
	temp = (RealPt - pMapStage->iBarrierSize/2)/(pMapStage->iBarrierSize);

	return temp;
}

bool PacMove::IsCPoint()
{
	//��ײǰ�����ж�, ��ת����ͬ��Ҫ�����ƶ�ʱ�Ƿ񵽴����ĵ�
	int n = (pt.x - pMapStage->iBarrierSize/2) % pMapStage->iBarrierSize;
	int k = (pt.y - pMapStage->iBarrierSize/2) % pMapStage->iBarrierSize;

	bool flag = (n==0 && k==0);

	return flag;
}

//���µ�ͼ����
void PacMove::CenterPoint()
{
	if (IsCPoint())
	{
		m_iCol = TransPoint(pt.x);
		m_iRow = TransPoint(pt.y);
	}
}

bool PacMove::IsCollision()
{
	bool flag = false;

	//�����ͼ�����, ʹ��PacMan�ڵ�ͼ��һ�����ܽ��뵽��һ��
	int MapMaxLenth = pMapStage->iBarrierSize*MAPLENTH+pMapStage->iBarrierSize/2;
	int MapMinLenth = pMapStage->iBarrierSize/2;

	//ˢ��
	CenterPoint();

	if (m_iCol < 0 || m_iRow < 0 || (m_iCol > MAPLENTH-1) || m_iRow > (MAPLENTH -1))
	{
		flag = true;
	}
	else if (IsCPoint())
	{
		//�˴��ж�ǰ�з���ָ���Ƿ���Ч
		switch (twCmd)
		{
		case UP:
			if (m_iRow > 0 && !pMapStage->BarrierMap[m_iRow-1][m_iCol])//�߼���ͼ���ϰ���Ϊ��
			{
				flag = true;//����ǰ��
			}
			break;
		case DOWN:
			if (m_iRow < (MAPLENTH-1) && !pMapStage->BarrierMap[m_iRow+1][m_iCol])
			{
				flag = true;
			}
			break;
		case LEFT:
			if (m_iCol > 0 && !pMapStage->BarrierMap[m_iRow][m_iCol-1])
			{
				flag = true;
			}
			break;
		case RIGHT:
			if (m_iCol < (MAPLENTH - 1) && !pMapStage->BarrierMap[m_iRow][m_iCol+1])
			{
				flag = true;
			}
			break;
		}

		if (!flag)//�н�����û���ϰ���
		{
			tw = twCmd;
		}
	}

	m_iX = pt.x;
	m_iY = pt.y;

	//�˴��ǿ���ǰ��
	switch (tw)
	{
	case UP:
		if (m_iRow > 0 && !pMapStage->BarrierMap[m_iRow-1][m_iCol])//�߼���ͼ���ϰ���Ϊ��
		{
			//�н�������ǽ
			flag = true;
			break;
		}
		pt.y -= m_iSpeed;
		if (pt.y < MapMinLenth)
		{
			//�����н������ײ�
			pt.y = MapMaxLenth;
		}
		break;
	case DOWN:
		if (m_iRow < (MAPLENTH-1) && !pMapStage->BarrierMap[m_iRow+1][m_iCol])
		{
			flag = true;
			break;
		}
		pt.y += m_iSpeed;
		if (pt.y > MapMaxLenth)
		{
			//�����н�����붥��
			pt.y = MapMinLenth;
		}
		break;
	case LEFT:
		if (m_iCol > 0 && !pMapStage->BarrierMap[m_iRow][m_iCol-1])
		{
			flag = true;
			break;
		}
		pt.x -= m_iSpeed;
		if (pt.x < MapMinLenth)
		{
			//�����н�������Ҷ�
			pt.x = MapMaxLenth;
		}
		break;
	case RIGHT:
		if (m_iCol < (MAPLENTH - 1) && !pMapStage->BarrierMap[m_iRow][m_iCol+1])
		{
			flag = true;
			break;
		}
		pt.x += m_iSpeed;
		if (pt.x > MapMaxLenth)
		{
			//�����н���������
			pt.x = MapMinLenth;
		}
		break;
	}
	
	return flag;
}

void PacMove::DrawBlank(HDC hdc)
{
	RECT rect;

	rect.top = m_iY - DRAWDIST;
	rect.bottom = m_iY + DRAWDIST;
	rect.left = m_iX - DRAWDIST;
	rect.right = m_iX + DRAWDIST;
	
	FillRect(hdc, &rect, CreateSolidBrush(RGB(255,255,255)));
}

void PacMove::SetPosition(int Row, int Col)
{
	m_iRow = Row;
	m_iCol = Col;

	this->pt.x = m_iCol*pMapStage->iBarrierSize + pMapStage->iBarrierSize/2;
	this->pt.y = m_iRow*pMapStage->iBarrierSize + pMapStage->iBarrierSize/2;
}

int PacMove::GetRow()
{
	return m_iRow;
}

int PacMove::GetCol()
{
	return m_iCol;
}

//End 

//PacMan:
PacMan::PacMan(int x, int y)
	:PacMove(x, y)
{
	this->m_iSpeed = PACSPEED;
	twCmd = tw = LEFT;
	m_uScore = 0;
}

void PacMan::MoveBehavior()
{
	IsCollision();
}

void PacMan::CenterPoint()
{
	PacMove::CenterPoint();

	if (IsCPoint())
	{
		if (m_iRow>=0 && m_iRow<MAPLENTH && m_iCol>=0 && m_iCol<=MAPLENTH)
		{
			if (pMapStage->PeaMap[m_iRow][m_iCol])
			{
				//�Ե�����
				PlaySound(TEXT("sndcoin.wav"), NULL, SND_ASYNC | SND_FILENAME);
				m_uScore++;
				pMapStage->PeaMap[m_iRow][m_iCol] = false;
			}
		}
	}
}

void PacMan::SetTwCmd(ToWard cmd)
{
	twCmd = cmd;
}

bool PacMan::Win()
{
	//����ͼ�����޶���ʱ��Win
	for (int row=0; row<MAPLENTH; row++)
	{
		for (int col=0; col<MAPLENTH; col++)
		{
			if (pMapStage->PeaMap[row][col] == true)
			{
				return false;
			}
		}
	}

	PlaySound(TEXT("sndwin.wav"), NULL, SND_FILENAME | SND_ASYNC);
	return true;
}

unsigned int PacMan::GetScore()
{
	return m_uScore;
}

POINT PacMan::GetPosition()
{
	return pt;
}

void PacMan::Catch()
{
	tw = CATCH;
}

ToWard PacMan::GetToward()
{
	return tw;
}

void PacMan::Draw(HDC hdc)
{
	int x1 = 0, x2 = 0;
	int y1 = 0, y2 = 0;
	int offsetX;
	int offsetY;
	
	if (tw == CATCH)
	{
		
	}
	else if (m_iFrame % 2 == 0)
	{
		//��ͳ���

		 offsetX = DISTANCE/2+DRAWDIFF;
		 offsetY = DISTANCE/2+DRAWDIFF;

		 switch (tw)
		 {
		 case UP:
			 x1 = pt.x - offsetX;
			 x2 = pt.x + offsetX;
			 y1 = y2 = pt.y - offsetY;
			 break;
		 case DOWN:
			 x1 = pt.x + offsetX;
			 x2 = pt.x - offsetX;
			 y1 = y2 = pt.y + offsetY;
			 break;
		 case LEFT:
			 x1 = x2 = pt.x - offsetX;
			 y1 = pt.y + offsetY;
			 y2 = pt.y - offsetY;
			 break;
		 case RIGHT:
			 x1 = x2 = pt.x + offsetX;
			 y1 = pt.y - offsetY;
			 y2 = pt.y + offsetY;
			 break;
		 }

		 Arc(hdc, pt.x-DISTANCE, pt.y-DISTANCE, pt.x+DISTANCE,
			 pt.y+DISTANCE, x1, y1, x2, y2);
		 MoveToEx(hdc, x1, y1, NULL);
		 LineTo(hdc, pt.x, pt.y);
		 LineTo(hdc, x2, y2);
	}
	else if (m_iFrame % 3 == 0)
	{
		Ellipse(hdc, pt.x-DISTANCE, pt.y-DISTANCE,
			pt.x+DISTANCE, pt.y+DISTANCE);
	}
	else
	{
		switch (tw)
		{
		case UP:
			x1 = pt.x - DISTANCE;
			x2 = pt.x + DISTANCE;
			y1 = y2 = pt.y;
			break;
		case DOWN:
			x1 = pt.x + DISTANCE;
			x2 = pt.x - DISTANCE;
			y1 = y2 = pt.y;
			break;
		case LEFT:
			x1 = x2 = pt.x;
			y1 = pt.y + DISTANCE;
			y2 = pt.y - DISTANCE;
			break;
		case RIGHT:
			x1 = x2 = pt.x;
			y1 = pt.y - DISTANCE;
			y2 = pt.y + DISTANCE;
			break;
		}

		Arc(hdc, pt.x-DISTANCE, pt.y-DISTANCE, pt.x+DISTANCE, pt.y+DISTANCE,
			x1, y1, x2, y2);
		MoveToEx(hdc, x1, y1, NULL);
		LineTo(hdc, pt.x, pt.y);
		LineTo(hdc, x2, y2);
	}

	m_iFrame++;
	if (m_iFrame == 100)
		m_iFrame = 0;
}

//��ͣ����
void PacMan::SysPause() //������ͣ��ʾ
{
	//::FillRect(hdc, &rect, CreateSolidBrush(RGB(255,255,255)));
	while (true)
	{
		Sleep(1000);
		if ((GetAsyncKeyState(VK_SPACE) & 0x800f) || (GetAsyncKeyState('P') & 0x800f) ||
			(GetAsyncKeyState(VK_PAUSE) & 0x800f))
			break;
	}
}
//End

//Enemy:
PacMan* Enemy::pacman = NULL;

Enemy::Enemy(int x, int y)
	:PacMove(x, y)
{
	this->m_iSpeed = ENESPEED;
	tw = LEFT;
	twCmd = UP;
}

void Enemy::Catch()
{
	//ͨ������֮��Ĳ�ֵ�ͻ���ͼ�εĴ�С���ж��Ƿ�ץ

	int EneDX = pt.x - pacman->GetPosition().x;
	int EneDY = pt.y - pacman->GetPosition().y;

	if ((-DRAWDIST < EneDX && EneDX < DRAWDIST) &&
		(-DRAWDIST < EneDY && EneDY < DRAWDIST))
	{
		PlaySound(TEXT("sndcatch.wav"), NULL, SND_FILENAME | SND_ASYNC);
		pacman->Catch();
	}
}

void Enemy::Draw(HDC hdc)
{
	HPEN pen = CreatePen(NULL, 0, color);
	HPEN prevPen = (HPEN)SelectObject(hdc, pen);

	//ͷ������
	Arc(hdc, pt.x-DISTANCE, pt.y-DISTANCE, pt.x+DISTANCE, pt.y+DISTANCE,
		pt.x+DISTANCE, pt.y, pt.x-DISTANCE, pt.y);
	
	const int LegLenth = DISTANCE / LEGCOUNT;
	
	if (m_iFrame % 2 == 0)
	{
		MoveToEx(hdc, pt.x-DISTANCE, pt.y, NULL);
		LineTo(hdc, pt.x-DISTANCE, pt.y+DISTANCE-LegLenth);
		MoveToEx(hdc, pt.x+DISTANCE, pt.y, NULL);
		LineTo(hdc, pt.x+DISTANCE, pt.y+DISTANCE-LegLenth);

		//����enemy�Ȳ�
		for (int i=0; i<LEGCOUNT; i++)
		{
			Arc(hdc, pt.x-DISTANCE+i*2*LegLenth, pt.y+DISTANCE-2*LegLenth,
				pt.x-DISTANCE+(i+1)*2*LegLenth, pt.y+DISTANCE,
				pt.x-DISTANCE+i*2*LegLenth, pt.y+DISTANCE-LegLenth,
				pt.x-DISTANCE+(i+1)*2*LegLenth, pt.y+DISTANCE-LegLenth);
		}
	}
	else
	{
		MoveToEx(hdc, pt.x-DISTANCE, pt.y, NULL);
		LineTo(hdc, pt.x-DISTANCE, pt.y+DISTANCE);
		MoveToEx(hdc, pt.x+DISTANCE, pt.y, NULL);
		LineTo(hdc, pt.x+DISTANCE, pt.y+DISTANCE);

		MoveToEx(hdc, pt.x-DISTANCE, pt.y+DISTANCE, NULL);
		LineTo(hdc, pt.x-DISTANCE+LegLenth, pt.y+DISTANCE-LegLenth);

		for (int i=0; i<LEGCOUNT-1; i++)
		{
			Arc(hdc, pt.x-DISTANCE+(1+i*2)*LegLenth, pt.y+DISTANCE - 2*LegLenth,
				pt.x-DISTANCE+(3+i*2)*LegLenth, pt.y+DISTANCE,
				pt.x-DISTANCE+(1+i*2)*LegLenth, pt.y+DISTANCE-LegLenth,
				pt.x-DISTANCE+(3+i*2)*LegLenth, pt.y+DISTANCE-LegLenth);
		}

		MoveToEx(hdc, pt.x+DISTANCE, pt.y+DISTANCE, NULL);
		LineTo(hdc, pt.x+DISTANCE-LegLenth, pt.y+DISTANCE-LegLenth);
	}

	//Enemy eyes
	int radius = DISTANCE/5;

	switch (tw)
	{
	case UP:
		Ellipse(hdc, pt.x-2*radius, pt.y-2*radius, pt.x, pt.y);
		Ellipse(hdc, pt.x, pt.y-2*radius, pt.x+2*radius, pt.y);
		break;
	case DOWN:
		Ellipse(hdc, pt.x-2*radius, pt.y, pt.x, pt.y+2*radius);
		Ellipse(hdc, pt.x, pt.y, pt.x+2*radius, pt.y+2*radius);
		break;
	case LEFT:
		Ellipse(hdc, pt.x-3*radius, pt.y-radius, pt.x-radius, pt.y+radius);
		Ellipse(hdc, pt.x-radius, pt.y-radius, pt.x+radius, pt.y-radius);
		break;
	case RIGHT:
		Ellipse(hdc, pt.x-radius, pt.y-radius, pt.x+radius, pt.y+radius);
		Ellipse(hdc, pt.x+radius, pt.y-radius, pt.x+3*radius, pt.y+radius);
		break;
	}

	m_iFrame++;
	if (m_iFrame == 100)
		m_iFrame = 0;
	SelectObject(hdc, prevPen);
	DeleteObject(pen);
}

void Enemy::MoveBehavior()
{
	bool flag = IsCollision();
	EnemyAI(flag);
	Catch();
}

//End
/*
	�޸��㷨:
	ʹ�ú�ɫ�������ں�ɫ���ӷ�Χ�ڽ���������׷��
	�޸ĳ���ɫ�㷨
*/
//RedEnemy:
RedEne::RedEne(int x, int y)
	:Enemy(x, y)
{
	color = RGB(255, 0, 0);
}

void RedEne::Draw(HDC hdc)
{
	Enemy::Draw(hdc);
}

/*
��������ƶ�����һ��׷�������ĵ���
�߼�:
���ڵ����ܱ�ʱ, ������ж� ������һ��Բ������
������������λ����ͬ���, ���ڷ�Χ��ʱ, ��ʼ׷�� 
�����������ƶ�.
ͬʱ,�����ײǽ�ж�, ���PacMan����λ�ò���, ���������ǽ�Ա�ʱ,
��ά����һ״̬, ����ά�ָ�λ�ú�һ��ʱ���, ������ƶ�
*/
void RedEne::EnemyAI(bool flag)
{
	//�������srand��ʹ����������Ϊһ��, ��Ҫ��ͬʱ��
	//srand(static_cast<unsigned int>(time(NULL)));
	unsigned int r = rand();
	const int RowDis = this->m_iRow - pacman->GetRow();
	const int ColDis = this->m_iCol - pacman->GetCol();
	static int prevRow;
	static int prevCol;

	srand(static_cast<unsigned int>(time(NULL)) + r);
	int rCmd = rand(); //��������ķ�������

	if (flag)//��ײ
	{
		if (rCmd % 4 == 0)
		{
			tw == UP ? twCmd=LEFT : twCmd=UP;
		}
		else if (rCmd % 2 == 0)
		{
			tw == RIGHT ? twCmd=UP : twCmd=RIGHT;
		}
		else if (rCmd % 3 == 0)
		{
			tw == DOWN ? twCmd=RIGHT : twCmd=DOWN; 
		}
		else 
		{
			tw == LEFT ? twCmd=DOWN : twCmd=LEFT;
		}
	}
	else
	{
		if ((sqrt((abs(RowDis)*abs(RowDis) + abs(ColDis)*abs(ColDis))) <= 2*REDESCAN) &&
			(prevRow != this->m_iRow && prevCol != this->m_iCol))
		{
			if (RowDis == 0)
			{
				if (ColDis <= REDESCAN && ColDis > 0)
				{
					twCmd = LEFT;
				}
				else if (ColDis < 0 && ColDis >= -REDESCAN)
				{
					twCmd = RIGHT;
				}
			}
			else if (ColDis == 0)
			{
				if (RowDis <= REDESCAN && RowDis > 0)
				{
					twCmd = UP;
					return;
				}
				else if (RowDis < 0 && RowDis > -REDESCAN)
				{
					twCmd = DOWN;
				}
			}
		}
		if (rCmd % 2 == 0)
		{
			tw != RIGHT ? twCmd = LEFT : twCmd = RIGHT;
		}
		else if (rCmd % 3 == 0)
		{
			tw != DOWN ? twCmd = UP : twCmd = DOWN;
		}
		else
		{
			tw != LEFT ? twCmd = RIGHT : twCmd = LEFT;
		}
	}
	prevRow = this->m_iRow;
	prevCol = this->m_iCol;

	return;
}

//End

/*
	�㷨�޸�:
	�����н��ٶ�, ��չ��׷��ʱ,����ٶ�, ����ʹ��
	��Ҫ���һ�����ʵ����ӱ���: 
	�߼�:
	��������ͬһ�л���ʱ, ���ҷ�Χ��BLUESCAN��, ���ܹ���ʼֱ��׷��
	�����������һ����, ���ܹ���Χ��ȷ��һ��Pacman��λ��,ͨ��
	���бȽ���ȷ��λ��, �Ǿ�ȷ
	��������������������ײǽ, �����RedEnemy���߼�
*/
//BlueEne:
BlueEne::BlueEne(int x, int y)
	:RedEne(x, y)
{
	color = RGB(0, 0, 255);
}

void BlueEne::Draw(HDC hdc)
{
	Enemy::Draw(hdc);
}

void BlueEne::EnemyAI(bool flag)
{
	const int RowDis = this->m_iRow - pacman->GetRow();
	const int ColDis = this->m_iCol - pacman->GetCol();

	if (!flag && RowDis == 0)
	{
		if (ColDis <= BLUESCAN && ColDis > 0)
		{
			twCmd = LEFT;
		}
		else if (ColDis < 0 && ColDis >= -BLUESCAN)
		{
			twCmd = RIGHT;
		}
	}
	else if (!flag && ColDis == 0)
	{
		if (RowDis <= BLUESCAN && RowDis > 0)
		{
			twCmd = UP;
			return;
		}
		else if (RowDis < 0 && RowDis > -BLUESCAN)
		{
			twCmd = DOWN;
		}
	}
	else if(!flag && (RowDis*RowDis > ColDis*ColDis))
	{
		if (ColDis > 0)
		{
			twCmd = LEFT;
		}
		else if (ColDis < 0)
		{
			twCmd = RIGHT;
		}
	}
	else if (!flag && (RowDis*RowDis < ColDis*ColDis))
	{
		if (RowDis > 0)
		{
			twCmd = UP;
		}
		else if (RowDis < 0)
		{
			twCmd = DOWN;
		}
	}
	else
	{
		RedEne::EnemyAI(flag);
	}
}

//End

/*
	�޸��㷨:
	��ɫ�ĵ��˴���ɫ������:
	ÿ���10s?���߶��� ����һ��Ѱַ,�ҵ�PacMan��λ��,
	ʱ��������ͨ����ȡϵͳʱ����ȷ�� �����ڼ����ͼˢ��ʱ�����ж�
	�ڼ������ʹ����ɫ������Χ// �����ʵ�����������Χʹ�ö�λ���ӽ�׼
*/
//GreenEne

GreenEne::GreenEne(int x, int y)
	:RedEne(x, y)
{
	color = RGB(0, 255, 0);
}

void GreenEne::Draw(HDC hdc)
{
	Enemy::Draw(hdc);
}

void GreenEne::EnemyAI(bool flag)
{
	const int RowDis = this->m_iRow - pacman->GetRow();
	const int ColDis = this->m_iCol - pacman->GetCol();

	if (!flag)
	{
		if (GreenStack.IsEmpty())
		{
			if (RowDis*RowDis > ColDis*ColDis)
			{
				if (ColDis > 0)
				{
					twCmd = LEFT;
				}
				else if (ColDis < 0)
				{
					twCmd = RIGHT;
				}
			}
			else
			{
				if (RowDis > 0)
				{
					twCmd = UP;
				}
				else if (RowDis < 0)
				{
					twCmd = DOWN;
				}
			}
		}
		else
		{
			if (GreenStack.PopStack(this->NextPoint))
			{
				if (this->NextPoint.c_col == this->m_iCol)
				{
					if (this->NextPoint.c_row >= this->m_iRow)
					{
						twCmd =	DOWN;
					}
					else
					{
						twCmd = UP;
					}
				}
				else if (this->NextPoint.c_row == this->m_iRow)
				{
					if (this->NextPoint.c_col >= this->m_iCol)
					{
						twCmd = RIGHT;
					}
					else
					{
						twCmd = LEFT;
					}
				}
			}
			
		
		}
	}
	else
	{
		RedEne::EnemyAI(flag);
	}
}

//End;