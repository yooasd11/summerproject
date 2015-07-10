#include <iostream>
#include <stdio.h>
#include <string>
#include <process.h>
#include <windows.h>
using namespace std;


//READ �̿��� �� �ִ� ����, WRITE �̿��� �� ���� ����
#define READ 0
#define WRITE 1

//���� ���� WRITE -> READ, WRITE �� ������ ���
//���� ���� READ -> WRITE, READ �� ������ ���

//���ǰ˻縦 �� interlock..

unsigned int WINAPI ThreadFunction_1(void* arg);
unsigned int WINAPI ThreadFunction_2(void* arg);
unsigned int WINAPI ThreadFunction_3(void* arg);
unsigned int WINAPI ThreadFunction_4(void* arg);


//�Ǵ� �ڵ�
//class Lock{
//private:
//	long state;
//	long readCount;
//public:
//
//	Lock(){
//		this->state = READ;
//		this->readCount = 0;
//	}
//	//�д¶��� �ɾ��µ� ���� ���°� write�̸� ��������...
//	void readlock(){
//		while (InterlockedCompareExchange(&this->state, 1, 1));
//		//readcount�� �����ñ�Ų��....
//		InterlockedExchangeAdd(&this->readCount,1);
//		//InterlockedExchange(&this->state, )
//		return;
//	}
//	void writelock(){
//		//while (InterlockedCompareExchange(&this->state, 1, 1) || InterlockedCompareExchange(&this->readCount, this->readCount, this->readCount));
//		while (InterlockedCompareExchange(&this->state, 1, 1)){
//			while (InterlockedCompareExchange(&this->readCount, this->readCount, this->readCount));
//		}
//		InterlockedCompareExchange(&this->state, 1, 0);
//		return;
//	}
//	void readrelease(){
//		InterlockedExchangeAdd(&this->readCount, -1);
//		return;
//
//	}
//	void writerelease(){
//		InterlockedCompareExchange(&this->state, 0, 1);
//		return;
//	}
//};

class Lock{
private:
	long state;
	long readCount;
public:

	Lock(){
		this->state = READ;
		this->readCount = 0;
	}
	//�д¶��� �ɾ��µ� ���� ���°� write�̸� ��������...
	void readlock(){
		//while (InterlockedCompareExchange(&this->state, 1, 1));
		while (!InterlockedCompareExchange(&this->readCount, this->readCount += 1, this->readCount));
		//readcount�� �����ñ�Ų��....
		//InterlockedExchangeAdd(&this->readCount, 1);
		//InterlockedExchange(&this->state, )
		return;
	}
	void writelock(){
		//while (InterlockedCompareExchange(&this->state, 1, 1) || InterlockedCompareExchange(&this->readCount, this->readCount, this->readCount));
	/*	while (InterlockedCompareExchange(&this->state, 1, 1)){
			while (InterlockedCompareExchange(&this->readCount, this->readCount, this->readCount));
		}*/
		while ( !(InterlockedCompareExchange(&this->state, 1, 1)) );
		InterlockedCompareExchange(&this->state, 1, 0);
		return;
	}
	void readrelease(){
		InterlockedExchangeAdd(&this->readCount, -1);
		return;

	}
	void writerelease(){
		InterlockedCompareExchange(&this->state, 0, 1);
		return;
	}
};


Lock *key;
int main(){
	int count = 0;
	DWORD dwThread_1, dwThread_2, dwThread_3, dwThread_4;
	HANDLE hThread_1, hThread_2, hThread_3, hThread_4;
	key = new Lock();

	hThread_1 = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction_1, (void*)&count, 0, (unsigned*)&dwThread_1);
	hThread_2 = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction_2, (void*)&count, 0, (unsigned*)&dwThread_2);
	hThread_3 = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction_3, (void*)&count, 0, (unsigned*)&dwThread_3);
	hThread_4 = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction_4, (void*)&count, 0, (unsigned*)&dwThread_4);

	WaitForSingleObject(hThread_1, INFINITE);
	WaitForSingleObject(hThread_2, INFINITE);
	WaitForSingleObject(hThread_3, INFINITE);
	WaitForSingleObject(hThread_4, INFINITE);

	CloseHandle(hThread_1);
	CloseHandle(hThread_2);
	CloseHandle(hThread_3);
	CloseHandle(hThread_4);

	delete key;
	printf("%d\n", count);

	return 0;
}

unsigned int WINAPI ThreadFunction_1(void* arg){

	for (int i = 0; i < 5200; i++){
		key->writelock();
		*((int *)arg) += 1;
		key->writerelease();

		key->readlock();
		printf("%d\n", *((int*)arg));
		key->readrelease();
	}

	return 0;
}

unsigned int WINAPI ThreadFunction_2(void* arg){

	for (int i = 0; i < 5400; i++){
		key->writelock();
		*((int *)arg) += 1;
		key->writerelease();

		key->readlock();
		printf("%d\n", *((int*)arg));
		key->readrelease();
	}
	return 0;
}

unsigned int WINAPI ThreadFunction_3(void* arg){

	for (int i = 0; i < 5400; i++){
		key->writelock();
		*((int *)arg) += 1;
		key->writerelease();

		key->readlock();
		printf("%d\n", *((int*)arg));
		key->readrelease();
	}
	return 0;
}

unsigned int WINAPI ThreadFunction_4(void* arg){

	for (int i = 0; i < 5400; i++){
		key->writelock();
		*((int *)arg) += 1;
		key->writerelease();

		key->readlock();
		printf("%d\n", *((int*)arg));
		key->readrelease();
	}
	return 0;
}