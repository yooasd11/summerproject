#pragma once
class ObjectManager
{
private:
	Lock *key;
public:
	int ObjectCount;
	std::map < int, std::shared_ptr<CollisionObject>> mappingObject;
	void registObject(std::shared_ptr<CollisionObject>);
	std::shared_ptr<CollisionObject> retObject(int);
	void removeObject(int);


	ObjectManager();
	~ObjectManager();
};

