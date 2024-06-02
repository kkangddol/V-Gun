#include "pch.h"
#include "ServerSession.h"
#include "ServerPacketHandler.h"
#include "NetworkManager.h"

ServerSession::~ServerSession()
{
	cout << "~ServerSession" << endl;
}

void ServerSession::OnConnected()
{
	cout << "Connected To Server" << endl;

	// Temp 접속시 로그인
	Protocol::C_LOGIN pkt;
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	this->Send(sendBuffer);
}

void ServerSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	PacketSessionRef session = GetPacketSessionRef();
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

	ServerPacketHandler::HandlePacket(session, buffer, len);
}

void ServerSession::OnSend(int32 len)
{

}

void ServerSession::OnDisconnected()
{
	cout << "Disconnected" << endl;
}
