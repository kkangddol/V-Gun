#pragma once

class Room;

using RoomRef = std::shared_ptr<Room>;

class RoomManager
{
public:
	RoomManager();

	void Push(RoomRef room);
	RoomRef Pop();

	RoomRef CreateRoom();
	RoomRef GetRoom(uint32 roomCode);

	void LeaveRoom(PlayerRef player);

	void Update();
public:
	USE_LOCK;

	Map<uint32, RoomRef> _rooms;
	Queue<RoomRef> _roomBuffer;

private:
	Vector<RoomRef> _roomPool;
};

extern RoomManager GRoomManager;