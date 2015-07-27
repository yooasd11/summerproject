#pragma once
class CollisionObject
{
private:
	Lock* key;
public:
	int objectid;
	float x;
	float y;
	enum type{
		USER=1,
		MAP,
	};
	enum type Type;
	int uid;

	CollisionObject();
	~CollisionObject();
};

