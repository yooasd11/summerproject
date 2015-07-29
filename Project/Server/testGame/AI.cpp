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

//이동함수는 적이 있나 없나 판단하고 , 주변에 있다고 판단했을 경우 attack을 시도..
//attack 함수는 attack을 
void AI::Action()
{
	LOCKING(this->key);
	//살아있는 상태의 AI의 행동구현
	if (this->current == state::alive)
	{
		//유저를 탐색
		//std::map<SOCKET, std::shared_ptr<USER>> mappingClient;
		std::map<int, std::shared_ptr<USER>>::iterator it;
		for (it = IocpConstructor::cm->mappingClient.begin(); it != IocpConstructor::cm->mappingClient.end(); it++)
		{
			float dx = it->second->x; float dy = it->second->y;
			//일정거리 이하일 때 공격을 한다...총알을 생성해서 보내야함 
			if (sqrt((this->x - dx)*(this->x - dx) + (this->y - dy)*(this->y - dy)) < 50)
			{
		//		float degree = getDegree(this->x, this->y, dx, dy);
		//		std::shared_ptr<bullet> Bullet(new bullet(IocpConstructor::manageGame->bulletCount, ClientBullet.uid(), ClientBullet.x(), ClientBullet.y(),
		//			ClientBullet.damage(), ClientBullet.velocity(), ClientBullet.direction()));
				//총알을 생성하고 발사..근데 방향을 어디로
				//발사하고 1초후에 작업이 다시 진행되도록 설정...
			}
		}
		//유저가 없는 경우 이동...이렇게 되면 한쪽으로만 움직이게 됨..
		//AI 자체의 direction과, 총알의 direction을 구분
		this->x = this->x + (this->velocity * 0.03f * sin(this->direction * PI / 180));
		this->y = this->y + (this->velocity * 0.03f * cos(this->direction * PI / 180));
	}
}