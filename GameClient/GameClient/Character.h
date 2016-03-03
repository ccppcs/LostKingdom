#pragma once

//����
#define EAST 0  // ����
#define WEST 1  // ����
#define SOUTH 2  // ����
#define NORTH 3  // ����

//����
#define S_NONE 0
#define S_MOVE 1 // �̵���
#define S_ATTACK 2// ������
#define S_DIE -1// ����(�̻��)


class CCharacter
{
protected:
	CPoint point;	//�ɸ��� ��ǥ
	int range;		//�����Ÿ�
	int job;	//����,���� ����
	int MAX_hp;	//�ִ�ü��
	int MAX_mp; //�ִ븶��
	int hp;		//ü��
	int mp;		//����
	int act;	//���ݷ�
	int def;	//����
	BOOL Attack_Type; //���Ÿ�,���� ����Ÿ�� ����
	BOOL Flag_direction; // �ɸ��� ���� �÷���
	BOOL Flag_status;  // ĳ���� ���� �÷���

	int Frame;			// �ִϸ��̼� ������
	
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

