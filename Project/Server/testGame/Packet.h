#pragma once


const int PKTLENGTH = (1 << 16);
const int PKTHEADER = 2 * sizeof(unsigned short);
const int PKTBODY = PKTLENGTH - PKTHEADER;

enum PACKET_HANDLER_TYPE
{
	PKT_BEGIN = 0,
	PKT_ECHO,			//1
	PKT_ACCOUNT,		//2
	PKT_ACCOUNT_LIST,	//3
	PKT_S_MOVE,			//4
	PKT_C_MOVE,			//5
	PKT_S_STOP,			//6
	PKT_C_STOP,			//7
	PKT_C_SHOOT,		//8
	PKT_S_SHOOT,		//9
	PKT_S_DISCONNECT,	//10
	PKT_C_COLLISION,	//11
	PKT_S_COLLISION,	//12
	PKT_END,			//11
};

class Packet
{
private:
	
public:
	unsigned short Length;
	unsigned short Type;
	int uid;
	char Msg[PKTBODY];
	Packet();
	~Packet();
	Packet* PacketSeperate(char*, int);
	void setLength(unsigned short);
	unsigned short getLength();
	void setType(unsigned short);
	unsigned short getType();
	char* getMsg();
	void setMsg(char*, int);
};