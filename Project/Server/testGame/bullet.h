#pragma once
class BULLET : public OBJECT
{
private:
	Lock key;
	
public:
	int shooter;
	int damage;
	float ax;
	float ay;
	enum state{
		ABLE = 1,
		DISABLE,
	};
	enum state CurrentState;
	void BULLET_MOVE();
	void CHANGE_STATE(int);
	BULLET();
	BULLET(float, float, float, float, int);
	~BULLET();
};

