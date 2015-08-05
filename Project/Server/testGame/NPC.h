#pragma once
class NPC_STATE;
class NPC_DEAD;
class NPC_ALIVE;

class NPC : public OBJECT
{
private:
	Lock key;
	
public:
	NPC_STATE *current_state;
	NPC_DEAD *npc_dead;
	NPC_ALIVE *npc_alive;
	NPC();
	NPC(float, float, float, float);
	~NPC();
	void NPC_DIRECTION_CHANGE();
	void NPC_SET_HP(int);
	void NPC_STATE_CHANGE(int);
	void NPC_INIT();
	void NPC_DESICION();
};

