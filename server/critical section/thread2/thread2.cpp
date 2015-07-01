// thread2.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Locking.h"


unsigned int WINAPI ThreadFunction_1(void* arg);
unsigned int WINAPI ThreadFunction_2(void* arg);

LOCKING *KEY;

int _tmain(int argc, _TCHAR* argv[])
{
	int count = 0;
	DWORD dwThread_1, dwThread_2;
	HANDLE hThread_1, hThread_2;
	KEY = new LOCKING();

	hThread_1 = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction_1, (void*)&count, 0, (unsigned*)&dwThread_1);
	hThread_2 = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction_2, (void*)&count, 0, (unsigned*)&dwThread_2);

	WaitForSingleObject(hThread_1, INFINITE);
	WaitForSingleObject(hThread_2, INFINITE);

	CloseHandle(hThread_1);
	CloseHandle(hThread_2);

	delete KEY;
	printf("%d\n", count);

	return 0;
}

unsigned int WINAPI ThreadFunction_1(void* arg){
	
	for (int i = 0; i < 5200; i++){
		KEY->LOCK();
		*((int *)arg) += 1;
		KEY->UNLOCK();
	}
	
	return 0;
}

unsigned int WINAPI ThreadFunction_2(void* arg){

	for (int i = 0; i < 5400; i++){
		KEY->LOCK();
		*((int*)arg) += 1;
		KEY->UNLOCK();
	}
	return 0;
}