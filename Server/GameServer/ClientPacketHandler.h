#pragma once
#include "Protocol.pb.h"

using PacketHandlerFunc = std::function<bool(PacketSessionRef&, BYTE*, int32)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

enum : uint16
{
	PKT_C_LOGIN = 1000,
	PKT_S_LOGIN = 1001,
	PKT_C_CREATE_ROOM = 1002,
	PKT_S_CREATE_ROOM = 1003,
	PKT_C_ENTER_ROOM = 1004,
	PKT_S_ENTER_ROOM = 1005,
	PKT_S_ANOTHER_ENTER_ROOM = 1006,
	PKT_C_LEAVE_ROOM = 1007,
	PKT_S_ANOTHER_LEAVE_ROOM = 1008,
	PKT_C_ROOM_SETTING = 1009,
	PKT_S_ROOM_SETTING = 1010,
	PKT_C_ROOM_START = 1011,
	PKT_S_ROOM_START = 1012,
	PKT_C_LOAD_COMPLETE = 1013,
	PKT_S_ROUND_START = 1014,
	PKT_S_SHOP_START = 1015,
	PKT_S_END_GAME = 1016,
	PKT_S_RESULT_START = 1017,
	PKT_C_PLAYER_TRANSFORM = 1018,
	PKT_S_PLAYERS_TRANSFORM = 1019,
	PKT_C_PLAYER_ANIM_STATE = 1020,
	PKT_S_PLAYER_ANIM_STATE = 1021,
	PKT_C_SELECT_PARTS = 1022,
	PKT_S_SELECT_PARTS = 1023,
	PKT_S_SELECT_PARTS_LIST = 1024,
	PKT_C_REROLL_PARTS = 1025,
	PKT_C_RESPAWN_CLICK = 1026,
	PKT_S_SPAWN_POINT = 1027,
	PKT_C_ATTACK_PLAYER = 1028,
	PKT_S_ATTACKED_PLAYER = 1029,
	PKT_S_DEATH_PLAYER = 1030,
	PKT_C_CHAT = 1031,
	PKT_S_CHAT = 1032,
};

// Custom Handlers
bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len);
bool Handle_C_LOGIN(PacketSessionRef& session, Protocol::C_LOGIN& pkt);
bool Handle_C_CREATE_ROOM(PacketSessionRef& session, Protocol::C_CREATE_ROOM& pkt);
bool Handle_C_ENTER_ROOM(PacketSessionRef& session, Protocol::C_ENTER_ROOM& pkt);
bool Handle_C_LEAVE_ROOM(PacketSessionRef& session, Protocol::C_LEAVE_ROOM& pkt);
bool Handle_C_ROOM_SETTING(PacketSessionRef& session, Protocol::C_ROOM_SETTING& pkt);
bool Handle_C_ROOM_START(PacketSessionRef& session, Protocol::C_ROOM_START& pkt);
bool Handle_C_LOAD_COMPLETE(PacketSessionRef& session, Protocol::C_LOAD_COMPLETE& pkt);
bool Handle_C_PLAYER_TRANSFORM(PacketSessionRef& session, Protocol::C_PLAYER_TRANSFORM& pkt);
bool Handle_C_PLAYER_ANIM_STATE(PacketSessionRef& session, Protocol::C_PLAYER_ANIM_STATE& pkt);
bool Handle_C_SELECT_PARTS(PacketSessionRef& session, Protocol::C_SELECT_PARTS& pkt);
bool Handle_C_REROLL_PARTS(PacketSessionRef& session, Protocol::C_REROLL_PARTS& pkt);
bool Handle_C_RESPAWN_CLICK(PacketSessionRef& session, Protocol::C_RESPAWN_CLICK& pkt);
bool Handle_C_ATTACK_PLAYER(PacketSessionRef& session, Protocol::C_ATTACK_PLAYER& pkt);
bool Handle_C_CHAT(PacketSessionRef& session, Protocol::C_CHAT& pkt);

