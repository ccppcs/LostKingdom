#pragma once

class Skill
{
public:
	Skill();
	~Skill(void);

protected:
	int MaxFrame;
	int Frame;
	UINT resource;
	CPoint point;
	BOOL use;
public:	
	int GetMaxFrame(void) const;
	UINT GetResourceID(void) const;
	CPoint GetPoint(void) const;
	void SetInfo(int MaxFrame, UINT res,CPoint point);
	void SetUse(BOOL use);
	int GetUse(void) const;
	int GetFrame(void) const;
	void SetFrame(int Frame);
	void Init(void);
};
