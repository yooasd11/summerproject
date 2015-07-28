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
	JYPlayer* pJYPlayer = (JYPlayer*)JYObjectManager::getInstance()->findObjectByUID(nUID);
	if (pJYPlayer == nullptr) return;
	pJYPlayer->setVelocity(fVelocity);
	pJYPlayer->setDirection(fDirection);
}

REGIST_HANDLER(PACKET_TYPE::PKT_S_STOP, SStopPacketHandler);
void SStopPacketHandler(Packet& p){
	char* PktBody = p.getMsg();
	InGamePacket::S_Stop sStopPacket;
	sStopPacket.ParseFromArray(PktBody, p.getLength());

	UINT nType = sStopPacket.type();
	UINT nUID = sStopPacket.uid();
	float fX = sStopPacket.x();
	float fY = sStopPacket.y();

	//CCLOG("S_Stop received : Type - %d, UID - %d, (%.2f, %.2f)", nType, nUID, fX, fY);
	JYPlayer* pJYPlayer = (JYPlayer*)JYObjectManager::getInstance()->findObjectByUID(nUID);
	if (pJYPlayer == nullptr) return;

	if (nType == JYOBJECT_TYPE::JY_PLAYER){
		pJYPlayer->setVelocity(0.0f);
		pJYPlayer->getCCObject()->setPosition(cocos2d::ccp(fX, fY));
	}
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
	float fX = sCollision.x();
	float fY = sCollision.y();

	JYObject* pJYUID1 = JYObjectManager::getInstance()->findObjectByUID(nUID1);
	if (pJYUID1 == nullptr) return;

	//bullet collision
	if (sCollision.has_th() == true){
		UINT nTh = sCollision.th();
		JYObject* pJYBullet = pJYUID1->getChildByTag(nTh);
		if (pJYBullet == nullptr) return;
		//bullet - player
		if (sCollision.has_uid2() == true){
			UINT nUID2 = sCollision.uid2();
			UINT nHP = sCollision.hp();
			JYPlayer* pJYUID2 = (JYPlayer*)JYObjectManager::getInstance()->findObjectByUID(nUID2);
			if (pJYUID2 == nullptr) return;
			pJYUID2->setHP(nHP);
			CCLOG("Victim - %d, HP - %d", pJYUID2->getUID(), pJYUID2->getHP());
		}
		JYObjectManager::getInstance()->removeObject(pJYBullet);
	}
	//player collision
	else{
		pJYUID1->getCCObject()->setPosition(cocos2d::ccp(fX, fY));
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
