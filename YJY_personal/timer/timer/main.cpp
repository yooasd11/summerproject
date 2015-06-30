#include <iostream>
#include <ctime>
#include <Windows.h>
using namespace std;

void timerClock(){
	clock_t prevClock = clock();
	while (clock() - prevClock <= (clock_t)10000) {}
	printf("%d millisecond(s) passed\n", clock() - prevClock);
}

void timerGTC(){
	int prevClock = GetTickCount();
	getchar();
	printf("%d millisecond(s) passed\n", GetTickCount() - prevClock);
}

void timerQuery(){
	LARGE_INTEGER start, end, f;
	QueryPerformanceFrequency(&f);
	QueryPerformanceCounter(&start);
	getchar();
	QueryPerformanceCounter(&end);

	double result = (end.QuadPart - start.QuadPart) / (double)f.QuadPart;
	printf("%f millisecond(s) passed\n", result);
}

int main(){
	timerClock();
	return 0;
}