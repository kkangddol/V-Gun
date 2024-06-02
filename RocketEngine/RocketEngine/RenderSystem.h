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
/// �������� Graphics ����� �ҷ��� �ʱ�ȭ��Ű��
/// ������ ������Ʈ�� �Ѱ��Ͽ� �׸� �� �ֵ��� �����ϴ� Ŭ����.
/// 
/// 23.06.20 ������ �����.
/// </summary>
namespace RocketCore
{
	class RenderSystem : public Singleton<RenderSystem>
	{
		friend Singleton;
		friend void RocketEngine::RocketDestroyWindow();

	private:
		RenderSystem();		// �̱����̱� ������ �ܺο��� ������ �� ������.

		/// �ý��� �ʱ�ȭ ����
	public:
		void Initialize(HWND hWnd, int screenWidth, int screenHeight, bool isEditor = false);
		void Finalize();

	private:
		bool _isEditor = false;

		/// ���� �ʱ�ȭ ����
	public:
		void MakeRenderableAll();
		void MakeAnimationAll();
		void MakeUIAll();

	public:
		void OnResize(int width, int height);

		/// ������ ����
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

		/// �ӽ� ���� ����� ����
	private:
		void RenderPhysicsDebug();

		/// �ִϸ��̼� ����
	public:
		void PlayAnimation(MeshRendererBase* renderer, RocketCore::Graphics::AnimData animData);

	public:
		int GetScreenWidth() const;
		int GetScreenHeight() const;

		/// �⺻ ����(������ �ڵ�, ������ ������ ��)
	private:
		HWND _hWnd;
		int _screenWidth;
		int _screenHeight;

		/// DLL ����
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
