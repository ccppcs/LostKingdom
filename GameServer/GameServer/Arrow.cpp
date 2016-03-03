#include "StdAfx.h"
#include "Arrow.h"


Arrow::Arrow(void)
{
}
Arrow::Arrow(BOOL direction, CPoint point,int act)
{
	a_direction=direction;
	a_point=point;
	this->act=act;
}


void Arrow::Start()
{
	switch(a_direction)
	{
	case 0 :
		a_point.y--;
		break;
	case 1 :
		a_point.y++;
		break;
	case 2 :
		a_point.x--;
		break;
	case 3:
		a_point.x++;
		break;
	}
}

Arrow::~Arrow(void)
{
}
