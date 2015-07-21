#pragma once


const int PKTLENGTH = (1 << 16);
const int PKTHEADER = 2 * sizeof(unsigned short);
const int PKTBODY = PKTLENGTH - PKTHEADER;

enum PACKET_HANDLER_TYPE
{
	PKT_BEGIN =0,
	PKT_ECHO,
	PKT_ACCOUNT,
	PKT_ACCOUNT_LIST,
	PKT_S_MOVE, //right =0, left =1
	PKT_C_MOVE,
	PKT_S_STOP,
	PKT_C_STOP,
	PKT_END,
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