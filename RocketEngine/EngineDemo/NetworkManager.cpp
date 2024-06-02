#include "pch.h"
#include "NetworkManager.h"
#include "RocketAPILoader.h"
#include "GameManager.h"
#include "LobbyManager.h"
#include "PlayerInfo.h"

#include "Service.h"
#include "ServerPacketHandler.h"
#include "ServerSession.h"

#include <iostream>
#include <map>

#include <fstream>
#include <sstream>

const std::string IP_ADDRESS_FILE = "../Server/serverIP.txt";

static RocketEngine::GameObject* GO = nullptr;

NetworkManager& NetworkManager::Instance()
{
	static NetworkManager instance(GO);
	return instance;
}

NetworkManager::NetworkManager(RocketEngine::GameObject* owner)
	: RocketEngine::Component(owner)
{
	RocketEngine::AddStaticComponent(this);
}

void NetworkManager::Start()
{
	ServerPacketHandler::Init();

	std::wifstream ipAddressFile(IP_ADDRESS_FILE);

	std::wstring ipAddressStr = L"";
	if (ipAddressFile.is_open())
	{
		std::getline(ipAddressFile, ipAddressStr);
	}

	_service = MakeShared<ClientService>(
		//NetAddress(L"172.16.1.218", 7777),
		//NetAddress(L"127.0.0.1", 7777),
		//NetAddress(L"222.121.81.67", 7777),
		NetAddress(ipAddressStr,7777),
		MakeShared<IocpCore>(),
		MakeShared<ServerSession>,
		1
	);

	this->isConnected = _service->Start();

// 	DEBUG_CODE(
// 		AllocConsole();
// 	FILE * pCout;
// 	freopen_s(&pCout, "CONOUT$", "w", stdout);
// 	);
// 
// 	if (isConnected == true)
// 	{
// 		// Temp
// 		DEBUG_CODE(
// 			{
// 			std::wcout << L"Connect " << _service->GetNetAddress().GetIpAddr() << " : " << _service->GetNetAddress().GetPortAddr() << std::endl;
// 			}
// 		);
// 	}
// 	else
// 	{
// 		std::wcout << L"우린 안될거야 " << _service->GetNetAddress().GetIpAddr() << " : " << _service->GetNetAddress().GetPortAddr() << std::endl;
// 	}
}

void NetworkManager::Finalize()
{
	//_service = nullptr;
}

void NetworkManager::Update()
{
	if (isConnected == true)
		_service->GetIocpCore()->Dispatch(0);
}

void NetworkManager::SendMyTransfrom(RocketEngine::RMFLOAT3 lastPosition, RocketEngine::RMQuaternion lastRotation)
{
	Protocol::C_PLAYER_TRANSFORM playerTransformPkt;

	auto position = playerTransformPkt.mutable_transform()->mutable_position();
	auto quaternion = playerTransformPkt.mutable_transform()->mutable_quaternion();

	position->set_x(lastPosition.x);
	position->set_y(lastPosition.y);
	position->set_z(lastPosition.z);

	quaternion->set_w(lastRotation.w);
	quaternion->set_x(lastRotation.x);
	quaternion->set_y(lastRotation.y);
	quaternion->set_z(lastRotation.z);

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(playerTransformPkt);
	_service->BroadCast(sendBuffer);
}

// Todo 변수 추가해도됨
void NetworkManager::SendChangeAnimationState(Protocol::AnimState animState)
{
	Protocol::C_PLAYER_ANIM_STATE playerAnimStatePkt;

	// Todo 이거 하나만 채우면 됨
	// Temp
	playerAnimStatePkt.set_state(animState);


	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(playerAnimStatePkt);
	_service->BroadCast(sendBuffer);
}

void NetworkManager::SendAttackPlayer(RocketEngine::RMFLOAT3 position, RocketEngine::RMQuaternion rotation, uint64 id, uint32 index, Protocol::AttackPoint attackPoint)
{
	Protocol::C_ATTACK_PLAYER pkt;

	pkt.mutable_attackedplayer()->set_id(id);
	pkt.mutable_attackedplayer()->set_index(index);
	pkt.set_attackpoint(attackPoint);

	this->WriteTransform(pkt.mutable_transform(), position, rotation);

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	_service->BroadCast(sendBuffer);
}

bool NetworkManager::IsConnected()
{
	return isConnected;
}

void NetworkManager::OnClickCreateRoomButton()
{
	Protocol::C_CREATE_ROOM createRoomPkt;

	GameManager::Instance().InitGameManager();

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(createRoomPkt);
	_service->BroadCast(sendBuffer);
}

void NetworkManager::OnClickEnterRoomButton(int code)
{
	Protocol::C_ENTER_ROOM enterRoomPkt;

	GameManager::Instance().InitGameManager();
	enterRoomPkt.set_roomcode(code);
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(enterRoomPkt);
	_service->BroadCast(sendBuffer);
}

void NetworkManager::OnClickGameStartButton()
{
	Protocol::C_ROOM_START roomStartPkt;

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(roomStartPkt);
	_service->BroadCast(sendBuffer);
}

void NetworkManager::OnClickChangeRoomSetting(int count, int time)
{
	Protocol::C_ROOM_SETTING roomSettingPkt;

	auto roomInfo = roomSettingPkt.mutable_roominfo();
	roomInfo->set_roundkill(count);
	roomInfo->set_roundtime(time);
	roomInfo->set_roomcode(LobbyManager::Instance().GetRoomCode());

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(roomSettingPkt);
	_service->BroadCast(sendBuffer);
}

void NetworkManager::OnClickExitRoom()
{
	Protocol::C_LEAVE_ROOM leaveRoomPkt;

	GameManager::Instance().InitGameManager();

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(leaveRoomPkt);
	_service->BroadCast(sendBuffer);
}

void NetworkManager::OnClickSelectParts(int index)
{
	auto& gameManager = GameManager::Instance();
	if (gameManager.GetPlayerInfo(gameManager.myId)->isDie == false)
		return;

	if (index < 0 || index > gameManager.selectPartsList.size())
		return;

	Protocol::C_SELECT_PARTS selectPartsPkt;

	selectPartsPkt.set_index((uint32)index);

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(selectPartsPkt);
	_service->BroadCast(sendBuffer);
}

void NetworkManager::OnClickRerollShopParts()
{
	auto& gameManager = GameManager::Instance();
	if (gameManager.GetPlayerInfo(gameManager.myId)->isDie == false)
		return;

	Protocol::C_REROLL_PARTS pkt;

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	_service->BroadCast(sendBuffer);
}

void NetworkManager::OnClickRespawnButton()
{
	auto& gameManager = GameManager::Instance();
	if (gameManager.GetPlayerInfo(gameManager.myId)->isDie == false)
		return;

	Protocol::C_RESPAWN_CLICK pkt;

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	_service->BroadCast(sendBuffer);
}

void NetworkManager::WriteTransform(Protocol::Transform* transformPkt, const RocketEngine::RMFLOAT3& position, const RocketEngine::RMQuaternion& rotation)
{
	auto positionPkt = transformPkt->mutable_position();
	auto rotationPkt = transformPkt->mutable_quaternion();

	positionPkt->set_x(position.x);
	positionPkt->set_y(position.y);
	positionPkt->set_z(position.z);

	rotationPkt->set_w(rotation.w);
	rotationPkt->set_x(rotation.x);
	rotationPkt->set_y(rotation.y);
	rotationPkt->set_z(rotation.z);
}
