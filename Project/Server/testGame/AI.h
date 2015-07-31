#pragma once

class NPC_STATE;
class NPC_DEAD;
class NPC_ALIVE;

class AI
{
private:
	Lock* key;
public:
	int nid;
	int hp;
	float x;
	float y;
	float direction;
	float velocity;
	NPC_STATE *current_state;
	NPC_DEAD *npc_dead;
	NPC_ALIVE *npc_alive;

	AI();
	AI(int,int, float, float, float, float);
	~AI();
	float getDegree(float, float, float, float);
	void Action();
	void Wait();

	void ChangeState(int);
	void ChangeDirection();

	void init();
	void decision();
	void AI_Change_Direction();
	void AI_set_hp(int);
};



