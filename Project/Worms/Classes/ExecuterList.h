#pragma once

#define _ADD(PARAM) __##PARAM

enum __Executer
{
	_ADD(MoveExecuter),
	_ADD(MoveRequestExecuter),
	_ADD(CollisionExecuter),
	_ADD(AssaultExecuter),
	_ADD(AccelerationExecuter),
	__Executer_END
};