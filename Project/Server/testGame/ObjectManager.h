#pragma once
class ObjectManager
{
private:
public:
	Lock key;
	static int OBJECTCOUNT;
	std::map<int, std::shared_ptr<OBJECT>> OBJECT_MAP;
	void REGIST(int, int);
	void REGIST_BULLET(std::shared_ptr<BULLET>);
	void REGIST_NPC(std::shared_ptr<NPC>);
	std::shared_ptr<OBJECT> FIND(int);
	void REMOVE(int);
	int FIND_USER(int);
	void REMOVE_JOB(int);

	ObjectManager();
	~ObjectManager();
};

