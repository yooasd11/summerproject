#pragma once
class USER
{
private:
public:
	int uid; //소켓번호
	int hp;
	float x;
	float y;
	enum state{
		WAITING =1,
		MOVE,
		ATTACK,
	};
	//받은 데이터와 현재까지 받은 데이터 크기...
	int total;
	int current;
	char Buffer[BUFSIZE];

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
};

