#pragma once
#include "Character.h"

class fighter : public CCharacter
{
protected :
	int level;
	int MAX_exp;
	int exp;
	int Skill_point;
	int gold;
	double skill1;
	double skill2;
	double skill3;

public:



	fighter(void);
	fighter(int hp_,int gold_,int maxhp);
	~fighter(void);
	void SetInfo(int hp_,int gold_,int maxhp,int maxmp,int mp_);
	void UseHpPotion();
	void UseMpPotion();
	void StatusUp(int num);
	void SkillUp(int num);
	void InstallTower();
	void UseSkill(int num);
	void LevelUp();
	void Acc();

	void SetGold(int gold);
	void SetMAXExp(int MAXExp);
	void SetExp(int exp);
	void SetSkillPoint(int sp);
	void SetLevel(int level);
	void SetSkill1(int skill_level);
	void SetSkill2(int skill_level);
	void SetSkill3(int skill_level);

	int GetGold();
	int GetMAXExp();
	int GetExp();
	int GetSkillPoint();
	int GetLevel() const;
	int GetSkill1();
	int GetSkill2();
	int GetSkill3();
	double GetHpRate();
	double GetMpRate();

	void InitData(int job,int ConnectedNumber);
};

