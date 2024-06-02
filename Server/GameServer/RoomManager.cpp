#include "pch.h"
#include "RoomManager.h"
#include "Room.h"
#include "Player.h"

#include <random>
#include <algorithm>

RoomManager GRoomManager;

RoomManager::RoomManager()
{
	// Temp
#ifdef _DEBUG
	
	_roomPool.push_back(MakeShared<Room>(1111));
	_roomPool.push_back(MakeShared<Room>(2222));
	_roomPool.push_back(MakeShared<Room>(3333));
	_roomPool.push_back(MakeShared<Room>(4444));
	_roomPool.push_back(MakeShared<Room>(5555));
	
#else
	for (uint32 i = 1000; i < 10000; i++)
	{
		_roomPool.push_back(MakeShared<Room>(i));
	}
#endif // _DEBUG

	for (uint32 i = 1111; i < 1112; i++)
	{
		_roomPool.push_back(MakeShared<Room>(i));
	}

	std::random_device rd;
	std::mt19937 gen(rd());

	std::shuffle(_roomPool.begin(), _roomPool.end(), gen);
}

void RoomManager::Push(RoomRef room)
{
	WRITE_LOCK;

	room->Init();


	_roomBuffer.push(room);

	if (_roomBuffer.size() == _rooms.size())
	{
		while (_roomBuffer.empty() == false)
		{
			auto roomBuf = _roomBuffer.front();
			_roomBuffer.pop();

			_rooms.erase(roomBuf->_roomCode);
			_roomPool.push_back(roomBuf);
		}
	}

	//_rooms.erase(room->_roomCode);

	//_roomPool.push_back(room);
}

RoomRef RoomManager::Pop()
{
	WRITE_LOCK;

	if (_roomPool.empty())
	{
		while (_roomBuffer.empty() == false)
		{
			auto roomBuf = _roomBuffer.front();
			_roomBuffer.pop();

			_rooms.erase(roomBuf->_roomCode);
			_roomPool.push_back(roomBuf);
		}

		if (_roomPool.empty())
			return nullptr;
	}

	RoomRef room = _roomPool.back();
	_roomPool.pop_back();

	room->Init();

	room->_roomState = Protocol::ROOM_STATE_LOBBY;
	_rooms[room->_roomCode] = room;

	return room;
}

RoomRef RoomManager::CreateRoom()
{
	return Pop();
}

RoomRef RoomManager::GetRoom(uint32 roomCode)
{
	READ_LOCK;

	auto room = this->_rooms.find(roomCode);
	if (room == this->_rooms.end())
		return nullptr;

	return room->second;
}

void RoomManager::LeaveRoom(PlayerRef player)
{
	if (player->_currentRoom == nullptr)
		return;

	WRITE_LOCK;

	auto room = player->_currentRoom;

	// 빈 방 됨
	if (room->Leave(player) == false)
		this->Push(room);
}

void RoomManager::Update()
{
	// Todo
	// 더 좋은 순회 방법 찾기

	WRITE_LOCK;

	for (auto& room : _rooms)
	{
		room.second->Update();
	}
}
