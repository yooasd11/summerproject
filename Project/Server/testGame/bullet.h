#pragma once
class bullet
{
private:
	Lock* key;
public:
	int th;
	int uid;
	float x;
	float y;
	float damage;
	float velocity;
	float direction;
	//�����ϰ� �ֳ� ����...
	bool working;

	bullet();
	bullet(int, int, float, float, float, float,float);
	~bullet(); 
	void bulletMove();
};

