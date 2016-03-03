#include "StdAfx.h"
#include "Skill.h"

Skill::Skill()
{
}
Skill::~Skill(void)
{
}
int Skill::GetMaxFrame(void) const
{
	return MaxFrame;
}

UINT Skill::GetResourceID(void) const
{
	return resource;
}

CPoint Skill::GetPoint(void) const
{
	return point;
}

void Skill::SetInfo(int MaxFrame, UINT res,CPoint point)
{
	this->MaxFrame = MaxFrame;
	this->resource = res;
	this->point = point;
	this->Frame=0;
	
}

void Skill::SetUse(BOOL use)
{
	this->use = use;
}

int Skill::GetUse(void) const
{
	return use;
}

int Skill::GetFrame(void) const
{
	return Frame;
}

void Skill::SetFrame(int Frame)
{
	this->Frame = Frame;
}

void Skill::Init(void)
{
	SetInfo(0,0,0);
	SetUse(FALSE);
}
