// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define _WINSOCKAPI_

#define BUFSIZE (1<<16)
#define READ 3
#define WRITE 5
#define ACCOUNT 2
#define S_MOVE 3
#define C_MOVE 4
#define RIGHT 0
#define LEFT 1
#define SPEED 5

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

#include <string>
#include <queue>
#include <vector>
#include <list>
#include <map>

#include "Lock.h"
#include "IoData.h"
#include "HandleData.h"
#include "USER.h"
#include "TimerJob.h"
#include "NPC.h"
#include "NPCManager.h"
#include "GameManager.h"
#include "ClientHandle.h"
#include "ClientManager.h"
#include "SocketListener.h"
#include "IocpConstructor.h"