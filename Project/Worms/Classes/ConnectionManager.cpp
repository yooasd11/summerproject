#include "ConnectionManager.h"
#include "PacketHandlerMap.h"
#include "cocos2d.h"
#include "AccountPacket.pb.h"

inline void ConnectionManager::errorHandling(char *msg){
	CCLOG("%s", msg);
	exit(1);
}

int ConnectionManager::accountTo(char* ip, int port){
	WSADATA wsaData;
	SOCKADDR_IN servAdr;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){
		errorHandling("WSAStartup() error");
		return -1;
	}

	if ((m_servSocket = socket(PF_INET, SOCK_STREAM, 0)) == -1){
		errorHandling("socket() error");
		return -2;
	}

	char* pIp = new char[std::strlen(ip)];
	strcpy(pIp, ip);
	if (!strcmp(pIp, "localhost")) 
		strcpy(pIp, "127.0.0.1");

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr(pIp);
	servAdr.sin_port = htons(port);

	if (connect(m_servSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR){
		errorHandling("connect() error");
		return -3;
	}
	fflush(stdin);
	FD_ZERO(&m_reads);
	FD_SET(m_servSocket, &m_reads);
	CCLOG("Connection established to %s:%d", ip, port);
	return 0;
}

int ConnectionManager::recvMsg(){
	char HeaderBuf[PKTHEADERSIZE];
	char BodyBuf[PKTBODYSIZE];
	Packet packet;

	int retRecv = recv(m_servSocket, HeaderBuf, PKTHEADERSIZE, 0);
	if (retRecv < 4) return -1;	// 이상한 헤더
	USHORT PktLen, PktType;

	memcpy(&PktLen, HeaderBuf, sizeof(USHORT));
	memcpy(&PktType, HeaderBuf + sizeof(USHORT), sizeof(USHORT));

	packet.setLength(PktLen);
	packet.setType(PktType);
	if (PktType >= PACKET_TYPE::PKT_END) return -2;
	int rcvdPacketLength = 0;
	while (rcvdPacketLength < PktLen)
	{
		int retSuccessRevSize = recv(m_servSocket, BodyBuf + rcvdPacketLength, PktLen, 0);
		rcvdPacketLength += retSuccessRevSize;
	}

	packet.setMsg(BodyBuf, PktLen);

	PacketHandlerMap::getInstance()->handlePacket(packet);

	return 0;
}

bool ConnectionManager::receive(){
	while (1){
		m_cpyReads = m_reads;
		m_timeout.tv_sec = 0;
		m_timeout.tv_usec = 1;

		int fdNum;

		if ((fdNum = select(0, &m_cpyReads, 0, 0, &m_timeout)) == SOCKET_ERROR) return false;
		if (fdNum <= 0) break;

		if (FD_ISSET(m_servSocket, &m_cpyReads)){
			if (recvMsg() == -1)
				errorHandling("ConnectionManager disabled");
		}
	}
	return true;
}

unsigned WINAPI ConnectionManager::transmit(const USHORT& PacketLength, const USHORT& PacketType, void* s){
	char PktBuf[PKTLENGTH];
	
	memcpy(PktBuf, &PacketLength, sizeof(USHORT));
	memcpy(PktBuf + sizeof(USHORT), &PacketType, sizeof(USHORT));
	memcpy(PktBuf + PKTHEADERSIZE, s, PacketLength);

	int sndPacketLength = 0, totalSize = PKTHEADERSIZE + PacketLength;
	while (sndPacketLength < totalSize){
		int retPktLength = send(this->getSocket(), PktBuf + sndPacketLength, totalSize, 0);
		sndPacketLength += retPktLength;
	}
	return 0;
}
