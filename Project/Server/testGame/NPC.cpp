#include "stdafx.h"
#include "NPC.h"


NPC::NPC()
{
	this->key = new Lock();
}

NPC::NPC(int _x, int _y, int _life, int _th)
{
	this->key = new Lock();
}

NPC::~NPC()
{


}


//이동함수는 적이 있나 없나 판단하고 , 주변에 있다고 판단했을 경우 attack을 시도..
//attack 함수는 attack을 
void NPC::NPCmove()
{
	if (this->state)
	{
		




	}
}