#pragma once
class AI
{
private:
	Lock* key;
public:
	enum state{
		alive = 1,
		dead,
		Attack,
		Waiting,
	};
	enum state current;
	int nid;
	int hp;
	float x;
	float y;
	float direction;
	float velocity;

	AI();
	AI(int,int, float, float, float, float);
	~AI();
	float getDegree(float, float, float, float);
	void Action();
	void Wait();
	void ChageState(AI::state);
};

