#pragma once

#define _ADD(PARAM) __##PARAM

enum __Executor
{
	_ADD(MoveExecutor),
	_ADD(CollisionExecutor),
	__MaxExecutor
};