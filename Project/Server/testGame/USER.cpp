#include "stdafx.h"
#include "USER.h"


USER::USER()
{
	this->x = 10.0f;
	this->y = 10.0f;
	this->hp = 100;
	this->current = 0;
	this->total = 0;
	state::WAITING;
}

USER::USER(int _id, int _hp, float _x, float _y)
{
	this->uid = _id;
	this->hp = _hp;
	this->x = _x;
	this->y = _y;
	this->current = 0;
	this->total = 0;
	state::WAITING;
}

USER::~USER()
{
}


void USER::setUSER(int _hp, float _x, float _y)
{
	this->hp = _hp;
	this->x = _x;
	this->y = _y;
}

void USER::setTotal(int len)
{
	this->total = len;
	return;
}

void USER::setCurrent(int len)
{
	this->current = len;
	return;
}

int USER::getTotal()
{
	return this->total;
}

int USER::getCurrent()
{
	return this->current;
}

void USER::setBuffer(char *msg, int len)
{
	memcpy(this->Buffer + this->current, msg, len);
	return;
}

char* USER::getBuffer()
{
	return this->Buffer;
}


bool USER::finished()
{
	if (this->current == this->total) return true;
	return false;
}

void USER::clear()
{
	this->current = 0;
	this->total = 0;
	memset(this->Buffer, 0, sizeof(this->Buffer));
}