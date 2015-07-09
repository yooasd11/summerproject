// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define MAX_MESSAGE (1<<12)
#define READ 3
#define WRITE 5
#define MAXCOUNT 1000
#define BUF_SIZE 1<<19

#define Player_ 1
#define NPC_ 2
#define Monster_ 3

#define _WINSOCKAPI_
#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <WinSock2.h>
#include <string>
#include <vector>
#include <map>
#include "Locking.h"

#include "datablock.h"
#include "ManageClient.h"
#include "HandleClient.h"
#include "Iocp.h"



// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
