#include "stdafx.h"
#include "USER.h"


USER::USER()
{
	this->connect = true;
	this->direction = 0;
	this->x = 10.0f;
	this->y = 10.0f;
	this->hp = 100;
	this->current = 0;
	this->total = 0;
	this->state = WAIT;
	this->velocity = 0.0f;
	this->key = new Lock();
}

USER::USER(int _id, int _hp, float _x, float _y)
{
	this->direction = 0;
	this->connect = true;
	this->uid = _id;
	this->hp = _hp;
	this->x = _x;
	this->y = _y;
	this->current = 0;
	this->total = 0;
	this->state = WAIT;
	this->velocity = 0.0f;
	this->key = new Lock();
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
	memcpy(this->Buffer, msg, len);
	return;
}

char* USER::getBuffer()
{
	return this->Buffer;
}


bool USER::finished()
{
	if (this->current == this->total && this->current != 0) return true;
	return false;
}

void USER::clear()
{
	this->current = 0;
	this->total = 0;
	memset(this->Buffer, 0, sizeof(this->Buffer));
}

void USER::UserpacketHandle(char *msg, int len, int _uid)
{

	Packet userPacket;
	unsigned short current = 0;

	LOCKING(this->key);
	memcpy(this->Buffer, msg, len);
	this->total = len;
	this->uid = _uid;
	if (this->isConnecting()){
		while (current < this->getTotal()){
			memcpy(&userPacket.Length, this->Buffer + current, sizeof(unsigned short));
			current += sizeof(unsigned short);
			memcpy(&userPacket.Type, this->Buffer + current, sizeof(unsigned short));
			current += sizeof(unsigned short);
			memcpy(userPacket.Msg, this->Buffer + current, userPacket.Length);
			current += userPacket.Length;
			PacketHandler::GetInstance()->HandlePacket(userPacket);
		}
	}
	//'clear'�� ��ġ�� �� �������־���.....
	this->clear();
	return;
}

void USER::UserpacketHandle()
{
	Packet userPacket;
	unsigned short current = 0;

	//����ؼ� ������ ó���� �� �ֵ���
	LOCKING(this->key);
	//���Ἲ�� Ȯ��...������ �ȵǾ� �ִٸ�
	if (this->isConnecting()){
		while (current < this->getTotal()){
			memcpy(&userPacket.Length, this->Buffer + current, sizeof(unsigned short));
			current += sizeof(unsigned short);
			memcpy(&userPacket.Type, this->Buffer + current, sizeof(unsigned short));
			current += sizeof(unsigned short);
			memcpy(userPacket.Msg, this->Buffer + current, userPacket.Length);
			current += userPacket.Length;
			PacketHandler::GetInstance()->HandlePacket(userPacket);
		}
	}
	//'clear'�� ��ġ�� �� �������־���.....
	this->clear();
}

void USER::UserMove(){

	TimerJob userMoveJob;
	//LOCKING(this->key);
	//�ð��ʺ��� ������ ����ǰ���..��ť���� �˾Ƽ� ��������
	if (this->isConnecting()){
		float dx = this->x + (this->velocity * 0.03f * sin(this->direction * PI / 180));
		float dy = this->y + (this->velocity * 0.03f * cos(this->direction * PI / 180));
		printf("%f %f\n", dx, dy);
		if (this->state == MOVE){
			if (!(dx > 640.0f || dx < 0.0f || dy > 320.0f || dy < 0.0f))
			{
				this->x = dx;
				this->y = dy;
			}
			//������ġ ���Ű� ��ġ�� ��ġ�� ��ε�ĳ����
			PacketHandler::GetInstance()->C_MOVE_Handler(IocpConstructor::cm->retUser(this->uid));
			printf("%f %f\n", dx, dy);
			//������ �۾��� ���ؼ� ó��..
			userMoveJob.func = std::bind(&USER::UserMove, IocpConstructor::cm->retUser(this->uid));
			userMoveJob.exectime = GetTickCount() + 30;
			IocpConstructor::jobs.push_back(userMoveJob);
		}
	}
	return;
}

bool USER::isConnecting(){
	return this->connect;
}