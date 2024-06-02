#include "pch.h"
#include <iostream>

#include "ThreadManager.h"

#include "Service.h"
#include "Session.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "ClientPacketHandler.h"
#include "Protocol.pb.h"
#include "RoomManager.h"
#include "VGun.h"
#include "Parts.h"

#include "Player.h"
#include <fstream>

int main()
{
	const std::string IP_ADDRESS_FILE = "../serverIP.txt";
	std::wifstream ipAddressFile(IP_ADDRESS_FILE);

	std::wstring ipAddressStr = L"";
	if (ipAddressFile.is_open())
	{
		std::getline(ipAddressFile, ipAddressStr);
	}
	// Todo System 초기화 한번에 묶기
	VGun::Init();
	Parts::Init();


	ClientPacketHandler::Init();

	ServerServiceRef service = MakeShared<ServerService>(
		NetAddress(ipAddressStr, 7777),
		MakeShared<IocpCore>(),
		MakeShared<GameSession>,
		10
	);

	ASSERT_CRASH(service->Start());

	std::cout << "Server Running..." << std::endl;

	for (int32 i = 0; i < 10; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}
			});
	}

	for (int32 i = 0; i < 1; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					GRoomManager.Update();
				}
			});
	}

	GThreadManager->Join();

	return 0;
}