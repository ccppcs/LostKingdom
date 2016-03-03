#include "StdAfx.h"
#include "Character.h"


CCharacter::CCharacter(void)
{
}


CCharacter::~CCharacter(void)
{
}


void CCharacter::Hit(int damage)
{
	hp-=damage;
}


void CCharacter::Attack(int direction) 
{
	this->SetStatusFlag(S_ATTACK);
}
////////////////////////////////
//			Set
///////////////////////////////
void CCharacter::SetDirection(int direction)
{
	Flag_direction=direction;
}
void CCharacter::FindTarget(void)
{

}
void  CCharacter::SetPoint(CPoint point)
{
	this->point = point;
}
void CCharacter::SetHP(int hp)
{
	this->hp = hp;
}
void CCharacter::SetMP(int mp)
{
	this->mp = mp;
}
void CCharacter::SetAct(int act)
{
	this->act = act;
}
void CCharacter::SetDef(int def)
{
	this->def = def;
}
void CCharacter::SetMAXHP(int MAX_hp)
{
	this->MAX_hp = MAX_hp;
}
void CCharacter::SetMAXMP(int MAX_mp)
{
	this->MAX_mp = MAX_mp;
}
void CCharacter::SetJob(int job)
{
	this->job=job;
}
void CCharacter::SetStatusFlag(BOOL flag)
{
	this->Flag_status=flag;
}
void CCharacter::SetFRAME(int Frame)
{
	this->Frame = Frame;
}
////////////////////////////////
//			Get
///////////////////////////////
CPoint  CCharacter::GetPoint() const
{
	return point;
}
int CCharacter::GetHP() const
{
	return hp;
}
int CCharacter::GetMP() const
{
	return mp;
}
int CCharacter::GetAct() const
{
	return act;
}
int CCharacter::GetDef() const
{
	return def;
}

int CCharacter::GetMAXHP() const
{
	return MAX_hp;
}
int CCharacter::GetMAXMP() const
{
	return MAX_mp;
}
int CCharacter::GetJob() const
{
	return job;
}
BOOL CCharacter::GetDirection() const
{
	return Flag_direction;
}
BOOL CCharacter::GetStatusFlag() const
{
	return Flag_status;
}
int CCharacter::GetFRAME() const
{
	return Frame;
}