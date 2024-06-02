#include "pch.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "ClientPacketHandler.h"
#include "Player.h"
#include "RoomManager.h"

void GameSession::OnConnected()
{
	GSessionManager.Add(static_pointer_cast<GameSession>(shared_from_this()));
}

void GameSession::OnDisconnected()
{
	if(_player != nullptr)
		GRoomManager.LeaveRoom(_player);
	
	GSessionManager.Remove(static_pointer_cast<GameSession>(shared_from_this()));
}

void GameSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	PacketSessionRef session = GetPacketSessionRef();
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

	// TODO ��Ŷ ID �뿪 üũ - Ŭ�� ���� or DB
	ClientPacketHandler::HandlePacket(session, buffer, len);
}

void GameSession::OnSend(int32 len)
{
	//cout << "OnSend Len : " << len << endl;
}