class ClientPacketHandler
{
public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handle_INVALID;
		GPacketHandler[PKT_C_LOGIN] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_LOGIN>(Handle_C_LOGIN, session, buffer, len); };
		GPacketHandler[PKT_C_CREATE_ROOM] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_CREATE_ROOM>(Handle_C_CREATE_ROOM, session, buffer, len); };
		GPacketHandler[PKT_C_ENTER_ROOM] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_ENTER_ROOM>(Handle_C_ENTER_ROOM, session, buffer, len); };
		GPacketHandler[PKT_C_LEAVE_ROOM] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_LEAVE_ROOM>(Handle_C_LEAVE_ROOM, session, buffer, len); };
		GPacketHandler[PKT_C_ROOM_SETTING] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_ROOM_SETTING>(Handle_C_ROOM_SETTING, session, buffer, len); };
		GPacketHandler[PKT_C_ROOM_START] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_ROOM_START>(Handle_C_ROOM_START, session, buffer, len); };
		GPacketHandler[PKT_C_LOAD_COMPLETE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_LOAD_COMPLETE>(Handle_C_LOAD_COMPLETE, session, buffer, len); };
		GPacketHandler[PKT_C_PLAYER_TRANSFORM] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_PLAYER_TRANSFORM>(Handle_C_PLAYER_TRANSFORM, session, buffer, len); };
		GPacketHandler[PKT_C_PLAYER_ANIM_STATE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_PLAYER_ANIM_STATE>(Handle_C_PLAYER_ANIM_STATE, session, buffer, len); };
		GPacketHandler[PKT_C_SELECT_PARTS] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_SELECT_PARTS>(Handle_C_SELECT_PARTS, session, buffer, len); };
		GPacketHandler[PKT_C_REROLL_PARTS] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_REROLL_PARTS>(Handle_C_REROLL_PARTS, session, buffer, len); };
		GPacketHandler[PKT_C_RESPAWN_CLICK] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_RESPAWN_CLICK>(Handle_C_RESPAWN_CLICK, session, buffer, len); };
		GPacketHandler[PKT_C_ATTACK_PLAYER] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_ATTACK_PLAYER>(Handle_C_ATTACK_PLAYER, session, buffer, len); };
		GPacketHandler[PKT_C_CHAT] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_CHAT>(Handle_C_CHAT, session, buffer, len); };
	}

	static bool HandlePacket(PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}
	static SendBufferRef MakeSendBuffer(Protocol::S_LOGIN& pkt) { return MakeSendBuffer(pkt, PKT_S_LOGIN); }
	static SendBufferRef MakeSendBuffer(Protocol::S_CREATE_ROOM& pkt) { return MakeSendBuffer(pkt, PKT_S_CREATE_ROOM); }
	static SendBufferRef MakeSendBuffer(Protocol::S_ENTER_ROOM& pkt) { return MakeSendBuffer(pkt, PKT_S_ENTER_ROOM); }
	static SendBufferRef MakeSendBuffer(Protocol::S_ANOTHER_ENTER_ROOM& pkt) { return MakeSendBuffer(pkt, PKT_S_ANOTHER_ENTER_ROOM); }
	static SendBufferRef MakeSendBuffer(Protocol::S_ANOTHER_LEAVE_ROOM& pkt) { return MakeSendBuffer(pkt, PKT_S_ANOTHER_LEAVE_ROOM); }
	static SendBufferRef MakeSendBuffer(Protocol::S_ROOM_SETTING& pkt) { return MakeSendBuffer(pkt, PKT_S_ROOM_SETTING); }
	static SendBufferRef MakeSendBuffer(Protocol::S_ROOM_START& pkt) { return MakeSendBuffer(pkt, PKT_S_ROOM_START); }
	static SendBufferRef MakeSendBuffer(Protocol::S_ROUND_START& pkt) { return MakeSendBuffer(pkt, PKT_S_ROUND_START); }
	static SendBufferRef MakeSendBuffer(Protocol::S_SHOP_START& pkt) { return MakeSendBuffer(pkt, PKT_S_SHOP_START); }
	static SendBufferRef MakeSendBuffer(Protocol::S_END_GAME& pkt) { return MakeSendBuffer(pkt, PKT_S_END_GAME); }
	static SendBufferRef MakeSendBuffer(Protocol::S_RESULT_START& pkt) { return MakeSendBuffer(pkt, PKT_S_RESULT_START); }
	static SendBufferRef MakeSendBuffer(Protocol::S_PLAYERS_TRANSFORM& pkt) { return MakeSendBuffer(pkt, PKT_S_PLAYERS_TRANSFORM); }
	static SendBufferRef MakeSendBuffer(Protocol::S_PLAYER_ANIM_STATE& pkt) { return MakeSendBuffer(pkt, PKT_S_PLAYER_ANIM_STATE); }
	static SendBufferRef MakeSendBuffer(Protocol::S_SELECT_PARTS& pkt) { return MakeSendBuffer(pkt, PKT_S_SELECT_PARTS); }
	static SendBufferRef MakeSendBuffer(Protocol::S_SELECT_PARTS_LIST& pkt) { return MakeSendBuffer(pkt, PKT_S_SELECT_PARTS_LIST); }
	static SendBufferRef MakeSendBuffer(Protocol::S_SPAWN_POINT& pkt) { return MakeSendBuffer(pkt, PKT_S_SPAWN_POINT); }
	static SendBufferRef MakeSendBuffer(Protocol::S_ATTACKED_PLAYER& pkt) { return MakeSendBuffer(pkt, PKT_S_ATTACKED_PLAYER); }
	static SendBufferRef MakeSendBuffer(Protocol::S_DEATH_PLAYER& pkt) { return MakeSendBuffer(pkt, PKT_S_DEATH_PLAYER); }
	static SendBufferRef MakeSendBuffer(Protocol::S_CHAT& pkt) { return MakeSendBuffer(pkt, PKT_S_CHAT); }

private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketType pkt;
		if (pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)) == false)
			return false;

		return func(session, pkt);
	}

	template<typename T>
	static SendBufferRef MakeSendBuffer(T& pkt, uint16 pktId)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

		SendBufferRef sendBuffer = GSendBufferManager->Open(packetSize);
		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;
		ASSERT_CRASH(pkt.SerializeToArray(&header[1], dataSize));
		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
};