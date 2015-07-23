#pragma once

#define _ADD(PARAM) __##PARAM

enum __Executor
{
	_ADD(MoveExecutor),
	_ADD(MoveRequestExecutor),
	_ADD(CollisionExecutor),
	_ADD(AssaultExecutor),
	__MaxExecutor
};