#include "StdAfx.h"
#include "Tower.h"
#include "math.h"


Tower::Tower(void)
{
}
Tower::Tower(CPoint t_point,int MAX_hp,int act, int def)
{
	point=t_point;
	this->MAX_hp=MAX_hp;
	hp=MAX_hp;
	this->act=act;
	this->def=def;
}

int Tower::FindTarget(CPoint c_point)
{
	CPoint temp;
	temp.x=point.x-c_point.x;
	temp.y=point.y-c_point.y;

	if(temp.x<0)temp.x *= -1;
	if(temp.y<0)temp.y *= -1;
	return temp.x+temp.y;
}

Tower::~Tower(void)
{
}
