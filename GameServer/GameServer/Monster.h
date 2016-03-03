#pragma once
#include "Character.h"

class Monster : public CCharacter
{
protected:
	CPoint prevPoint;

public:
	Monster(void);
	~Monster(void);

	void Init();
	int GetGold();
	int GetExp();
	void SetMonster(int monster);
	CPoint GetLength(CPoint point);
	void SetPrevPoint(CPoint point);
	CPoint GetPrevPoint(void) const;
	void Move(int direction);
};

