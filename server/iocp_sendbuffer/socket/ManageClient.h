#pragma once
#include "user.h"

class ManageClient
{
private:
	std::map<int, int> mappingClient;
	int ClntCnt;
	LOCKING *KEY;
public:
	user us[MAXCOUNT];
	ManageClient();
	~ManageClient();
	void mapping(int);
	int retIndex(int);
};

