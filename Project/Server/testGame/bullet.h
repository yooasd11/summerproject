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
	//동작하고 있냐 없냐...
	bool working;

	bullet();
	bullet(int, float, float, float, float, float);
	~bullet(); 
	void bulletMove();
};

