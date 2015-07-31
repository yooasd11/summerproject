#pragma once
class USER
{
public:
	bool connect;
	int uid; //���Ϲ�ȣ
	int objectID;
	int hp;
	float x;
	float y;
	float direction;
	float velocity;
	enum state{
		WAITING = 1,
		MOVING,
		DEAD,
	};
	enum state crt; //WAITNG = 1, MOVE = 2, ATTACK = 3, DEAD = 4;
	//���� �����Ϳ� ������� ���� ������ ũ��
	
	int total;
	int current;
	char Buffer[BUFSIZE];
	Lock *key;

	USER();
	USER(int, SOCKET, int, float, float);
	void setUSER(int, float, float);
	~USER();
	void setTotal(int);
	void setCurrent(int);
	void setBuffer(char*, int);
	char* getBuffer();
	int getTotal();
	int getCurrent();
	bool finished();
	void clear();
	void UserpacketHandle();
	void UserpacketHandle(char*, int, int);
	bool isConnecting();
	void ChangeState(USER::state);

	//���¿� ���� ����
	void Set_user_hp(int);

	//��ť�� ���� �۾�����...
	void UserMove();
};

