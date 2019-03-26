#ifndef PACMANMOVE_H
#define PACMANMOVE_H

#include "stdafx.h"
#include "PacManMap.h"
#include "PacSearch.h"

#define PACSPEED 6 // PacMan �ٶ�
#define ENESPEED 4
//#define ENEMAXSP 5.5 //�����˷���PacMan����ٶ�
#define DISTANCE 10//ͼ�η�Χ
#define DRAWDIFF 2 //��ͼ
#define DRAWDIST 12 
#define REDESCAN 8
#define BLUESCAN 30 //��ɫ���˼��ӷ�Χ 8
//#define ENECRAZY 5
#define LEGCOUNT 10

enum ToWard {UP, DOWN, LEFT, RIGHT, CATCH};//����

class PacMove
{
public:
	int GetRow();
	int GetCol();

	void SetPosition(int, int);
	void DrawBlank(HDC);//���ƻ�����ͼ
	void virtual Draw(HDC hdc) = 0;//����PacMan Enemy,Ӧ�û�����Windows GDI
	void virtual MoveBehavior() = 0;//�൱�ڶ�̬�� ��ͬ�����������Ϊ��ͬ
	PacMove(int, int);

	static PacMap* pMapStage;//�����Ǿ�̬��Ա

protected:
	bool IsCollision(); //�ж��Ƿ���ײ
	bool IsCPoint();   //�ж��Ƿ񵽴�����λ��
	int TransPoint(int);//����ת��
	virtual void CenterPoint();//��������λ��

	int m_iX;
	int m_iY;
	int m_iRow;
	int m_iCol;
	int m_iSpeed;
	int m_iFrame;
	ToWard tw;
	ToWard twCmd;
	POINT pt;
};

//PacMan
class PacMan : public PacMove
{
public:
	PacMan(int, int);
	POINT GetPosition();
	ToWard GetToward();
	bool Win();
	void Draw(HDC);
	void SetTwCmd(ToWard);
	void Catch();
	void MoveBehavior();
	void SysPause();
	unsigned int GetScore();
protected:
	virtual void  CenterPoint();
	unsigned int m_uScore;
};

//Enemy
class Enemy : public PacMove
{
public:
	Enemy(int, int);
	void virtual Draw(HDC);
	void MoveBehavior();

	static PacMan* pacman;

protected:
	void Catch();//PacMan��ץ�ж�
	void virtual EnemyAI(bool) = 0;//��ͬ��ɫAI�̶Ȳ�ͬ

	COLORREF color;
};

class RedEne : public Enemy
{
public:
	RedEne(int, int);
	void Draw(HDC);

protected:
	void EnemyAI(bool);
};

class BlueEne : public RedEne
{
public:
	BlueEne(int, int);
	void Draw(HDC);

protected:
	void EnemyAI(bool);
};

//��ɫ���̶ܳ����..����
class GreenEne : public RedEne
{
public:
	GreenEne(int, int);
	void Draw(HDC);

	PacStack GreenStack;
	PacPoint NextPoint;
	PacSearch FindPac;

protected:
	void EnemyAI(bool);
};

#endif