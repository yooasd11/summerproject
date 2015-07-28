#include "stdafx.h"
#include "bullet.h"


bullet::bullet(int _id, float _x, float _y, float _damage, float _v, float _direction)
{
	this->key = new Lock();
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
	delete this->key;
}

void bullet::bulletMove()
{
	//�����ϴ� �Ѿ˿� ���� �۾��� ó�� 
	LOCKING(this->key);
	if (this->working){
		//�Ѿ��� ���� �Ⱥ����� �Ǵ��� �˾ƾ���...640 X 320
		float dx = this->x + (this->velocity * 0.03f * sin(this->direction * PI / 180));
		float dy = this->y + (this->velocity * 0.03f * cos(this->direction * PI / 180));
		printf("%f %f\n", dx, dy);
		std::shared_ptr<bullet> bul = IocpConstructor::manageGame->retBullet(this->th);
		//���Ⱑ �ϴ� �浹ó���Ѵ� �κ���....
		//�ʰ��� �浹ó���� �������� �浹ó���� �ʿ��ϴ�...�ٿ����üũ..
		if (dx >= 640.0f || dx < 0 || dy >= 320.0f || dy < 0)
		{
			//�Ѿ��� ������� ��..
			//stop ��Ŷ�� ��������
			this->working = false;
			printf("���\n");
			//PacketHandler::GetInstance()->S_COLLISION_Handler(bul);
			//IocpConstructor::manageGame->removeBullet(bul->th);

			//PacketHandler::GetInstance()->S_COLLISION_Handler(IocpConstructor::manageGame->retBullet(this->th));
			//IocpConstructor::manageGame->removeBullet(IocpConstructor::manageGame->retBullet(this->th)->th);
			return;
		}

		//������ ��ġ�� ��ġ�ϴ��� Ȯ���ؾ���..���ݹ����� ����������� ? 
		std::map<int, std::shared_ptr<USER>>::iterator it;
		for (it = IocpConstructor::cm->mappingClient.begin(); it != IocpConstructor::cm->mappingClient.end(); it++)
		{
			//������ �¾��� ���..
			float userX = it->second->x; float userY = it->second->y;
			if ( sqrt( (dx - userX)*(dx - userX) + (dy - userY)*(dy - userY) ) < 20 && it->second->uid != this->uid)
			{
				//������ ������ ����..
				printf("damage\n");
				this->working = false;
				it->second->hp -= this->damage;
				//�������� �����κ� ����
				//PacketHandler::GetInstance()->S_COLLISION_Handler(bul,it->second->uid, it->second->hp  );
		//		IocpConstructor::manageGame->removeBullet(bul->th);


				//PacketHandler::GetInstance()->S_COLLISION_Handler(IocpConstructor::manageGame->retBullet(this->th), it->first, it->second->hp);
				//IocpConstructor::manageGame->removeBullet(IocpConstructor::manageGame->retBullet(this->th)->th);
				return;			
			}
		}
		TimerJob bulletMoveJob;
		this->x = dx;
		this->y = dy;
		//�����ε� ĳ�����ϴ� �κ��� ����..
		//PacketHandler::GetInstance()->C_SHOOT_Handler(IocpConstructor::manageGame->retBullet(this->th));
		//PacketHandler::GetInstance()->C_SHOOT_Handler(bul);
		bulletMoveJob.func = std::bind(&bullet::bulletMove, bul);
		bulletMoveJob.exectime = GetTickCount() + 30;
		IocpConstructor::jobs.push_back(bulletMoveJob);
	}
	return;
}