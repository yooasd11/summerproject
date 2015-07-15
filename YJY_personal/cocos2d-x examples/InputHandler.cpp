#include "Inputhandler.h"
#include "JYObject.h"

void InputHandler::pushObject(JYObject* pObject){
	if (pObject == nullptr) return;
	for (int i = 0; i < JYObjectArray.size(); ++i){
		if (JYObjectArray[i] == pObject)
			return;
	}
	JYObjectArray.push_back(pObject);
}

void InputHandler::popObject(JYObject* pObject){
	for (auto it = JYObjectArray.begin(); it < JYObjectArray.end(); ++it){
		if (*it == pObject) {
			JYObjectArray.erase(it);
			break;
		}
	}
}

void InputHandler::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
	bIsKeyPressed[(int)keyCode] = true;
	for (auto it = JYObjectArray.begin(); it < JYObjectArray.end(); ++it){
		(*it)->onKeyPressed(keyCode, pEvent);
	}
}

void InputHandler::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* pEvent){
	bIsKeyPressed[(int)keyCode] = false;
	for (auto it = JYObjectArray.begin(); it < JYObjectArray.end(); ++it){
		(*it)->onKeyReleased(keyCode, pEvent);
	}
}

void InputHandler::onMouseMove(cocos2d::Event* pEvent){
	for (auto it = JYObjectArray.begin(); it < JYObjectArray.end(); ++it){
		(*it)->onMouseMove(pEvent);
	}
}

void InputHandler::tick(float fDeltaTime){
	for (auto it = JYObjectArray.begin(); it < JYObjectArray.end(); ++it){
		(*it)->tick(fDeltaTime);
	}
}

