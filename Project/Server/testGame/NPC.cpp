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


//�̵��Լ��� ���� �ֳ� ���� �Ǵ��ϰ� , �ֺ��� �ִٰ� �Ǵ����� ��� attack�� �õ�..
//attack �Լ��� attack�� 
void NPC::NPCmove()
{
	if (this->state)
	{
		




	}
}