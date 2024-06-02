#pragma once
#include "Singleton.h"
#include "..\\RocketMath\\RocketMath.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif // _DEBUG

#include <vector>
#include <string>

namespace RocketCore
{
	struct DebugText
	{
		RocketEngine::RMFLOAT2 centerPos;
		std::string content;
		float size;
	};

	struct DebugBox
	{
		RocketEngine::RMFLOAT4X4 worldTM;
		RocketEngine::RMFLOAT3 widthHeightDepth;
		bool isWire;
		RocketEngine::RMFLOAT4 color;
	};

	struct DebugLine
	{
		RocketEngine::RMFLOAT3 beginPoint;
		RocketEngine::RMFLOAT3 endPoint;
		RocketEngine::RMFLOAT4 color;
	};

	struct Debug2DBox
	{
		RocketEngine::RMFLOAT2 LT;
		RocketEngine::RMFLOAT2 RB;
		RocketEngine::RMFLOAT4 color;
	};

}

namespace RocketCore
{
	class DebugSystem : public Singleton<DebugSystem>
	{
		friend Singleton;
	private:
		DebugSystem() = default;

	public:
		void Initialize();
		void Finalize();

	public:
		void CheckDebugMode();

	public:
		void DrawDebugText(RocketEngine::RMFLOAT2 centerPos, std::string content, float size);
		void DrawDebugBox(RocketEngine::RMFLOAT4X4 worldTM, RocketEngine::RMFLOAT3 widthHeightDepth = {1.0f,1.0f,1.0f}, bool isWire = true, RocketEngine::RMFLOAT4 color = { 1.0f,0.0f,0.0f,1.0f });
		void DrawDebugLine(RocketEngine::RMFLOAT3 beginPoint, RocketEngine::RMFLOAT3 endPoint, RocketEngine::RMFLOAT4 color = { 1.0f,0.0f,0.0f,1.0f });
		void DrawDebug2DBox(RocketEngine::RMFLOAT2 LT, RocketEngine::RMFLOAT2 RB, RocketEngine::RMFLOAT4 color = { 1.0f,0.0f,0.0f,1.0f });

	public:
		std::vector<DebugText>& GetTextQueue();
		std::vector<DebugBox>& GetBoxQueue();
		std::vector<DebugLine>& GetLineQueue();
		std::vector<Debug2DBox>& Get2DBoxQueue();

	public:
		void FlushBoxQueue();
		void FlushTextQueue();
		void FlushLineQueue();
		void Flush2DBoxQueue();

	private:
		std::vector<DebugText> _textQueue;
		std::vector<DebugBox> _boxQueue;
		std::vector<DebugLine> _lineQueue;
		std::vector<Debug2DBox> _2DBoxQueue;

	public:
		void SetDebugMode(bool isDebugMode);
		bool IsDebugMode() const;

	private:
		bool _isDebugMode;
	};
}
