#include "main.h"
#include "AppDelegate.h"
#include "cocos2d.h"
#include "ConnectionManager.h"

USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	char SERVER_IP_ADDRESS[100];
	int SERVER_PORT_NUMBER = 9200;
	int retConvert = WideCharToMultiByte(CP_ACP, 0, lpCmdLine, -1, SERVER_IP_ADDRESS, 128, NULL, NULL);
	ConnectionManager::getInstance()->accountTo(SERVER_IP_ADDRESS, SERVER_PORT_NUMBER);
    // create the application instance
    AppDelegate app;
    return Application::getInstance()->run();
}
