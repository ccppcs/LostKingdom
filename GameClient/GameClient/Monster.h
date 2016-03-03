#pragma once
#include "Character.h"

class Monster : public CCharacter
{
protected:
	int goldup;
	int expup;


public:
	Monster(void);
	~Monster(void);
};

