// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define MaxClient 1000
#define BufSize (1<<19)
#define READ 3
#define WRITE 5

#define _WINSOCKAPI_

void ErrorHandling(char *msg);

#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>
#include <WinSock2.h>
#include <string>
#include <vector>
#include <list>
#include <map>

#include "lock.h"
#include "IoData.h"
#include "HandleData.h"
#include "Buffer.h"
#include "ClientData.h"
#include "ClientHandle.h"
#include "ClientManager.h"
#include "ConnectSocket.h"
#include "Iocp.h"





