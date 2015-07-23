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

	void C_MOVE_Handler(std::shared_ptr<USER>);
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