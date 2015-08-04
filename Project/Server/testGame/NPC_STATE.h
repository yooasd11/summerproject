#pragma once
class NPC_STATE
{
public:
	NPC_STATE();
	~NPC_STATE();
	virtual void NPC_Action(std::shared_ptr<NPC>);
};


class NPC_ALIVE : public NPC_STATE
{
public:
	NPC_ALIVE();
	~NPC_ALIVE();
	virtual void NPC_Action(std::shared_ptr<NPC>);
};


class NPC_DEAD : public NPC_STATE
{
public:
	NPC_DEAD();
	~NPC_DEAD();
	virtual void NPC_Action(std::shared_ptr<NPC>);
};

