#pragma once
#include "Character.h"

class Tower : public CCharacter
{
protected:

public:
	Tower(void);
	Tower(CPoint t_point,int MAX_hp,int act, int def);
	int FindTarget(CPoint c_point);
	~Tower(void);
};

