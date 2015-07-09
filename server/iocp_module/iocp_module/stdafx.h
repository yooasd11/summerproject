// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
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





