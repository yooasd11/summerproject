#ifndef MAIN_INCLUDE
#include "Packet.h"
#endif
#include <vector>

void EchoHandler(Packet& p);
void EchoListHandler(Packet& p);
void EchoCharacterHandler(Packet& p);

class HandlerMap{
private:
	void (*HandlerTable[P_MAX])(Packet&);
public:
	HandlerMap(){
		for (int i = 0; i < P_MAX; ++i) 
			HandlerTable[i] = nullptr;
		HandlerTable[P_ECHO] = EchoHandler;
		HandlerTable[P_ECHOLIST] = EchoListHandler;
		HandlerTable[P_ECHOCHARACTER] = EchoCharacterHandler;
	}
	~HandlerMap() {}
	void DeregisterFunction(int type){
		HandlerTable[type] = nullptr;
	}
	bool HandlePacket(Packet& p){
		int type = p.getType();
		if (HandlerTable[type] == nullptr){
			fprintf(stdout, "Handler not registered for type %d\n", type);
			return false;
		}
		HandlerTable[type](p);
		return true;
	}
};

void EchoHandler(Packet& p){
	char* PktBody = p.getMsg();
	fprintf(stdout, "%s\n", PktBody);
}

void EchoListHandler(Packet& p){
	char* PktBody = p.getMsg();

	int size = -1;

	memcpy(&size, PktBody, sizeof(int));

	std::vector<int> vec;

	for (int i = 1; i <= size; ++i){
		int val;
		memcpy(&val, PktBody + (sizeof(int)*i), sizeof(int));
		vec.push_back(val);
	}

	for (unsigned int i = 0; i < vec.size(); ++i){
		fprintf(stdout, "%d ", vec[i]);
	}
	fprintf(stdout, "\n");
}

void EchoCharacterHandler(Packet& p){
	char* PktBody = p.getMsg();
	Character character;

	memcpy(character.name, PktBody, NAMESIZE);
	memcpy(&character.x, PktBody + NAMESIZE, sizeof(double));
	memcpy(&character.y, PktBody + NAMESIZE + sizeof(double), sizeof(double));
	memcpy(&character.id, PktBody + NAMESIZE + 2 * sizeof(double), sizeof(long long));

	fprintf(stdout, "%s %lf %lf %lld\n", character.name, character.x, character.y, character.id);
}