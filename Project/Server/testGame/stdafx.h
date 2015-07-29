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
#define LEFT 1
#define SPEED 5
#define ACCOUNT 2
#define WAIT 1
#define MOVE 2
#define ATTACK 3
#define MAXGETTICK (1<<28)
#define PI 3.14159265
#define PLAYER 1
#define BULLET 2

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

#include "Lock.h"
#include "IoData.h"
#include "HandleData.h"
#include "USER.h"
#include "bullet.h"
#include "TimerJob.h"
#include "AI.h"
#include "NPCManager.h"
#include "GameManager.h"
#include "CollisionObject.h"
#include "ObjectManager.h"
#include "ClientHandle.h"
#include "ClientManager.h"
#include "SocketListener.h"
#include "Packet.h"
#include "PacketHandler.h"
#include "IocpConstructor.h"

