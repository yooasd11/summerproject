#pragma once
class USER
{
public:
	int uid; //소켓번호
	int hp;
	float x;
	float y;
	int state; //WAITNG = 1, MOVE = 2, ATTACK = 3;
	//받은 데이터와 현재까지 받은 데이터 크기...
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

	//잡큐에 관한 작업정의...
	void UserMove();
};

