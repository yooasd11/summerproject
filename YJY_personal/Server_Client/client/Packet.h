#ifndef MAIN_INCLUDE
#include <cstring>
#endif

const int PKTLENGTH = (1 << 16);
const int PKTHEADERSIZE = 2 * sizeof(unsigned short);
const int PKTBODYSIZE = PKTLENGTH - PKTHEADERSIZE;

enum PACKET_HANDLER_TYPE
{
	P_BEGIN = 0,
	P_ECHO,
	P_ECHOLIST,
	P_ECHOCHARACTER,
	P_MAX,
};

class Packet{
private:
	unsigned short mLength;
	unsigned short mType;
	char mMsg[PKTBODYSIZE];
public:
	Packet() : mLength(0), mType(0) {}

	void setLength(unsigned short length){
		mLength = length;
	}

	unsigned short getLength(){
		return mLength;
	}

	void setType(unsigned short type){
		mType = type;
	}

	unsigned short getType(){
		return mType;
	}

	char* getMsg(){
		return mMsg;
	}

	void setMsg(char* msg, int len){
		memcpy(mMsg, msg, len);
	}
};

