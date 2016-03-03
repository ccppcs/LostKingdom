#pragma once
#include "Character.h"

class Arrow : public CCharacter
{
protected:
	BOOL a_direction;	//발사체 방향
	CPoint a_point;		//발사체 좌표
	int act;
	
public:
	Arrow(void);
	Arrow(BOOL direction, CPoint point,int act);
	void Start();
	~Arrow(void);
};

