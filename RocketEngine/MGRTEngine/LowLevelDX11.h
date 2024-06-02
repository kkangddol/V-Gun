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
		void DirectX_CreateFullScreenQuads(); //FullScreenQuad�� VB/IB ��ü���� �����.
		void DirectX_InitGraphicsBuffer(); //Graphics ���۸� ���� �ǵ��� ��� �����Ѵ�.
		void DirectX_CreateBlendStates();

		//Deferred Rendering : G-Buffer Pass�� ���ε��Ѵ�!


		//���������� DX�� �ٷ�� ���� ��� ���⼭ �����Ѵٰ� ��������.
		//������ ����Ǹ�, �������!
		//DX ��� ��ü�� �ٸ� Ŭ�����鵵 �����ϳ�,
		//���� ���𰡸� ������ְ� ���.. => �̿� ���� �۵��� �� ���� ���̴�.

		ScreenBufferPhong* _deferredPhongShaderQuad = nullptr; //Phong Full Screen Quad. (������)
		ScreenBuffer* _deferredPhongFullScreenQuad = nullptr;	//���۵� ������ (Phong)�� �Űܿ� ����.

		ScreenBufferPBR* _deferredPBRShaderQuad = nullptr;		//PBR Full Screen Quad (������)
		ScreenBuffer* _deferredPBRFullScreenQuad = nullptr;		//���۵� ������ (PBR)�� �Űܿ� ����.

		ScreenBufferSkinnedPBR* _deferredPBRSkinnedShaderQuad = nullptr;		//(Skinned) PBR Full Screen Quad (������)
		ScreenBuffer* _deferredPBRSkinnedFullScreenQuad = nullptr;				//(Skinned) ���۵� ������ (PBR)�� �Űܿ� ����.

		ScreenBuffer* _forwardFullScreenQuad = nullptr;			//������ �������� ����� ����.
		ScreenBufferPrecombined* _combinedFullScreenQuad = nullptr;		 
		//���������� ��ũ���� ���� ��. (Deferred + Forward, w/ Masking).. �̾�����, ������ �ƴϴ�! �׳� ���� ������� MainRenderTarget�� ��.
		//�׷���, �����ϸ� �ȵȴ�. ���� ��.
		
		Microsoft::WRL::ComPtr<ID3D11Device1> m_d3dDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext1> m_d3dMainContext;
		///[TW] �������� Forward Rendering���� ���� �������! �׷���, CreateDeferredContext�� ��������� �ƴ�.. �� �ڼ��� ����!
		Microsoft::WRL::ComPtr<ID3D11DeviceContext1> m_d3dDeferredContext;

		Microsoft::WRL::ComPtr<IDXGISwapChain1> m_swapChain = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pBackBuffer = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pDepthStencil = nullptr;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_MainRenderTargetView = nullptr;	//�������� RenderTarget. ��� ����� �ᱹ ���⸦ ���ļ� ��µȴ�!
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView = nullptr;	//�������� ������ ��ġ�� DepthStencil.
		ID3D11ShaderResourceView* _dsv_sync_srv = nullptr;

		//Forward/Deferred RTV�� ���� ScreenBuffer�� ����ȴ�.

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

		//�������� â ����.
		HWND m_hWnd = nullptr;
		UINT m_WindowWidth = 0;
		UINT m_WindowHeight = 0;
		bool _isEditor = false;

		//��ǥ�ϴ� Resolution.
		UINT m_TargetResolutionWidth = 1920;
		UINT m_TargetResolutionHeight = 1080;

		//Deferred-Forward ��ġ�� ���� BlendState ����.
		Microsoft::WRL::ComPtr<ID3D11BlendState> m_NoBlend = nullptr;

	private:
		static LowLevelDX11* instance;
		//�̱���
		LowLevelDX11() : m4xMsaaQuality(NULL), m_AdapterDesc(DXGI_ADAPTER_DESC1()), m_WindowHeight(0), m_WindowWidth(0),
			m_dxgiParam(DXGI_PRESENT_PARAMETERS()), m_hWnd(nullptr), m_swapChainDesc(DXGI_SWAP_CHAIN_DESC1()), m_FullClientViewport(D3D11_VIEWPORT())
		{}
		LowLevelDX11(const LowLevelDX11& ref) {}
		LowLevelDX11& operator=(const LowLevelDX11& ref) {}
		~LowLevelDX11() 
		{}

		//Deferred�� ���� ScreenQuad�� ����� ��ȯ���ش�. (Ÿ���� ������ ����� ����)
		template <typename T>
		ScreenBuffer* MakeFullScreenQuad(Shader* shader);

		//�߰������� �����Ǵ� Quad���� ������ ���� �Լ�.
		void SetupDefaultQuadResources(ScreenBuffer* screenBuffer);

		//Deferred ����� �߰��ɼ��� �þ��.
		void SetupPhongDeferredResources();
		//PBR Deferred ���ҽ� �¾�!
		void SetupPBRDeferredResources();
		//PBR Deferred Skinned ���ҽ� �¾�!
		void SetupPBRSkinnedDeferredResources();
	};

	//������ ���⼭ Template �Ű������� ScreenBuffer, ScreenBuffer �� �ϳ����� �Ѵ�.
	//�ϴ� type_traits�� �Ἥ ���� üũ�ϱ�� �� ����.
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