const int PktLength = (1 << 16);
const int PktHeaderSize = 2 * sizeof(USHORT);
const int PktBodySize = PktLength - PktHeaderSize;

#define NAME 20
#define MaxClnt 30
#define ECHO 14


class Packet{
private:
	char mMsg[PktBodySize];
	unsigned short mLength;
	unsigned short mType;
public:
	Packet() : mLength(2), mType(0) {
		*mMsg = NULL;
	}

	Packet(char* msg, unsigned short length, unsigned short type) : mLength(length), mType(type) {
		strcpy(mMsg, msg);
	}
	char* getMsg(){
		return mMsg;
	}

	unsigned short getLength(){
		return mLength;
	}

	bool setType(unsigned short type){
		mType = type;
		return true;
	}

	unsigned short getType(){
		return mType;
	}

	void inputMsg(){
		fgets(mMsg, PktBodySize, stdin);
		mLength = PktHeaderSize + strlen(mMsg);
	}
};