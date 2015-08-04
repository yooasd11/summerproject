// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define _WINSOCKAPI_

#define BUFSIZE (1<<16)
#define READ 3
#define WRITE 5
#define ACCOUNT 2
#define MAXGETTICK (1<<28)
#define PI 3.14159265

////
#define Object_USER 1
#define Object_BULLET 2
#define Object_NPC 3

/////////////////////게임내부
#define DISTANCE 10
#define DAMAGE_DISTANCE 50
#define BULLET_VELOCITY 200
#define BULLET_DAMAGE_1 50
#define AI_NEXT_TICK 3000
#define BULLETNEXTTICK 30
#define NEXT_TICK 30

///////////////맵
#define WIDTH 1500
#define HEIGHT 700
#define LAND 80
#define GRAVITY -350

////////////////////////
#define AI_ATTACK_DELAY 3000
#define AI_VELOCITY 50
#define AI_MOVE_DELAY 30
#define AI_DEAD_DELAY 10000
#define AI_DIRECTION_DELAY 10000
#define NPC_STATUS_ALIVE 1
#define NPC_STATUS_DEAD 2
#define USER_VELOCITY 50
#define LOCKING(key) LockHelper lockHelper (key);
//#define LOCK(key) LockHelper lockHelper (key);

void ErrorHandling(char *msg);

#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <windows.h>
#include <process.h>
#include <WinNT.h>
#include <functional>
#include <ctime>
#include <math.h>

#include <string>
#include <queue>
#include <vector>
#include <list>
#include <map>
#include <memory>

//#include "Lock.h"
//#include "OBJECT.h"
//#include "USER.h"
//#include "NPC.h"
//#include "BULLET.h"
//#include "IoData.h"
//#include "HandleData.h"
//#include "TimerJob.h"
//
//#include "NPC_STATE.h"
//#include "ObjectManager.h"
//#include "ClientHandle.h"
//#include "SocketListener.h"
//#include "Packet.h"
//#include "PacketHandler.h"
//#include "IocpConstructor.h"


#include "Lock.h"
#include "OBJECT.h"
#include "USER.h"
#include "NPC.h"
#include "NPC_STATE.h"
#include "BULLET.h"
#include "ObjectManager.h"
#include "TimerJob.h"
#include "IoData.h"
#include "HandleData.h"
#include "packet.h"
#include "PacketHandler.h"
#include "ClientHandle.h"
#include "SocketListener.h"
#include "IocpConstructor.h"

