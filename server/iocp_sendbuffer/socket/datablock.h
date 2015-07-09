typedef struct{
	SOCKET hClntSock;
	SOCKADDR_IN clntAdr;
}PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

typedef struct{
	OVERLAPPED overlapped;
	WSABUF wsaBuf;
	char buf[BUF_SIZE];
	int rwMode;

}PER_IO_DATA, *LPPER_IO_DATA;