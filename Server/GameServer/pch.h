#pragma once

#define WIN32_LEAN_AND_MEAN

#ifdef _DEBUG

#pragma comment(lib,"ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib,"ProtoBuf\\Debug\\libprotobufd.lib")

#else
#pragma comment(lib,"ServerCore\\Release\\ServerCore.lib")
#pragma comment(lib,"ProtoBuf\\Release\\libprotobuf.lib")

#endif

#include "CorePch.h"

#include "Enum.pb.h"
#include "Struct.pb.h"

#include "Settings.h"
#include "Time.h"
#include "Random.h"

using GameSessionRef = std::shared_ptr<class GameSession>;
using PlayerRef = std::shared_ptr<class Player>;