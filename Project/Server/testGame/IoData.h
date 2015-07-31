#pragma once

typedef struct{
	OVERLAPPED overlapped;
	WSABUF wsaBuf;
	char buf[BUFSIZE];
	int rwMode;
}IoData;
