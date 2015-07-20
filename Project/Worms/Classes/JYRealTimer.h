#pragma once

#include "cocos2d.h"

class JYRealTimer{
private:
	LARGE_INTEGER nLast, nFreq;
public:
	JYRealTimer() {
		QueryPerformanceFrequency(&nFreq);
		QueryPerformanceCounter(&nLast);
	}

	~JYRealTimer() {}

	float getElapsedTime(){
		LARGE_INTEGER nNow;
		QueryPerformanceCounter(&nNow);
		float ret = (float)(nNow.QuadPart - nLast.QuadPart) / nFreq.QuadPart;
		nLast = nNow;
		return ret;
	}
};