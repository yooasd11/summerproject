// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define MaxClient 1000
#define MaxNPC 100
#define BufSize (1<<19)
#define WAITING 2
#define START 4
#define READ 3
#define WRITE 5
#define TIMER 7

#define _WINSOCKAPI_

void ErrorHandling(char *msg);

#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <windows.h>
#include <process.h>
#include <WinNT.h>
#include <functional>

#include <string>
#include <queue>
#include <vector>
#include <list>
#include <map>

#include "../character.pb.h"

#include "Lock.h"
#include "IoData.h"
#include "HandleData.h"
#include "Buffer.h"
#include "ClientData.h"
#include "ClientHandle.h"
#include "ClientManager.h"
#include "NPC.h"
#include "NPCManager.h"
#include "GameManager.h"
#include "TimerJob.h"
#include "ConnectSocket.h"
#include "Iocp.h"




