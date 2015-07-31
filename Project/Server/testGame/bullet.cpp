#include "stdafx.h"
#include "bullet.h"


bullet::bullet(int _id, int _shooter,  float _x, float _y, float _damage, float _v, float _direction)
{
	this->key = new Lock();
	this->uid = _id;
	this->x = _x;
	this->y = _y;
	this->velocity = _v;
	this->direction = _direction;
	this->damage = _damage;
	this->shooter = _shooter;
	this->working = true;
}

bullet::bullet()
{
	this->key = new Lock();
}

bullet::~bullet()
{
	delete this->key;
}

void bullet::ChangeState(bool s)
{
	LOCKING(this->key);
	if (!this->working)
		this->working = s;
	return;
}

void bullet::bulletMove()
{
	//�����ϴ� �Ѿ˿� ���� �۾��� ó�� 
	LOCKING(this->key);
	if (this->working){
		//�Ѿ��� ���� �Ⱥ����� �Ǵ��� �˾ƾ���...640 X 320
		float dx = this->x + (this->velocity * 0.03f * sin(this->direction * PI / 180));
		float dy = this->y + (this->velocity * 0.03f * cos(this->direction * PI / 180));
		//printf("%f %f\n", dx, dy);
		std::shared_ptr<bullet> bul = IocpConstructor::manageGame->retBullet(this->uid);
		//���Ⱑ �ϴ� �浹ó���Ѵ� �κ���....
		//�ʰ��� �浹ó���� �������� �浹ó���� �ʿ��ϴ�...�ٿ����üũ..
		if (dx >= 1500.0f || dx < 0 || dy >= 700.0f || dy < 0)
		{
			//�Ѿ��� ������� ��..
			//stop ��Ŷ�� ��������
			this->working = false;
			PacketHandler::GetInstance()->S_COLLISION_Handler(bul);
			IocpConstructor::manageGame->removeBullet(bul->uid);
			return;
		}

		for (auto user : IocpConstructor::cm->mappingClient)
		{
			float userX = user.second->x; float userY = user.second->y;
			if (sqrt((dx - userX)*(dx - userX) + (dy - userY)*(dy - userY)) < DAMAGE_DISTANCE && user.second->objectID != this->shooter && (user.second->crt != USER::state::DEAD))
			{
				this->working = false;
				user.second->hp -= this->damage;
				if (user.second->hp <= 0)
				{
					printf("�׾���\n");
					user.second->ChangeState(USER::state::DEAD);
					printf("%d\n", user.second->crt);
				}
				//�������� �����κ� ����
				PacketHandler::GetInstance()->S_COLLISION_Handler(bul, user.second->objectID, user.second->hp);
				IocpConstructor::manageGame->removeBullet(bul->uid);

				return;
			}
		}

		for (auto AI :IocpConstructor::nm->mappingAI )
		{
			float aiX = AI.second->x; float aiY = AI.second->y;
			if (sqrt((dx - aiX)*(dx - aiX) + (dy - aiY)*(dy - aiY)) < DAMAGE_DISTANCE && AI.second->nid != this->shooter && AI.second->current_state != AI.second->npc_dead)
			{
				this->working = false;
				AI.second->hp -= this->damage;
				if (AI.second->hp <= 0)
				{
					AI.second->ChangeState(NPC_STATUS_DEAD);
				}
				PacketHandler::GetInstance()->S_COLLISION_Handler(bul, AI.second->nid, AI.second->hp);
				IocpConstructor::manageGame->removeBullet(bul->uid);
				return;
			}
		}
		TimerJob bulletMoveJob;
		this->x = dx;
		this->y = dy;
		//�����ε� ĳ�����ϴ� �κ��� ����..
		//PacketHandler::GetInstance()->C_SHOOT_Handler(bul);
		bulletMoveJob.func = std::bind(&bullet::bulletMove, bul);
		bulletMoveJob.exectime = GetTickCount() + 30;
		{
			LOCKING(IocpConstructor::queueLock)
			IocpConstructor::jobs.push_back(bulletMoveJob);
		}
		
	}
	return;
}