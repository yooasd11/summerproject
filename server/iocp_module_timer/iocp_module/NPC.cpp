#include "stdafx.h"
#include "NPC.h"


NPC::NPC()
{
	this->x = 0; this->y = 0;
}

NPC::NPC(std::string name, int _x, int _y, int _th){
	this->Name = name;
	this->x = _x;
	this->y = _y;
	this->th = _th;
}
NPC::~NPC()
{


}


void NPC::setNPC(std::string name, int _x, int _y, int _th){
	this->Name = name;
	this->x = _x;
	this->y = _y;
	this->th = _th;
	return;
}

std::string NPC::getNPCname(){
	return this->Name;
}

int NPC::getX(){
	return this->x;
}

int NPC::getY(){
	return this->y;
}

void NPC::Move(){

	this->x += 3;
	this->y += 2;
	//if (th == 0)
	printf("%d 새로운 위치 %d %d\n",this->th, this->x, this->y);
	return;
}