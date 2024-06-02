#pragma once
#include "RocketAPILoader.h"
#include "Types.h"
#include "Enum.pb.h"
#include "Struct.pb.h"

class ClientService;

using ClientServiceRef = std::shared_ptr<ClientService>;

class NetworkManager : public RocketEngine::Component
{
public:
	static NetworkManager& Instance();

private:
	NetworkManager(RocketEngine::GameObject* owner);

public:
	virtual void Start() override;

	virtual void Finalize() override;

	// void Start();
	virtual void Update() override;

public: //
	bool IsConnected();

public:
	void SendMyTransfrom(RocketEngine::RMFLOAT3 position, RocketEngine::RMQuaternion rotation);
	void SendChangeAnimationState(Protocol::AnimState animState);

	void SendAttackPlayer(RocketEngine::RMFLOAT3 position, RocketEngine::RMQuaternion rotation, uint64 id = 0, uint32 index = 0, Protocol::AttackPoint attackPoint = Protocol::ATTACK_POINT_NONE);

public: // Click
	void OnClickCreateRoomButton();
	void OnClickEnterRoomButton(int code);
	void OnClickGameStartButton();

	void OnClickChangeRoomSetting(int count, int time);
	void OnClickExitRoom();

	void OnClickSelectParts(int index);
	void OnClickRerollShopParts();
	
	void OnClickRespawnButton();

private:
	void WriteTransform(Protocol::Transform* transformPkt, const RocketEngine::RMFLOAT3& position, const RocketEngine::RMQuaternion& rotation);

private:
	bool isConnected;
	ClientServiceRef _service;
};