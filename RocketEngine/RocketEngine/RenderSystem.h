#pragma once
#include "Singleton.h"
#include <windows.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include "..\\RocketGraphicsInterface\\IRocketGraphics.h"
#include "..\\RocketGraphicsInterface\\AnimData.h"
#include "..\\RocketGraphicsInterface\\RenderType.h"
#include "RocketAPI.h"

#ifdef _DX11
#ifdef _DEBUG
#define GRAPHICSDLL_PATH (L"..\\x64\\Debug\\RocketRendering.dll") // (".\\my\\Path\\"#filename) ".\\my\\Path\\filename"
#else
#define GRAPHICSDLL_PATH ("Graphics\\RocketRendering.dll"#filename)
#endif // _DEBUG
#elif _DX12
#ifdef _DEBUG
#define GRAPHICSDLL_PATH (L"..\\x64\\Debug\\RocketDX12.dll") // (".\\my\\Path\\"#filename) ".\\my\\Path\\filename"
#else
#define GRAPHICSDLL_PATH ("Graphics\\RocketDX12.dll"#filename)
#endif // _DEBUG
#endif // _DX11 or _DX12

namespace RocketCore::Graphics
{
	class IRenderable;
	class ISketchable;
	class RenderConstantData;
}

namespace RocketCore
{
	class MeshRendererBase;
	class UIRenderer;
}

namespace RocketEngine
{
	class Light;
}

/// <summary>
/// 엔진에서 Graphics 모듈을 불러와 초기화시키고
/// 렌더링 컴포넌트를 총괄하여 그릴 수 있도록 관리하는 클래스.
/// 
/// 23.06.20 강석원 인재원.
/// </summary>
namespace RocketCore
{
	class RenderSystem : public Singleton<RenderSystem>
	{
		friend Singleton;
		friend void RocketEngine::RocketDestroyWindow();

	private:
		RenderSystem();		// 싱글턴이기 때문에 외부에서 생성할 수 없도록.

		/// 시스템 초기화 관련
	public:
		void Initialize(HWND hWnd, int screenWidth, int screenHeight, bool isEditor = false);
		void Finalize();

	private:
		bool _isEditor = false;

		/// 게임 초기화 관련
	public:
		void MakeRenderableAll();
		void MakeAnimationAll();
		void MakeUIAll();

	public:
		void OnResize(int width, int height);

		/// 렌더링 관련
	public:
		void DrawProcess();

	private:
		void UpdateConstantData(RocketCore::Graphics::RenderConstantData& data);

	private:
		void BeginDraw();
		void DrawCurrentScene(Graphics::eRenderType renderType);
		void EndDraw();

	private:
		void DrawCurrentUI();

	private:
		void DrawDebugBox();
		void DrawDebugText();
		void DrawDebugLine();
		void DrawDebug2DBox();

		/// 임시 물리 디버그 관련
	private:
		void RenderPhysicsDebug();

		/// 애니메이션 관련
	public:
		void PlayAnimation(MeshRendererBase* renderer, RocketCore::Graphics::AnimData animData);

	public:
		int GetScreenWidth() const;
		int GetScreenHeight() const;

		/// 기본 정보(윈도우 핸들, 윈도우 사이즈 등)
	private:
		HWND _hWnd;
		int _screenWidth;
		int _screenHeight;

		/// DLL 관련
	private:
		HMODULE hGraphicsModule;
		std::unique_ptr<RocketCore::Graphics::IRocketGraphics> _rocketGraphics;

	public:
		void AddLight(RocketEngine::Light* light);

	private:
		std::unordered_map<MeshRendererBase*, Graphics::IRenderable*> _renderMap;
		std::unordered_map<UIRenderer*, Graphics::ISketchable*> _uiMap;
		std::vector<RocketEngine::Light*> _lights;
	};
}
