// thread2.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"


unsigned int WINAPI ThreadFunction_1(void* arg);
unsigned int WINAPI ThreadFunction_2(void* arg);


//int argc, _TCHAR* argv[]
int _tmain(int argc, _TCHAR* argv[])
{
	int count = 0;
	DWORD dwThreadArray[10];
	HANDLE hThreadArray[10];
	

	for (int i = 0; i < 100000; i++){

		if (i % 2 == 0){
			hThreadArray[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction_1, (void*)&count, 0, (unsigned*)&dwThreadArray[i]);

		}
		else{
			hThreadArray[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction_2, (void*)&count, 0, (unsigned*)&dwThreadArray[i]);

		}
	}
	printf("%d\n", count);

	return 0;
}

unsigned int WINAPI ThreadFunction_1(void* arg){
	
	*((int *)arg) += 1;
	return 0;
}

unsigned int WINAPI ThreadFunction_2(void* arg){
	*((int*)arg) -= 1;
	return 0;
}