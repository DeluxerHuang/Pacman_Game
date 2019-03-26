#include "stdafx.h"
#include "PacSearch.h"

//PacStack���������ջ�ṹ
void PacStack::InitStack()
{
	this->pTop = new PointNode;
	if (this->pTop == NULL)
	{
		MessageBox(NULL,TEXT("error: NO memmory to InitStack"), TEXT("PacTips"), MB_ICONERROR);
		PostQuitMessage(0);
	}
	else
	{
		this->pButtom = this->pTop;
		this->pButtom->pNext = NULL;
	}
}

void PacStack::PushStack(PacPoint pt)
{
	PointNode *CurNode = new PointNode;
	if (CurNode == NULL)
	{
		MessageBeep(MB_ICONASTERISK);
		ClearStack();
	}
	
	CurNode->PN_pt = pt;

	CurNode->pNext = this->pTop;
	this->pTop = CurNode;
}


bool PacStack::IsEmpty() const
{
	if (this->pTop == this->pButtom)
		return true;
	else 
		return false;
}

bool PacStack::PopStack(PacPoint &NextPoint)//��ջ,����λ��
{
	if (IsEmpty())
	{
		return false;
	}
	else
	{
		PointNode *pop = new PointNode;
		pop = this->pTop;

		NextPoint = pop->PN_pt;

		this->pTop = this->pTop->pNext;
		delete pop;
		pop = NULL;

		return true;
	}
}

void PacStack::ClearStack()
{
	if (IsEmpty())
		return;

	PointNode *p= this->pTop;
	PointNode *q = NULL;

	while (p != this->pTop)
	{
		q = p->pNext;
		delete p;
		p = q;
	}
	this->pTop = this->pButtom;

	return;
}

//End

//PacSearch
/*
	��ȡʱ����������,һ��openһ��closed
	����ȡ������λ�÷���open����,���жϺ�������λ�÷���
	closed����,open�����ж��л�����С·���������,
	close���Ѷ�ȡ���õ�
	��ȡʱ���жϸõ��Ƿ���closed����
*/
static PacList *Open = NULL;
static PacList *Closed = NULL;

void ClearList(PacList* list)
{
    PacList *temp = list->pNext;
    PacNode *pNode;

    if (temp == NULL)
        return;

    while (temp != NULL)
    {
        PacList* head = temp;
        temp = temp->pNext;
        pNode = head->L_pt;

        delete pNode;
        delete head;
        list->pNext = temp;
    }
}

void ClearOpenList()
{
    ClearList(Open);
}

void ClearCloseList()
{
    ClearList(Closed);
}

static PacSearch* psearch = NULL;

PacSearch::PacSearch()
{
}

PacSearch::~PacSearch()
{
}

PacSearch * PacSearch::InitSearch()
{
	if (psearch == NULL)
	{
		psearch = new PacSearch;
	}
	else
	{
		delete psearch;
		psearch = new PacSearch;
	}
	return psearch;
}

void PacSearch::Destroy()
{
	if (psearch != NULL)
	{
		delete psearch;
		psearch = NULL;
	}
}

PacNode * PacSearch::GetNode() //��ȡ������Сֵ�ڵ�
{
	PacList *minFValue = Open->pNext;
	PacList *temp = Open->pNext;
	PacList *NextMin = Open;
	PacNode *MinNode;

	if (temp == NULL)
		return NULL;

	while (temp->pNext != NULL)//һֱ�Ƚϵõ���Сֵ
	{
		if ((temp->pNext->L_pt->f) < (minFValue->L_pt->f))
		{
			minFValue = temp->pNext;
			NextMin = temp;
		}
		temp = temp->pNext;
	}

	MinNode = minFValue->L_pt;
	temp = NextMin->pNext;
	NextMin->pNext = NextMin->pNext->pNext;

	delete temp;
	
	return MinNode;
}

void PacSearch::PushOpen(PacNode *node)
{
	PacList *temp;
	
	temp = new PacList;
	temp->L_pt = node;

	temp->pNext = Open->pNext;
	Open->pNext = temp;
}

void PacSearch::PushClose(PacNode *node)
{
	PacList *temp;

	temp = new PacList;
	temp->L_pt = node;

	temp->pNext = Closed->pNext;
	Closed->pNext = temp;
}

int PacSearch::GetHValue(int frow, int fcol)
{
	int x = abs(m_iDesRow - frow)+abs(m_iDesCol-fcol); //�����پ���
	return x*AVGVALUE; //
}

void PacSearch::SetDesRow(int frow)
{
	m_iDesRow = frow;
}

void PacSearch::SetDesCol(int fcol)
{
	m_iDesCol = fcol;
}

int PacSearch::GetDesRow() const
{
	return m_iDesCol;
}

int PacSearch::GetDesCol() const
{
	return m_iDesRow;
}

bool PacSearch::IsBarrier(int frow, int fcol)//�ж����������Ƿ���ǽ����
{
	if (map_s[frow][fcol] == BARRIER)
		return false;
	//��������������������
    if(fcol < 0 || fcol >= MAPLENTH)
        return false;
    if(frow < 0 || frow >= MAPLENTH)
        return false;
	return true;
}

PacNode * PacSearch::FindOpen(int frow, int fcol)//��λ���Ƿ���open����
{
	PacList *temp = Open->pNext;

	if (temp == NULL)
		return NULL;

	while (temp != NULL)
	{
		if ((temp->L_pt->row == frow) && (temp->L_pt->col == fcol))
		{
			return temp->L_pt;
		}
		else
		{
			temp = temp->pNext;
		}
	}

	return NULL;
}

