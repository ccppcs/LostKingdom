#include "StdAfx.h"
#include "fighter.h"


fighter::fighter(void)
{
}

fighter::fighter(int hp_,int gold_,int maxhp)
{
	MAX_hp = maxhp;
	hp = hp_;
	gold = gold_;

}

fighter::~fighter(void)
{
}
void fighter::UseSkill(int select)
{
	switch(select)
	{
	case 1 : mp-=100;break;
	case 2 : mp-=150;break;
	case 3 : mp-=200;break;
	}
}

void fighter::SetInfo(int hp_,int gold_,int maxhp,int maxmp,int mp_)
{
	MAX_mp=maxmp;
	mp= mp_;
	MAX_hp= maxhp;
	hp= hp_;
	gold= gold_;
}
	void fighter::SetGold(int gold)
	{
		this->gold = gold;
	}
	void fighter::SetMAXExp(int MAXExp)
	{
		this->MAX_exp=MAXExp;
	}
	void fighter::SetExp(int exp)
	{
		this->exp = exp;
	}
	void fighter::SetSkillPoint(int sp)
	{
		this->Skill_point = sp;
	}
	void fighter::SetLevel(int level)
	{
		this->level=level;
	}
	void fighter::SetSkill1(int skill_level)
	{
		this->skill1 = skill_level;
	}
	void fighter::SetSkill2(int skill_level)
	{
		this->skill2 = skill_level;
	}
	void fighter::SetSkill3(int skill_level)
	{
		this->skill3 = skill_level;
	}
void fighter::InstallTower()
{

}


void fighter::LevelUp()
{
	if(job==0)
	{
		level++;
		exp=0;
		MAX_hp +=30;
		MAX_mp +=5;
		hp=MAX_hp;
		mp=MAX_mp;
		act++;
		def+=2;
		MAX_exp+=level*100;
		Skill_point++;
	}
	else if(job==1)
	{
		level++;
		exp=0;
		MAX_hp +=20;
		MAX_mp +=15;
		hp=MAX_hp;
		mp=MAX_mp;
		act+=2;
		def++;
		MAX_exp+=level*100;
		Skill_point++;
	}
}


int fighter::GetMAXExp()
{
	return MAX_exp;
}
/******지금경험치*******/
int fighter::GetExp()
{
	return exp;
}
/*******스킬포인트******/
int fighter::GetSkillPoint()
{
	return Skill_point;
}
/******케릭터 레벨*******/
int fighter::GetLevel() const
{
	return level;
}
/******1~3번스킬 레벨값*******/
int fighter::GetSkill1()
{
	return (int)skill1;
}

int fighter::GetSkill2()
{
	return (int)skill2;
}

int fighter::GetSkill3()
{
	return (int)skill3;
}
/******HP비율알아내기*******/
double fighter::GetHpRate() 
{
	return ((double)hp/(double)MAX_hp);
}
double fighter::GetMpRate()
{
	return ((double)mp/(double)MAX_mp);
}
int fighter::GetGold()
{
	return gold;
}
void fighter::InitData(int job,int ConnectedNumber)
{
	this->job = job;
	if(job)		// 궁수
	{
		this->SetAct(100);
		this->SetDef(100);
		this->SetHP(1000);
		this->SetMP(1000);
		this->SetMAXHP(1000);
		this->SetMAXMP(1000);
		this->SetGold(1000);
		this->SetMAXExp(1000);
		this->SetExp(900);
		this->SetLevel(1);
		this->SetSkillPoint(5);
		this->SetSkill1(1);
		this->SetSkill2(1);
		this->SetSkill3(1);
	}
	else		// 전사
	{
		this->SetAct(100);
		this->SetDef(100);
		this->SetHP(1000);
		this->SetMP(1000);
		this->SetMAXHP(1000);
		this->SetMAXMP(1000);
		this->SetGold(1000);
		this->SetMAXExp(1000);
		this->SetExp(900);
		this->SetLevel(1);
		this->SetSkillPoint(5);
		this->SetSkill1(1);
		this->SetSkill2(1);
		this->SetSkill3(1);
		
	}
	this->SetStatusFlag(S_NONE);
	this->SetDirection(EAST);
	this->SetPoint(CPoint(ConnectedNumber,2));
}



void fighter::UsePotion(int num)
{
	switch(num)
	{
	case 0 :
		gold -=100;
		if(MAX_hp-hp<50)
		{
			hp = MAX_hp;break;
		}
		hp += 50;break;
	case 1 :
		gold -=100;
		if(MAX_mp-mp<30)
		{
			mp = MAX_mp;break;
		}
		mp += 30;break;
	}
}

void fighter::StatusUp(int num)
{
	gold -=300;
	switch(num)
	{
	case 0:
		act++;
		break;
	case 1:
		def++;
		break;
	case 2:
		MAX_hp +=50;
		break;
	case 3:
		MAX_mp +=30;
		break;
	}
}


void fighter::SkillUp(int num)
{
	
	Skill_point-=1;
	switch(num)
	{
	case 0 :
		skill1 ++;break;
	case 1 :
		skill2 ++;break;
	case 2 :
		skill3 ++;break;
	}
}
void fighter::Init(void)
{
	level=0;
	MAX_exp=0;
	exp=0;
	Skill_point=0;
	gold=0;
	skill1=0;
	skill2=0;
	skill3=0;
	point=CPoint(-1,-1);
	range=0;
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
}
void fighter::DefeatMonster(int gold,int exp)
{
	this->SetGold(this->GetGold()+gold);
	this->SetExp(this->GetExp()+exp);
}
int fighter::GetSkillDamage(int skill)
{
	int damage=100;
	switch(skill)
	{
	case 1:
		damage=this->GetSkill1()*120;
		break;
	case 2:
		damage=this->GetSkill2()*150;
		break;
	case 3:
		damage=this->GetSkill3()*200;
		break;
	}
	return damage;
}
