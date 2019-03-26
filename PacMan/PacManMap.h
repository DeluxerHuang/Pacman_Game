#ifndef PACMANMAP_H
#define PACMANMAP_H

#include "stdafx.h"

/*
	��ͼ��С����19��λ����19*19��ά����
	Pacman��������Ϊ(9, 10)
	Enemy��������Ϊ(9, 8)\
	R-ROW - Y
	C-COL - X
*/

#define MAPLENTH 19
#define PACSHOW_R 10 
#define PACSHOW_C 9
#define ENESHOW_R 8
#define ENESHOW_C 9

//������ͼ��: ��ͼ�ߴ�, ��ͼ�ϰ���, Pea�ֲ�
class PacMap
{
	friend class PacMove;
	friend class PacMan;
public:
	void DrawMap(HDC);
	void DrawPea(HDC);

	PacMap(){}
	virtual ~PacMap(){}

	bool BarrierMap[MAPLENTH][MAPLENTH]; //�ϰ����ͼҪ�������������,���Ա��public..���ٸ���һ�ݵ�ͼ��...
protected:
	void ShowPosition();//PacMan Enemy����λ��

	//��ͼ���
	bool PeaMap[MAPLENTH][MAPLENTH];

protected:
	static int iBarrierSize; //�ϰ����С
	static int iPeaSize;	 //���Ӵ�С
	COLORREF color;
};

class Stage : public PacMap
{
public:
	Stage(int i=1); //��ʼ�ؿ�����Ϊ1

	bool InitialMap();
	void StageWin();
	void InitColor();
	unsigned int GetStage();
	bool InitMap[MAPLENTH][MAPLENTH]; //�ļ�����ȡ

private:
	unsigned int m_iSta;
	
};

#endif
