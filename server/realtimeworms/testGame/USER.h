#pragma once
class USER
{
private:

public:
	int uid;
	int hp;
	int state; //�غ�Ǿ��� �ȵǾ��� �����ִ� ����???
	float x;
	float y;
	char *Buffer;
	USER();
	USER(int, int, float, float);
	void setUSER(int, float, float);
	~USER();
};

