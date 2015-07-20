#pragma once


const int PKTLENGTH = (1 << 16);
const int PKTHEADER = 2 * sizeof(unsigned short);
const int PKTBODY = PKTLENGTH - PKTHEADER;

enum PAKCET_HANDLER_TYPE
{
	P_echo=1,
	P_Account,
	P_SMOVE,
	P_CMOVE,
};


class Packet
{
private:
	unsigned short Length;
	unsigned short Type;
	char Msg[PKTBODY];
public:
	Packet();
	~Packet();
	void setLength(unsigned short);
	unsigned short getLength();
	void setType(unsigned short);
	unsigned short getType();
	char* getMsg();
	void setMsg(char*, int);
};