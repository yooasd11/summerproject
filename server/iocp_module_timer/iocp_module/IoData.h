#pragma once
class IoData
{
public:
	OVERLAPPED overlapped;
	WSABUF wsaBuf;
	char buf[BufSize];
	int rwMode;
	IoData();
	~IoData();
};

