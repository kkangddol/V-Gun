#pragma once
#include "Define3D.h"
#include "GeometryGenerator.h"
#include <type_traits>
#include <stdlib.h>
#include <memory>
#include "ScreenBuffer.h"
#include "ScreenBufferPhong.h"
#include "ScreenBufferPBR.h"
#include "ScreenBufferSkinnedPBR.h"
#include "ScreenBufferPrecombined.h"

namespace RocketCore::Graphics
{
	class Shader;

	class LowLevelDX11
	{
		friend class RenderPipeline;
		friend class IRenderer;
		friend class ForwardRenderer;
	public:
		static LowLevelDX11* GetInstance()
		{
			if (instance == nullptr)
			{
				instance = new LowLevelDX11();
				//atexit(DestroyD3D);
			}
			return instance;
		}

		void DirectX_Init(HWND _hWnd, int screenWidth, int screenHeight, bool isEditor);
		void DirectX_OnResize();
		void DirectX_CreateRasterizerStates();
		HRESULT DirectX_GetAdapterInfo();
		void DirectX_Finalize();
		void DirectX_CreateFullScreenQuads(); //FullScreenQuad의 VB/IB 자체만을 만든다.
		void DirectX_InitGraphicsBuffer(); //Graphics 버퍼를 원래 의도된 대로 세팅한다.
		void DirectX_CreateBlendStates();

		//Deferred Rendering : G-Buffer Pass를 바인딩한다!


		//일차적으로 DX를 다루는 것은 모두 여기서 관리한다고 생각하자.
		//구조가 연결되면, 나누기로!
		//DX 헤더 자체는 다른 클래스들도 가능하나,
		//직접 무언가를 만들어주고 등등.. => 이와 같이 작동할 수 있을 것이다.

		ScreenBufferPhong* _deferredPhongShaderQuad = nullptr; //Phong Full Screen Quad. (개별적)
		ScreenBuffer* _deferredPhongFullScreenQuad = nullptr;	//디퍼드 렌더링 (Phong)이 옮겨올 쿼드.

		ScreenBufferPBR* _deferredPBRShaderQuad = nullptr;		//PBR Full Screen Quad (개별적)
		ScreenBuffer* _deferredPBRFullScreenQuad = nullptr;		//디퍼드 렌더링 (PBR)이 옮겨올 쿼드.

		ScreenBufferSkinnedPBR* _deferredPBRSkinnedShaderQuad = nullptr;		//(Skinned) PBR Full Screen Quad (개별적)
		ScreenBuffer* _deferredPBRSkinnedFullScreenQuad = nullptr;				//(Skinned) 디퍼드 렌더링 (PBR)이 옮겨올 쿼드.

		ScreenBuffer* _forwardFullScreenQuad = nullptr;			//포워드 렌더링이 출력할 쿼드.
		ScreenBufferPrecombined* _combinedFullScreenQuad = nullptr;		 
		//최종적으로 스크린에 보일 것. (Deferred + Forward, w/ Masking).. 이었으나, 이제는 아니다! 그냥 최종 결과물은 MainRenderTarget에 줌.
		//그래도, 삭제하면 안된다. 로직 상.
		
		Microsoft::WRL::ComPtr<ID3D11Device1> m_d3dDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext1> m_d3dMainContext;
		///[TW] 예전에는 Forward Rendering만을 위해 만들었다! 그러니, CreateDeferredContext로 만들어진게 아님.. 더 자세히 보기!
		Microsoft::WRL::ComPtr<ID3D11DeviceContext1> m_d3dDeferredContext;

		Microsoft::WRL::ComPtr<IDXGISwapChain1> m_swapChain = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pBackBuffer = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pDepthStencil = nullptr;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_MainRenderTargetView = nullptr;	//최종적인 RenderTarget. 모든 결과는 결국 여기를 거쳐서 출력된다!
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView = nullptr;	//최종적인 영향을 미치는 DepthStencil.
		ID3D11ShaderResourceView* _dsv_sync_srv = nullptr;

		//Forward/Deferred RTV는 이제 ScreenBuffer에 저장된다.

		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_SolidRS;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_WireFrameRS;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_NormalDSS;

		DXGI_ADAPTER_DESC1 m_AdapterDesc = DXGI_ADAPTER_DESC1();
		DXGI_PRESENT_PARAMETERS m_dxgiParam = DXGI_PRESENT_PARAMETERS();

