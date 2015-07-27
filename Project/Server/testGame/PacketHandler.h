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
	void SendAccountPacket(SOCKET);
	void BroadCastAccountPacket();
	void BroadCast(char*, int);
	void C_MOVE_Handler(Packet&);
	void C_STOP_handler(Packet&);
	void C_SHOOT_handler(Packet&);
	void C_DISCONNECT_Handler(SOCKET);
	void C_STOP_handler(std::shared_ptr<bullet>);
	void S_COLLISION_Handler(std::shared_ptr<bullet>);
	void S_COLLISION_Handler(std::shared_ptr<bullet>,int,int);
	void S_COLLISION_Handler(std::shared_ptr<USER>);
	void C_MOVE_Handler(std::shared_ptr<USER>);
	void C_SHOOT_Handler(std::shared_ptr<bullet>);
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