#ifndef MAIN_INCLUDE
#define MAIN_INCLUDE
#include "winsockstd.h"
#include "Character.h"
#include "Packet.h"
#include "Connection.h"
#endif

Connection* ConnectionManager;


int _tmain()
{
	char ip[101] = "localhost";
	int port = 9200;

	/*printf_s("Input server info(IP PORT) : ");
	fscanf_s(stdin, "%s", ip, sizeof(ip));
	fscanf_s(stdin, "%d", &port);*/

	//Create a Connection Manager and account to server
	ConnectionManager = Connection::GetInstance();
	ConnectionManager->AccountTo(ip, port);

	//Start sending packet thread
	HANDLE hSndThread = (HANDLE)_beginthreadex(NULL, 0, ConnectionManager->SendMsg, NULL, 0, NULL);

	while (true)
	{
		ConnectionManager->Receive();
	}

	WaitForSingleObject(hSndThread, INFINITE);

	return 0;
}
