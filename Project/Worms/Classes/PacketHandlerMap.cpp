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

	MyScene* pMyScene = GET_MYSCENE
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
		UINT nUID = mAccount.uid();
		UINT nHP = mAccount.hp();
		float fX = mAccount.x();
		float fY = mAccount.y();

		if (JYObjectManager::getInstance()->findObjectByUID(nUID) == nullptr){
			MyScene* pMyScene = GET_MYSCENE
			pMyScene->createDragon(nUID, ccp(fX, fY));
		}
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
	CCLOG("S_Move from server : UID - %d, X - %.1f, Y - %.1f, V - %.1f", nUID, fX, fY, fVelocity);
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

	UINT nUID = sStopPacket.uid();
	float fX = sStopPacket.x();
	float fY = sStopPacket.y();

	CCLOG("Position from server : (%.2f, %.2f)", fX, fY);
	JYPlayer* pJYPlayer = (JYPlayer*)JYObjectManager::getInstance()->findObjectByUID(nUID);
	if (pJYPlayer == nullptr) return;
	pJYPlayer->setVelocity(0.0f);
	pJYPlayer->getCCObject()->setPosition(cocos2d::ccp(fX, fY));
}

REGIST_HANDLER(PACKET_TYPE::PKT_S_SHOOT, SShootPacketHandler);
void SShootPacketHandler(Packet& p){
	char* PktBody = p.getMsg();
	InGamePacket::S_Shoot sShootPacket;
	sShootPacket.ParseFromArray(PktBody, p.getLength());

	UINT nUID = sShootPacket.uid();
	float fX = sShootPacket.x();
	float fY = sShootPacket.y();
	float fDirection = sShootPacket.direction();
	float fVelocity = sShootPacket.velocity();
	float fDamage = sShootPacket.damage();

	CCLOG("Bullet shot : UID - %d, (%.2f, %.2f)", nUID, fX, fY);
	JYPlayer* pJYPlayer = (JYPlayer*)JYObjectManager::getInstance()->findObjectByUID(nUID);
	if (pJYPlayer == nullptr) return;
	JYArm* pJYBullet = (JYArm*)pJYPlayer->getChildByName("JYBullet");
	if (pJYBullet == nullptr) return;
	cocos2d::CCNode* pCCPlayer = pJYPlayer->getCCObject();
	if (pCCPlayer == nullptr) return;
	pCCPlayer->setPosition(cocos2d::ccp(fX, fY));
	pJYBullet->setDirection(fDirection);
	pJYBullet->setVelocity(fVelocity);
	pJYBullet->setDamage(fDamage);
}

REGIST_HANDLER(PACKET_TYPE::PKT_S_DISCONNECT, SDisconnectHandler);
void SDisconnectHandler(Packet& p){
	char* PktBody = p.getMsg();
	AccountPacket::S_Account_List::Disconnect sDisconnect;
	sDisconnect.ParseFromArray(PktBody, p.getLength());
	
	UINT nUID = sDisconnect.uid();

	MyScene* pMyScene = GET_MYSCENE
	pMyScene->deleteDragon(nUID);
}

REGIST_HANDLER(PACKET_TYPE::PKT_END, EndHandler);
void EndHandler(Packet& p){
	
}
