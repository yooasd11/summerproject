#pragma once
#ifndef MAIN_INCLUDE
#include "Packet.h"
#endif

class PacketHandlerMap{
private:
	typedef void(*PacketHandler)(Packet& p);
	PacketHandler m_packetHandlerTable[PACKET_TYPE::PKT_END];
	PacketHandlerMap(){
		for (int i = 0; i < PACKET_TYPE::PKT_END; ++i)
			m_packetHandlerTable[i] = nullptr;
	}
	~PacketHandlerMap() {}
public:
	static PacketHandlerMap* getInstance(){
		static PacketHandlerMap instance;
		return &instance;
	}
	
	void registHandler(PACKET_TYPE eType, PacketHandler func);
	void unregistHandler(int type);
	bool handlePacket(Packet& p);
};

class HandlerRegistHelper
{
	typedef void (*HandlerFunc)(Packet&);
public:
	HandlerRegistHelper(PACKET_TYPE eType, HandlerFunc func)
	{
		PacketHandlerMap::getInstance()->registHandler(eType, func);
	}
};

