#pragma once

class PacketHandler
{
private:
	typedef void(*HandlerFunc)(Packet&);
	HandlerFunc HandlerTable[10];
	PacketHandler();
	~PacketHandler();
public:
	static PacketHandler* GetInstance()
	{
		static PacketHandler pInstance;
		return &pInstance;
	}
	void RegistHandler(PACKET_HANDLER_TYPE eType, HandlerFunc func);
	void UnregistHandler(int type);
	bool HandlePacket(Packet& p);
	void SendAccountPacket(SOCKET, int);
	void BroadCastAccountPacket();
	void BroadCast(char*, int);
	
	void S_DISCONNECT_HANDLER(int);
	void C_MOVE_HANDLER(Packet&);
	void S_MOVE_HANDLER(std::shared_ptr<OBJECT>);
	void S_STOP_HANDLER(std::shared_ptr<OBJECT>);
	void C_STOP_HANDLER(Packet&);
	void S_SHOOT_HANDLER(std::shared_ptr<OBJECT>);
	void C_SHOOT_HANDLER(Packet&);
	
	void S_COLLISION_HANDLER(std::shared_ptr<OBJECT>, std::shared_ptr<OBJECT>);
	void C_COLLISION_HANDLER(Packet&);

	void S_RESPAWN_HANDLER(std::shared_ptr<OBJECT>);
};

class HandlerRegistHelper
{
	typedef void(*HandlerFunc)(Packet&);
public:
	HandlerRegistHelper(PACKET_HANDLER_TYPE eType, HandlerFunc func)
	{
		PacketHandler::GetInstance()->RegistHandler(eType, func);
	}
};