#pragma once
class HandleData
{
public:
	SOCKET ClntSock;
	SOCKADDR_IN clntAdr;
	HandleData();
	~HandleData();
};

