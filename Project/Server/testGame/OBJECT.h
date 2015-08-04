#pragma once
class OBJECT
{
private:
public:
	int type;
	int ObjectId;
	int hp;
	float x;
	float y;
	float vx;
	float vy;
	virtual void CALCULATE_DISTANCE();
	OBJECT();
	~OBJECT();
};

