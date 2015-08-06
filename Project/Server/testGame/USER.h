#pragma once
class USER : public OBJECT
{
private:
	Lock key;
public:
	int socket;
	float ax;
	float ay;
	enum state{
		ALIVE = 1,
		DEAD,
		DISCONNECT,
		STOP,
	};
	Lock user_buffer_key;
	char user_buffer[BUFSIZE];
	enum state CurrentState;
	USER();
	USER(int, float, float, int);
	~USER();
	void SetVelocity(float, float);
	void SetAccelate(float, float); 
	void ChangeState(int);
	void USER_MOVE();
	void USER_SET_HP(int);
	void USER_RESPAWN();
	virtual void CALCULATE_DISTANCE();
};

