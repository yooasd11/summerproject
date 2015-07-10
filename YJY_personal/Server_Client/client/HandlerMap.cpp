#include "HandlerMap.h"
#ifndef MAIN_INCLUDE
#include "Character.h"
#endif
#include <vector>


inline void HandlerMap::RegistHandler(PACKET_HANDLER_TYPE eType, HandlerFunc func)
{
	HandlerTable[eType] = func;
}

inline void HandlerMap::UnregistHandler(int type){
	HandlerTable[type] = nullptr;
}

inline bool HandlerMap::HandlePacket(Packet& p){
	int type = p.getType();
	if (HandlerTable[type] == nullptr){
		fprintf(stdout, "Handler not registered for type %d\n", type);
		return false;
	}
	HandlerTable[type](p);
	return true;
}


//command registry
//macro defined
#define REGIST_HANDLE(PACKET_TYPE, PACKET_HANDLE) \
	void PACKET_HANDLE(Packet& p); \
	HandlerRegistHelper Handler_##PACKET_HANDLE(PACKET_TYPE, PACKET_HANDLE);

REGIST_HANDLE(P_ECHO, EchoHandler);
void EchoHandler(Packet& p){
	char* PktBody = p.getMsg();
	fprintf(stdout, "%s\n", PktBody);
}

REGIST_HANDLE(P_ECHOLIST, EchoListHandler);
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

REGIST_HANDLE(P_ECHOCHARACTER, EchoCharacterHandler);
void EchoCharacterHandler(Packet& p){
	char* PktBody = p.getMsg();
	Character character;

	memcpy(character.name, PktBody, NAMESIZE);
	memcpy(&character.x, PktBody + NAMESIZE, sizeof(double));
	memcpy(&character.y, PktBody + NAMESIZE + sizeof(double), sizeof(double));
	memcpy(&character.id, PktBody + NAMESIZE + 2 * sizeof(double), sizeof(long long));

	fprintf(stdout, "%s %lf %lf %lld\n", character.name, character.x, character.y, character.id);
}
