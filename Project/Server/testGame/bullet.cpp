#include "stdafx.h"
#include "bullet.h"


bullet::bullet(int _th, int _id, float _x, float _y, float _damage, float _v, float _direction)
{
	this->key = new Lock();
	this->th = _th;
	this->uid = _id;
	this->x = _x;
	this->y = _y;
	this->velocity = _v;
	this->direction = _direction;
	this->damage = _damage;

}

bullet::bullet()
{
	this->key = new Lock();
}


bullet::~bullet()
{
}

void bullet::bulletMove()
{
	//�����ϴ� �Ѿ˿� ���� �۾��� ó��
	if (this->working){
		//�Ѿ��� ���� �Ⱥ����� �Ǵ��� �˾ƾ���...640 X 320
		float dx = this->x + (this->velocity * 0.1f * cos(this->direction * PI / 180));
		float dy = this->y + (this->velocity * 0.1f * sin(this->direction * PI / 180));
		if (dx >= 640.0f || dx < 0 || dy >= 320.0f || dy < 0)
		{
			//�Ѿ��� ������� ��..
			//stop ��Ŷ�� ��������
			this->working = false;
			PacketHandler::GetInstance()->C_STOP_handler(IocpConstructor::manageGame->retBullet(this->th));
			IocpConstructor::manageGame->removeBullet(this->th);
			return;
		}

		//������ ��ġ�� ��ġ�ϴ��� Ȯ���ؾ���..���ݹ����� ����������� ? 
		std::map<SOCKET, std::shared_ptr<USER>>::iterator it;
		for (it = IocpConstructor::cm->mappingClient.begin(); it != IocpConstructor::cm->mappingClient.end(); it++)
		{
			//�¾��� ��� ��� ó��..
			float userX = it->second->x; float userY = it->second->y;
		}

		TimerJob bulletMoveJob;
		this->x += (this->velocity * 0.1f * cos(this->direction * PI / 180));
		this->y += (this->velocity * 0.1f * sin(this->direction * PI / 180));
		PacketHandler::GetInstance()->C_SHOOT_Handler(IocpConstructor::manageGame->retBullet(this->th));

		bulletMoveJob.func = std::bind(&bullet::bulletMove, this);
		bulletMoveJob.exectime = GetTickCount() + 100;
		IocpConstructor::jobs.push_back(bulletMoveJob);
	}
	return;
}