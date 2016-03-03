#pragma once
#include "Character.h"

class Arrow : public CCharacter
{
protected:
	BOOL a_direction;	//�߻�ü ����
	CPoint a_point;		//�߻�ü ��ǥ
	int act;
	
public:
	Arrow(void);
	Arrow(BOOL direction, CPoint point,int act);
	void Start();
	~Arrow(void);
};

