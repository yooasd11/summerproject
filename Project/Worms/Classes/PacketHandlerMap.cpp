#include "PacketHandlerMap.h"
#include "cocos2d.h"
#include "AccountPacket.pb.h"
#include "InGamePacket.pb.h"
#include "MyScene.h"
#include "JYPlayer.h"
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
	UINT AccountSize;
	memcpy(&AccountSize, PktBody, sizeof(UINT));
	AccountPacket::S_Account sAccountPacket;
	for (UINT i = 0; i < AccountSize; ++i){
		sAccountPacket.ParseFromArray(PktBody + sizeof(UINT) + i*sAccountPacket.ByteSize() , sAccountPacket.ByteSize());

		UINT nUid = sAccountPacket.uid();
		UINT nHP = sAccountPacket.hp();
		UINT nPosX = sAccountPacket.x();
		UINT nPosY = sAccountPacket.y();

		CCLOG("Account result : UID - %d, HP - %d, X - %d, Y - %d", nUid, nHP, nPosX, nPosY);

		cocos2d::CCScene* pNowScene = cocos2d::CCDirector::getInstance()->getRunningScene();
		MyScene* pMyScene = (MyScene*)pNowScene->getChildByName("MyScene");
		pMyScene->createDragon(nUid, ccp((float)nPosX, (float)nPosY));
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
	cocos2d::CCScene* pNowScene = cocos2d::CCDirector::getInstance()->getRunningScene();
	MyScene* pMyScene = (MyScene*)pNowScene->getChildByName("MyScene");
	pMyScene->pJYPlayerDragon->getCCObject()->setPosition(ccp(fX, fY));
}

REGIST_HANDLER(PACKET_TYPE::PKT_END, EndHandler);
void EndHandler(Packet& p){
	
}
