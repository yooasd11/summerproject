#pragma once
class USER
{
public:
	int uid; //���Ϲ�ȣ
	int hp;
	float x;
	float y;
	int state; //WAITNG = 1, MOVE = 2, ATTACK = 3;
	//���� �����Ϳ� ������� ���� ������ ũ��...
	float velocity;
	int total;
	int current;
	char Buffer[BUFSIZE];
	Lock *key;

	USER();
	USER(int, int, float, float);
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
	void packetHandle();

	//��ť�� ���� �۾�����...
	void UserMove();
};

