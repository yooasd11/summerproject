#include "stdafx.h"
#include "Buffer.h"


Buffer::Buffer()
{
	this->CurrentLength = 0;
	this->TotalLength = 0;
	memset(this->buf, 0, sizeof(this->buf));
}


Buffer::~Buffer()
{
}
