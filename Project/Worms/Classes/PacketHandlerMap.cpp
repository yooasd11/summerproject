#include "PacketHandlerMap.h"
#include "cocos2d.h"
#include "AccountPacket.pb.h"
#include "InGamePacket.pb.h"
#include "MyScene.h"
#include "JYPlayer.h"
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

	MyScene* pMyScene = (MyScene*)cocos2d::CCDirector::getInstance()->getRunningScene()->getChildByName("MyScene");
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

		MyScene* pMyScene = (MyScene*)cocos2d::CCDirector::getInstance()->getRunningScene()->getChildByName("MyScene");
		pMyScene->createDragon(nUID, ccp(fX, fY));
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
	float fVelocity = sMovePacket.velocity();

	JYPlayer* pJYPlayer = (JYPlayer*)JYObjectManager::getInstance()->findObjectByUID(nUID);
	pJYPlayer->getCCObject()->setPosition(cocos2d::ccp(fX, fY));
	pJYPlayer->setVelocity(fVelocity);
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
	pJYPlayer->setVelocity(0.0f);
}

REGIST_HANDLER(PACKET_TYPE::PKT_END, EndHandler);
void EndHandler(Packet& p){
	
}
