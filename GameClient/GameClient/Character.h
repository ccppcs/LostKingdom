#pragma once

//방향
#define EAST 0  // 동쪽
#define WEST 1  // 서쪽
#define SOUTH 2  // 남쪽
#define NORTH 3  // 북쪽

//상태
#define S_NONE 0
#define S_MOVE 1 // 이동중
#define S_ATTACK 2// 공격중
#define S_DIE -1// 죽음(미사용)


class CCharacter
{
protected:
	CPoint point;	//케릭터 좌표
	int range;		//사정거리
	int job;	//직업,몬스터 종류
	int MAX_hp;	//최대체력
	int MAX_mp; //최대마력
	int hp;		//체력
	int mp;		//마력
	int act;	//공격력
	int def;	//방어력
	BOOL Attack_Type; //원거리,근접 공격타입 선택
	BOOL Flag_direction; // 케릭터 방향 플래그
	BOOL Flag_status;  // 캐릭터 상태 플래그

	int Frame;			// 애니메이션 프레임
	
public:
	CCharacter(void);
	~CCharacter(void);
	void FindTarget(void); 
	void Hit(int damage);
	void Attack(int direction);

	void SetDirection(int direction);
	void SetPoint(CPoint point);
	void SetHP(int hp);
	void SetMP(int mp);
	void SetAct(int act);
	void SetDef(int def);
	void SetMAXHP(int MAX_hp);
	void SetMAXMP(int MAX_mp);
	void SetJob(int job);
	void SetStatusFlag(BOOL flag);
	void SetFRAME(int Frame);

	CPoint GetPoint() const;
	int GetHP() const;
	int GetMP() const;
	int GetAct() const;
	int GetDef() const;
	int GetMAXHP() const;
	int GetMAXMP() const;
	int GetJob() const;
	BOOL GetDirection() const;
	BOOL GetStatusFlag() const;
	int GetFRAME() const;
};

