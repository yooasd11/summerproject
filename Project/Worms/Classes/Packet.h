#ifndef MAIN_INCLUDE
#include <cstring>
#endif

const int PKTLENGTH = (1 << 16);
const int PKTHEADERSIZE = 2 * sizeof(unsigned short);
const int PKTBODYSIZE = PKTLENGTH - PKTHEADERSIZE;

enum PACKET_TYPE
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
	PKT_END,			//13
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

//어떤 패킷이 응답으로 올지 명시
#define WAITING_FOR(PacketName)