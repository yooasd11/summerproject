#include "stdafx.h"
#include "Packet.h"

Packet::Packet()
{
	this->key = new Lock();
}


Packet::~Packet()
{
	delete this->key;
}

void Packet::setLength(unsigned short length)
{
	this->Length = length;
}

unsigned short Packet::getLength()
{
	return this->Length;
}

void Packet::setType(unsigned short type)
{
	this->Type = type;
}

unsigned short Packet::getType()
{
	return this->Type; 
}

char* Packet::getMsg()
{
	return this->Msg;
}

void Packet::setMsg(char *msg, int len)
{
	memcpy(this->Msg, msg, len);
}

bool Packet::PacketSeperate(char* msg, int Len, int _socket)
{
	LOCKING(this->key);
	this->socket = _socket;
	memcpy(&this->Length, msg, sizeof(unsigned short));
	memcpy(&this->Type, msg + sizeof(unsigned short), sizeof(unsigned short));
	memcpy(this->Msg, msg + (sizeof(unsigned short)* 2), this->Length);
	if (Len - PKTHEADER != this->Length) return false;
	else PacketHandler::GetInstance()->HandlePacket(*this);
	return true;
}