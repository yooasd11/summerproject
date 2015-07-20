#pragma once
class USER
{
private:

public:
	int uid;
	int hp;
	int state; //준비되었나 안되었나 보여주는 상태???
	float x;
	float y;
	char *Buffer;
	USER();
	USER(int, int, float, float);
	void setUSER(int, float, float);
	~USER();
};

