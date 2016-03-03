#include "StdAfx.h"
#include "Monster.h"

#define M_SLIME 0
#define M_SKELETON 1
#define M_GOLEM 2
#define M_VAMPIRE 3
#define M_KNIGHT 4

Monster::Monster(void)
{
}

Monster::~Monster(void)
{
}
void Monster::Init()
{
	point=CPoint(-1,-1);
	prevPoint = point;
	range=1;
	job=-1;
	MAX_hp=0;
	MAX_mp=0;
	hp=0;
	mp=0;
	act=0;
	def=0;
	Attack_Type=-1;
	Flag_direction=0;
	Flag_status=-1;
	Frame=0;
}
int Monster::GetGold()
{
	int gold=0;
	switch (job)
	{
	case M_SLIME:
		gold=50;
		break;
	case M_SKELETON:
		gold=100;
		break;
	case M_GOLEM:
		gold=500;
		break;
	case M_VAMPIRE:
		gold=1000;
		break;
	case M_KNIGHT:
		gold=2000;
		break;
	}
	return gold;
}
int Monster::GetExp()
{
	int exp=0;
	switch (job)
	{
	case M_SLIME:
		exp=50;
		break;
	case M_SKELETON:
		exp=100;
		break;
	case M_GOLEM:
		exp=500;
		break;
	case M_VAMPIRE:
		exp=1000;
		break;
	case M_KNIGHT:
		exp=2000;
		break;
	}
	return exp;
}
void Monster::SetMonster(int monster)
{
	job = monster;
	MAX_mp = 0;
	mp=0;
	switch (monster)
	{
	case M_SLIME:
		MAX_hp = 500;
		hp=MAX_hp;	
		act=10;
		def=5;	
		Attack_Type=0;		
		Flag_direction=WEST; 
		Flag_status=S_NONE;  
		break;
	case M_SKELETON:
		MAX_hp = 700;
		hp=MAX_hp;	
		act=20;
		def=5;	
		Attack_Type=0;		
		Flag_direction=WEST; 
		Flag_status=S_NONE;  
		break;
	case M_GOLEM:
		MAX_hp = 2000;
		hp=MAX_hp;	
		act=500;
		def=80;	
		Attack_Type=0;		
		Flag_direction=WEST; 
		Flag_status=S_NONE;  
		break;
	case M_VAMPIRE:
		MAX_hp = 1000;
		hp=MAX_hp;	
		act=100;
		def=5;	
		Attack_Type=0;		
		Flag_direction=WEST; 
		Flag_status=S_NONE;  
		break;
	case M_KNIGHT:
		MAX_hp = 5000;
		hp=MAX_hp;	
		act=200;
		def=50;	
		Attack_Type=0;		
		Flag_direction=WEST; 
		Flag_status=S_NONE;
		break;
	}
}
CPoint Monster::GetLength(CPoint point)
{
	CPoint result;
	result = this->point-point;
	return result;
}

void Monster::SetPrevPoint(CPoint point)
{
	prevPoint = point;
}

CPoint Monster::GetPrevPoint(void) const
{
	return prevPoint;
}
void Monster::Move(int direction)
{
	this->prevPoint=point;
	switch(direction)
	{
	case EAST:
		this->point.x++;
		break;
	case WEST:
		this->point.x--;
		break;
	case SOUTH:
		this->point.y++;
		break;
	case NORTH:
		this->point.y--;
		break;
	}
	this->SetDirection(direction);
	this->SetStatusFlag(S_MOVE);
}