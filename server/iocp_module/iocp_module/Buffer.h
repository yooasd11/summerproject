#pragma once
class Buffer
{
public:
	char buf[BufSize];
	int CurrentLength;
	int TotalLength;
	Buffer();
	~Buffer();
};

