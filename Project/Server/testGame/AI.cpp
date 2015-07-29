#include "stdafx.h"
#include "AI.h"


AI::AI()
{
	this->current = state::alive;
	this->key = new Lock();
}

AI::AI(int _nid, int _hp, float _x, float _y, float _direction, float _velocity)
{
	this->current = state::alive;
	this->nid = _nid;
	this->hp = _hp;
	this->x = _x;
	this->y = _y;
	this->direction = _direction;
	this->velocity = _velocity;
	this->key = new Lock();
}

AI::~AI()
{


}

float getDegree(float x1, float y1, float x2, float y2)
{
	return atan2(y2-y1, x2-x1);
}

//�̵��Լ��� ���� �ֳ� ���� �Ǵ��ϰ� , �ֺ��� �ִٰ� �Ǵ����� ��� attack�� �õ�..
//attack �Լ��� attack�� 
void AI::Action()
{
	LOCKING(this->key);
	//����ִ� ������ AI�� �ൿ����
	if (this->current == state::alive)
	{
		//������ Ž��
		//std::map<SOCKET, std::shared_ptr<USER>> mappingClient;
		std::map<int, std::shared_ptr<USER>>::iterator it;
		for (it = IocpConstructor::cm->mappingClient.begin(); it != IocpConstructor::cm->mappingClient.end(); it++)
		{
			float dx = it->second->x; float dy = it->second->y;
			//�����Ÿ� ������ �� ������ �Ѵ�...�Ѿ��� �����ؼ� �������� 
			if (sqrt((this->x - dx)*(this->x - dx) + (this->y - dy)*(this->y - dy)) < 50)
			{
		//		float degree = getDegree(this->x, this->y, dx, dy);
		//		std::shared_ptr<bullet> Bullet(new bullet(IocpConstructor::manageGame->bulletCount, ClientBullet.uid(), ClientBullet.x(), ClientBullet.y(),
		//			ClientBullet.damage(), ClientBullet.velocity(), ClientBullet.direction()));
				//�Ѿ��� �����ϰ� �߻�..�ٵ� ������ ����
				//�߻��ϰ� 1���Ŀ� �۾��� �ٽ� ����ǵ��� ����...
			}
		}
		//������ ���� ��� �̵�...�̷��� �Ǹ� �������θ� �����̰� ��..
		//AI ��ü�� direction��, �Ѿ��� direction�� ����
		this->x = this->x + (this->velocity * 0.03f * sin(this->direction * PI / 180));
		this->y = this->y + (this->velocity * 0.03f * cos(this->direction * PI / 180));
	}
}