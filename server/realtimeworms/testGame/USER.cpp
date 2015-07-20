#include "stdafx.h"
#include "USER.h"


USER::USER()
{
	this->x = 10;
	this->y = 10;
	this->hp = 100;
	this->Buffer = new char[BUFSIZE];
}

USER::USER(int _id, int _hp, float _x, float _y)
{
	this->uid = _id;
	this->hp = _hp;
	this->x = _x;
	this->y = _y;
	this->Buffer = new char[BUFSIZE];
}

USER::~USER()
{
	//delete this->Buffer;
}


void USER::setUSER(int _hp, float _x, float _y)
{
	this->hp = _hp;
	this->x = _x;
	this->y = _y;
}