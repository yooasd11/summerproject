#pragma once
#include "cocos2d.h"
#include <vector>

class JYObject;

class InputHandler{
private:
	InputHandler() { memset(bIsKeyPressed, 0, sizeof(bIsKeyPressed)); }
	~InputHandler() {}
	bool bIsKeyPressed[(int)cocos2d::EventKeyboard::KeyCode::KEY_PLAY + 1];
public:
	static InputHandler* getInstance(){
		static InputHandler Instance;
		return &Instance;
	}

	bool isKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode){
		return bIsKeyPressed[(int)keyCode];
	}

	void pushObject(JYObject* pObject);
	void popObject(JYObject* pObject);
	
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent);
	void onMouseMove(cocos2d::Event* pEvent);
	void onMouseDown(cocos2d::Event* pEvent);
	void onMouseScroll(cocos2d::Event* pEvent);
};