// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
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



// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
