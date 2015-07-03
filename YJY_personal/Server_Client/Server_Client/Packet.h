#include <vector>

const int PKTLENGTH = (1 << 16);
const int PKTHEADERSIZE = 2 * sizeof(USHORT);
const int PKTBODYSIZE = PKTLENGTH - PKTHEADERSIZE;

#define ECHOLIST 11
#define ECHOCHARACTER 12
#define ECHO 14


class Packet{
private:
	unsigned short mLength;
	unsigned short mType;
	char mMsg[PKTBODYSIZE];
public:
	Packet() : mLength(0), mType(0) {}

	void setLength(USHORT length){
		mLength = length;
	}

	USHORT getLength(){
		return mLength;
	}

	void setType(USHORT type){
		mType = type;
	}

	USHORT getType(){
		return mType;
	}

	char* getMsg(){
		return mMsg;
	}

	void setMsg(char* msg, int len){
		memcpy(mMsg, msg, len);
	}
};

