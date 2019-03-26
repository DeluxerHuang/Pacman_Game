#ifndef PACSEARCH
#define PACSEARCH

#include "stdafx.h"
#include "PacManMap.h"

#define AVGVALUE 10 //����ʱ��Ȩ��ֵ
//��ͼ���
#define BARRIER  1
#define PATH  0
#define STARTPOS 2
#define DESPOS   3
#define MINPATH  4

struct PacPoint 
{
	int c_row;
	int c_col;
};

struct PacNode //��������ʱ��¼�Ľڵ�
{
	int g;
	int h;
	int f; //f = g + h;
	int row;
	int col;
	PacPoint dir;//ָ����λ��
	PacNode *parent;
};

struct PacList //���ֵ�ı� ��open and close
{
	PacNode *L_pt;
	PacList *pNext;
};

struct PointNode //��ջ�ڵ�
{
	PacPoint PN_pt;
	PointNode *pNext;
};

class PacStack //������ȡ���λ�õ�ջ�ṹ
{
public:
	void InitStack();
	void PushStack(PacPoint);
	bool PopStack(PacPoint &);
	bool IsEmpty() const; 
	void ClearStack();

protected:
	PointNode *pTop;
	PointNode *pButtom;
};

class PacSearch
{
public:
	PacSearch();
	~PacSearch();
	static PacSearch *InitSearch();
	static void Destroy();
	PacNode *GetNode();
	void PushOpen(PacNode *); //���ձ�
	PacNode *FindOpen(int, int);
	void PushClose(PacNode *); //���Ѷ�ȡ��, �Ѿ��ų���λ��
	bool FindClose(int, int);
	bool IsBarrier(int, int);
	int GetHValue(int, int);
	int GetDesRow() const;
	int GetDesCol() const;
	void SetDesRow(int);
	void SetDesCol(int);
	void CreateNodeArea(PacNode *, int, int, int);//���ڽ�����Χ�½ڵ�
	void AreaNode(PacNode *); //�ڻ��ڽ����õĽڵ��������

	void MinPath(bool [MAPLENTH][MAPLENTH], int, int, int, int, PacStack &); //Ѱ�����·��
	bool SEARCH(bool barrier[MAPLENTH][MAPLENTH], int StartRow, int StartCol, int DesRow, int DesCol, PacStack &GreenStack); //�ӿں���

private:
	int map_s[MAPLENTH][MAPLENTH];
	int m_iDesRow;
	int m_iDesCol;
};

#endif