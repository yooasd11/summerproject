#include "PacketHandlerMap.h"
#include "cocos2d.h"
#include "AccountPacket.pb.h"
#include "InGamePacket.pb.h"
#include "MyScene.h"
#include "JYPlayer.h"
#include "JYArm.h"
#include "JYObjectManager.h"
#include <vector>


void PacketHandlerMap::registHandler(PACKET_TYPE eType, PacketHandler func)
{
	m_packetHandlerTable[eType] = func;
}

void PacketHandlerMap::unregistHandler(int type){
	m_packetHandlerTable[type] = nullptr;
}

bool PacketHandlerMap::handlePacket(Packet& p){
	int type = p.getType();
	if (m_packetHandlerTable[type] == nullptr){
		CCLOG("Handler not registered for type %d\n", type);
		return false;
	}
	m_packetHandlerTable[type](p);
	return true;
}

//command registry
//macro defined
#define REGIST_HANDLER(PACKET_TYPE, PACKET_HANDLE) \
	void PACKET_HANDLE(Packet& p); \
	HandlerRegistHelper Regist##PACKET_HANDLE(PACKET_TYPE, PACKET_HANDLE);

REGIST_HANDLER(PACKET_TYPE::PKT_ECHO, EchoPacketHandler);
void EchoPacketHandler(Packet& p){
	char* PktBody = p.getMsg();
	CCLOG("%s\n", PktBody);
}

REGIST_HANDLER(PACKET_TYPE::PKT_ACCOUNT, AccountPacketHandler);
void AccountPacketHandler(Packet& p){
	char* PktBody = p.getMsg();
	UINT nUID;
	memcpy(&nUID, PktBody, sizeof(UINT));

	MyScene* pMyScene = GET_MYSCENE;
	pMyScene->nPlayerUID = nUID;
}

REGIST_HANDLER(PACKET_TYPE::PKT_ACCOUNT_LIST, AccountListPacketHandler);
void AccountListPacketHandler(Packet& p){
	char* PktBody = p.getMsg();
	AccountPacket::S_Account_List sAccountList;
	sAccountList.ParseFromArray(PktBody, p.getLength());
	USHORT nSize = sAccountList.account_member_size();
	for (USHORT i = 0; i < nSize; ++i){
		AccountPacket::S_Account_List::Account mAccount = sAccountList.account_member(i);
		MyScene* pMyScene = GET_MYSCENE;
		pMyScene->createDragon(mAccount);
	}
}

REGIST_HANDLER(PACKET_TYPE::PKT_S_MOVE, SMovePacketHandler);
void SMovePacketHandler(Packet& p){
	char* PktBody = p.getMsg();
	InGamePacket::S_Move sMovePacket;
	sMovePacket.ParseFromArray(PktBody, p.getLength());
	
	UINT nUID = sMovePacket.uid();
	float fX = sMovePacket.x();
	float fY = sMovePacket.y();
	float fDirection = sMovePacket.direction();
	float fVelocity = sMovePacket.velocity();

	//CCLOG("S_Move from server : UID - %d, X - %.1f, Y - %.1f, V - %.1f", nUID, fX, fY, fVelocity);
	JYPlayer* pJYObject = (JYPlayer*)JYObjectManager::getInstance()->findObjectByUID(nUID);
	if (pJYObject == nullptr) return;

	pJYObject->setVelocity(fVelocity);
	pJYObject->setDirection(fDirection);
}

REGIST_HANDLER(PACKET_TYPE::PKT_S_STOP, SStopPacketHandler);
void SStopPacketHandler(Packet& p){
	char* PktBody = p.getMsg();
	InGamePacket::S_Stop sStopPacket;
	sStopPacket.ParseFromArray(PktBody, p.getLength());

	UINT nUID = sStopPacket.uid();
	float fX = sStopPacket.x();
	float fY = sStopPacket.y();

	//CCLOG("S_Stop received : Type - %d, UID - %d, (%.2f, %.2f)", nType, nUID, fX, fY);
	JYObject* pJYObject = JYObjectManager::getInstance()->findObjectByUID(nUID);
	if (pJYObject == nullptr) return;

	pJYObject->setVelocity(0.0f);
	pJYObject->getCCObject()->setPosition(cocos2d::ccp(fX, fY));
}

REGIST_HANDLER(PACKET_TYPE::PKT_S_SHOOT, SShootPacketHandler);
void SShootPacketHandler(Packet& p){
	char* PktBody = p.getMsg();
	InGamePacket::S_Shoot sShootPacket;
	sShootPacket.ParseFromArray(PktBody, p.getLength());

	//CCLOG("Bullet shot : UID - %d, (%.2f, %.2f)", nUID, fX, fY);
	MyScene* pMyScene = GET_MYSCENE;
	pMyScene->createBullet(sShootPacket);
}

REGIST_HANDLER(PACKET_TYPE::PKT_S_COLLISION, SCollisionHandler);
void SCollisionHandler(Packet& p){
	char* PktBody = p.getMsg();
	InGamePacket::S_Collision sCollision;
	sCollision.ParseFromArray(PktBody, p.getLength());

	UINT nUID1 = sCollision.uid1();

	JYObject* pJYObject1 = JYObjectManager::getInstance()->findObjectByUID(nUID1);
	if (pJYObject1 == nullptr) return;

	JYOBJECT_TYPE eType1 = pJYObject1->getObjectType();

	//player collision
	if (eType1 == JYOBJECT_TYPE::JY_PLAYER){
		float fX = sCollision.x();
		float fY = sCollision.y();
		pJYObject1->getCCObject()->setPosition(cocos2d::ccp(fX, fY));
	}
	//bullet collision
	else if (eType1 == JYOBJECT_TYPE::JY_ARM){
		//bullet - object
		if (sCollision.has_uid2() == true){
			UINT nUID2 = sCollision.uid2();
			JYObject* pJYObject2 = JYObjectManager::getInstance()->findObjectByUID(nUID2);
			if (pJYObject2 == nullptr) return;

			JYOBJECT_TYPE eType2 = pJYObject2->getObjectType();
			
			//bullet - player
			if (eType2 == JYOBJECT_TYPE::JY_PLAYER){
				UINT nHP = sCollision.hp();
				((JYPlayer*)pJYObject2)->setHP(nHP);
			}
			//bullet - bullet
			else{
				JYObjectManager::getInstance()->removeObject(pJYObject2);
			}
		}
		JYObjectManager::getInstance()->removeObject(pJYObject1);
	}
}

REGIST_HANDLER(PACKET_TYPE::PKT_S_DISCONNECT, SDisconnectHandler);
void SDisconnectHandler(Packet& p){
	char* PktBody = p.getMsg();
	AccountPacket::S_Account_List::Disconnect sDisconnect;
	sDisconnect.ParseFromArray(PktBody, p.getLength());
	
	UINT nUID = sDisconnect.uid();
	
	JYObject* pTarget = JYObjectManager::getInstance()->findObjectByUID(nUID);
	if (pTarget != nullptr){
		JYObjectManager::getInstance()->removeObject(pTarget);
	}
}

REGIST_HANDLER(PACKET_TYPE::PKT_END, EndHandler);
void EndHandler(Packet& p){
	
}
