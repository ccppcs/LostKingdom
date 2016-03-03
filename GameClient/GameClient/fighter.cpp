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
void fighter::UseSkill(int num)
{
	switch(num)
	{
		case 0 :
			if(mp<10){
				AfxMessageBox(_T("���� ����!"));return;	}
			AfxMessageBox(_T("1����ų ���!"));
			mp -= 10;break;
		case 1 :
			if(mp<20){
				AfxMessageBox(_T("���� ����!"));return;	}
			AfxMessageBox(_T("2����ų ���!"));
			mp -= 20;
			break;
		case 2 :
			if(mp<30){
				AfxMessageBox(_T("���� ����!"));return;		}
			AfxMessageBox(_T("3����ų ���!"));
			mp -= 30;
			break;
	}
}
void fighter::Acc()
{
	hp -=50;
	if(hp<=0)
		AfxMessageBox(_T("������!"));
}
void fighter::UseMpPotion()
{
	if(gold<100)
	{
		AfxMessageBox(_T("���� �����մϴ�!"));
		return;
	}
	if(hp==MAX_hp)
	{
		AfxMessageBox(_T("�ִ� �����Դϴ�!"));
		return;
	}
	gold -= 100;
	if(MAX_mp-mp<30)
		mp = MAX_mp;
	mp +=30;
}
void fighter::UseHpPotion()
{
	if(gold<100)
	{
		AfxMessageBox(_T("���� �����մϴ�!"));
		return;
	}
	if(hp==MAX_hp)
	{
		AfxMessageBox(_T("�ִ� ü���Դϴ�!"));
		return;
	}
	gold -= 100;
	if(MAX_hp-hp<50)
		hp = MAX_hp;
	hp +=50;
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

void fighter::StatusUp(int status)
{
	if(gold<100)
	{
		AfxMessageBox(_T("������!"));return;
	}
	gold-=100;
	switch(status)
	{
	case 0 :
		act+=3;
		break;
	case 1 :
		def+=2;
		break;
	case 2 :
		MAX_hp+=50;
		hp+=50;
		break;
	case 3 :
		MAX_mp+=30;
		mp+=30;
		break;
	}
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

void fighter::SkillUp(int num)
{
	if(Skill_point<1)
	{
		AfxMessageBox(_T("��������!"));return;
	}
	Skill_point-=1;
	switch(num)
	{
	case 0 :
		skill1 +=0.5;break;
	case 1 :
		skill2 +=1;break;
	case 2 :
		skill3 +=1.5;break;
	}
}


int fighter::GetMAXExp()
{
	return MAX_exp;
}
/******���ݰ���ġ*******/
int fighter::GetExp()
{
	return exp;
}
/*******��ų����Ʈ******/
int fighter::GetSkillPoint()
{
	return Skill_point;
}
/******�ɸ��� ����*******/
int fighter::GetLevel() const
{
	return level;
}
/******1~3����ų ������*******/
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
/******HP�����˾Ƴ���*******/
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
	if(job)		// �ü�
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
	else		// ����
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