		DXGI_FORMAT m_DXGIFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

		DXGI_SWAP_CHAIN_DESC1 m_swapChainDesc = DXGI_SWAP_CHAIN_DESC1();
		D3D11_VIEWPORT m_FullClientViewport = D3D11_VIEWPORT();
		D3D11_VIEWPORT m_QuadViewport = D3D11_VIEWPORT();

		

		UINT      m4xMsaaQuality = 0;
		bool	  mEnable4xMsaa = false;

		D3D11_RECT d3dScissorsRect;

		//윈도우즈 창 관련.
		HWND m_hWnd = nullptr;
		UINT m_WindowWidth = 0;
		UINT m_WindowHeight = 0;
		bool _isEditor = false;

		//목표하는 Resolution.
		UINT m_TargetResolutionWidth = 1920;
		UINT m_TargetResolutionHeight = 1080;

		//Deferred-Forward 합치기 위한 BlendState 설정.
		Microsoft::WRL::ComPtr<ID3D11BlendState> m_NoBlend = nullptr;

	private:
		static LowLevelDX11* instance;
		//싱글턴
		LowLevelDX11() : m4xMsaaQuality(NULL), m_AdapterDesc(DXGI_ADAPTER_DESC1()), m_WindowHeight(0), m_WindowWidth(0),
			m_dxgiParam(DXGI_PRESENT_PARAMETERS()), m_hWnd(nullptr), m_swapChainDesc(DXGI_SWAP_CHAIN_DESC1()), m_FullClientViewport(D3D11_VIEWPORT())
		{}
		LowLevelDX11(const LowLevelDX11& ref) {}
		LowLevelDX11& operator=(const LowLevelDX11& ref) {}
		~LowLevelDX11() 
		{}

		//Deferred를 위한 ScreenQuad를 만들어 반환해준다. (타입은 실제로 만드는 게임)
		template <typename T>
		ScreenBuffer* MakeFullScreenQuad(Shader* shader);

		//추가적으로 렌더되는 Quad들의 연동을 위한 함수.
		void SetupDefaultQuadResources(ScreenBuffer* screenBuffer);

		//Deferred 기법이 추가될수록 늘어난다.
		void SetupPhongDeferredResources();
		//PBR Deferred 리소스 셋업!
		void SetupPBRDeferredResources();
		//PBR Deferred Skinned 리소스 셋업!
		void SetupPBRSkinnedDeferredResources();
	};

	//무조건 여기서 Template 매개변수는 ScreenBuffer, ScreenBuffer 중 하나여야 한다.
	//일단 type_traits를 써서 일차 체크하기는 할 것임.
	template <typename T>
	ScreenBuffer* RocketCore::Graphics::LowLevelDX11::MakeFullScreenQuad(Shader* shader)
	{
		static_assert(std::is_base_of<ScreenBuffer, T>::value, "T must be inherited from ScreenBuffer!");

		ScreenBuffer* tSB = new T(shader);

		GeometryGenerator gen;
		GeometryGenerator::MeshData_PosNormalTex meshData;
		gen.GenerateFullscreenQuad(meshData);

		//DirectXHelper::GenerateVertexBuffer(m_direct3D.GetDevice(), meshData, m_screenQuad);
		D3D11_BUFFER_DESC vertexBufferDesc{ 0, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0 };
		D3D11_BUFFER_DESC indexBufferDesc{ 0, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, 0, 0, 0 };
		D3D11_SUBRESOURCE_DATA vertexSubresourceData{};
		D3D11_SUBRESOURCE_DATA indexSubresourceData{};

		if (meshData.Indices.empty() || meshData.Vertices.empty())
			assert(false);

		tSB->_indexCount = meshData.Indices.size();

		vertexBufferDesc.ByteWidth = sizeof(GeometryGenerator::GeomVertex_PosNormalTex) * meshData.Vertices.size();
		vertexSubresourceData.pSysMem = meshData.Vertices.data();
		HR(m_d3dDevice.Get()->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &(tSB->_vertexBuffer)));

		indexBufferDesc.ByteWidth = sizeof(UINT) * meshData.Indices.size();
		indexSubresourceData.pSysMem = meshData.Indices.data();
		HR(m_d3dDevice.Get()->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &(tSB->_indexBuffer)));

		return tSB;
	}

}