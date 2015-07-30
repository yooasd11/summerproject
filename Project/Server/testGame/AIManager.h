#pragma once
class AIManager
{
private:
	Lock *key;
public:
	std::map<int, std::shared_ptr<AI>> mappingAI;
	void registAI(std::shared_ptr<AI>);
	std::shared_ptr<AI> retAI(int);
	void removeAI(int);

	AIManager();
	~AIManager();
};