bool PacSearch::FindClose(int frow, int fcol)//λ���Ƿ���close����
{
	PacList *temp = Closed->pNext;
	
	if (temp == NULL)
		return false;

	while (temp != NULL)
	{
		if ((temp->L_pt->row == frow) && (temp->L_pt->col == fcol))
		{
			return true;
		}
		else
		{
			temp = temp->pNext;
		}
	}

	return false;
}

void PacSearch::CreateNodeArea(PacNode *MinNode, int frow, int fcol, int GValue)
{
	int g = MinNode->g + GValue;

	if (!FindClose(frow, fcol))
	{
		PacNode *OldNode = FindOpen(frow, fcol);
		if (OldNode != NULL)
		{
			if (OldNode->g > g)
			{
				OldNode->dir.c_row = MinNode->row;
				OldNode->dir.c_col = MinNode->col;
				OldNode->parent = MinNode;
				OldNode->g = g;
				OldNode->f = OldNode->h + OldNode->g;
			}
		}
		else
		{
			PacNode *ptNode = new PacNode;
			ptNode->parent = MinNode;
			ptNode->h = GetHValue(frow, fcol);
			ptNode->g = g;
			ptNode->f = ptNode->h + ptNode->g;
			ptNode->row = frow;
			ptNode->col = fcol;
			ptNode->dir.c_row = MinNode->row;
			ptNode->dir.c_col = MinNode->col;

			PushOpen(ptNode);
		}
	}
	return;
}

//����Ҫ�ĺ���:�����ܵĵ��жϷ�����
void PacSearch::AreaNode(PacNode *MinNode)
{
	int frow = MinNode->row;
	int fcol = MinNode->col;

	if (IsBarrier(frow-1, fcol))//UP
	{
		CreateNodeArea(MinNode, frow-1, fcol, AVGVALUE);
	}
	if (IsBarrier(frow+1, fcol))//DOWN
	{
		CreateNodeArea(MinNode, frow+1, fcol, AVGVALUE);
	}
	if (IsBarrier(frow, fcol-1))//LEFT
	{
		CreateNodeArea(MinNode, frow, fcol-1, AVGVALUE);
	}
	if (IsBarrier(frow, fcol+1))//RIGHT
	{
		CreateNodeArea(MinNode, frow, fcol+1, AVGVALUE);
	}

	PushClose(MinNode);
}

/*
������fֵ��С��·��,���������ѹ��ջ��, ��ջ�ṹ��ӦΪ������Ŀ����ֱ�ӷ���,
�൱�ڴ�Ŀ��������߻���ʼ��,����ջ��Ԫ�ؾ�����һ�����ߵ�λ��
*/

void PacSearch::MinPath(bool barrier[MAPLENTH][MAPLENTH], int fStartRow, int fStartCol, int fDesRow, int fDesCol, PacStack &GreenStack)
{
	PacNode *start = new PacNode;
	PacNode *MinNode = NULL;
	PacPoint point;
	PacPoint temp;

	//��ʼ��������ͼ
	for (int frow=0; frow<MAPLENTH; frow++)
	{
		for (int fcol=0; fcol<MAPLENTH; fcol++)
		{
			if (barrier[frow][fcol])
				map_s[frow][fcol] = PATH;
			else
				map_s[frow][fcol] = BARRIER;
		}
	}
	//�����ʼ���յ�λ��
	map_s[fStartRow][fStartCol] = STARTPOS;
	map_s[fDesRow][fDesCol] = DESPOS;

	//�趨Ŀ���
	SetDesRow(fDesRow);
	SetDesCol(fDesCol);

	//��ʼ�����
	start->parent = NULL;
	start->row = fStartRow;
	start->col = fStartCol;
	start->g = 0;
	start->h = GetHValue(start->row, start->col);
	start->f = start->g + start->h;
	start->dir.c_row = start->row;
	start->dir.c_col = start->col;

	PushOpen(start);

	while (true)
	{
		//�൱�����ڹ������, �������ж�, ��λ�ú�����λ�õ�Ȩ��ʹ��Ч���Ը�

		MinNode = GetNode();
		if (MinNode == NULL)
			break;
		else if ((MinNode->row == GetDesRow()) && (MinNode->col == GetDesCol()))
		{
			map_s[fDesRow][fDesCol] = PATH;
			
			point.c_row = MinNode->row;
			point.c_col = MinNode->col;
			GreenStack.PushStack(point);
			//pt����һ��λ�ñ��
			while (!(MinNode->dir.c_row == fStartRow && MinNode->dir.c_col == fStartCol))
			{
				map_s[MinNode->dir.c_row][MinNode->dir.c_col] = MINPATH;
				
				//·���ڵ���ջ
				temp.c_row = MinNode->dir.c_row;
				temp.c_col = MinNode->dir.c_col;
				GreenStack.PushStack(temp);

				MinNode = MinNode->parent;
				point.c_row = MinNode->row;
				point.c_col = MinNode->col;
			}
			break;
		}
		AreaNode(MinNode);
	}
}

//��װ���ṩ�Ľӿں���
bool PacSearch::SEARCH(bool barrier[MAPLENTH][MAPLENTH], int fStartRow, int fStartCol, int fDesRow, int fDesCol, PacStack &GreenStack)
{
	PacSearch *pS = PacSearch::InitSearch();

	Open = new PacList;
	Open->pNext = NULL;
	Closed = new PacList;
	Closed->pNext = NULL;

	MinPath(barrier, fStartRow, fStartCol, fDesRow, fDesCol, GreenStack);

	ClearOpenList();
	ClearCloseList();
	delete Open;
	delete Closed;

	return true;;
}
//End