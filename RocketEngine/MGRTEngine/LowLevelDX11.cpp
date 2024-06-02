#include "LowLevelDX11.h"
#include "RenderPipeline.h"
#include "Define3D.h"
#include "GeometryGenerator.h"
#include "Shader.h"
#include "ResourceManager.h"

namespace RocketCore::Graphics
{
	LowLevelDX11* LowLevelDX11::instance = nullptr;


	//일단 구체적인 구현보다 윤곽부터!
	void LowLevelDX11::DirectX_Init(HWND _hWnd, int screenWidth, int screenHeight, bool isEditor)
	{
		//D3D 초기화.
		HRESULT hr = S_OK;
		this->m_hWnd = _hWnd;
		//일단 D3DDevice를 만드는 과정.

		m_WindowWidth = screenWidth;
		m_WindowHeight = screenHeight;
		_isEditor = isEditor;

		//D2D 호환 가능하게.
		UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(DEBUG) || defined(_DEBUG)  
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_FEATURE_LEVEL levels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1,
		};

		Microsoft::WRL::ComPtr<ID3D11Device>        device; //D3D Device의 역할.
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext; //Immediate Context

		D3D_FEATURE_LEVEL m_featureLevel;

		//D3D11CreateDevice.
		hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			0,
			createDeviceFlags,
			levels,
			ARRAYSIZE(levels),
			D3D11_SDK_VERSION,
			&device,
			&m_featureLevel,
			&immediateContext
		);

		//QueryInterface로 명시적으로 바꿔주자!
		hr = device->QueryInterface(__uuidof(ID3D11Device1), (void**)&m_d3dDevice);
		hr = immediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), (void**)&m_d3dMainContext);

		///Deferred Context 만들기!
		///만들어졌기는 했는데, 추가 셋업은 뒤에 하는 것이 맞다. -> 이거 안쓰고도 Deferred 자체는 가능하다! 
		hr = m_d3dDevice.Get()->CreateDeferredContext1(0, &m_d3dDeferredContext);

		device->Release();
		immediateContext->Release();

		//멀티샘플링 몇단계까지? 체크
		hr = m_d3dDevice->CheckMultisampleQualityLevels(
			m_DXGIFormat, 4, &m4xMsaaQuality);
		assert(m4xMsaaQuality > 0);

		///디바이스 호환 여기.
		Microsoft::WRL::ComPtr<IDXGIDevice3> dxgiDevice;
		hr = m_d3dDevice->QueryInterface(__uuidof(IDXGIDevice3), (void**)(&dxgiDevice));
		if (FAILED(hr))
		{

		}

		Microsoft::WRL::ComPtr<IDXGIAdapter> adapterBasic;
		hr = dxgiDevice->GetAdapter(&adapterBasic);
		if (FAILED(hr))
		{

		}

		Microsoft::WRL::ComPtr<IDXGIAdapter2> adapterUpgraded;
		hr = adapterBasic->QueryInterface(__uuidof(IDXGIAdapter2), (void**)(&adapterUpgraded));
		if (FAILED(hr))
		{

		}

		Microsoft::WRL::ComPtr<IDXGIFactory2> dxgiFactory;
		hr = adapterUpgraded->GetParent(IID_PPV_ARGS(&dxgiFactory));
		//hr = CreateDXGIFactory2(0, __uuidof(IDXGIFactory2), (void**)(&dxgiFactory));

		//////Swap Chain Desc를 만듬. 
		ZeroMemory(&m_swapChainDesc, sizeof(m_swapChainDesc));
		m_swapChainDesc.Width = this->m_WindowWidth;
		m_swapChainDesc.Height = this->m_WindowHeight;
		m_swapChainDesc.BufferCount = 2;
		m_swapChainDesc.Format = m_DXGIFormat;
		m_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		m_swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		m_swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		m_swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		m_swapChainDesc.Flags = 0;

		if (mEnable4xMsaa)
		{
			m_swapChainDesc.SampleDesc.Count = 4;
			m_swapChainDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
		}
		// No MSAA
		else
		{
			m_swapChainDesc.SampleDesc.Count = 1;
			m_swapChainDesc.SampleDesc.Quality = 0;
		}

		if (SUCCEEDED(hr))
		{
			dxgiFactory->CreateSwapChainForHwnd(dxgiDevice.Get(), m_hWnd, &m_swapChainDesc, 0, 0, m_swapChain.GetAddressOf());
		}

		ReleaseCOM(dxgiDevice);
		ReleaseCOM(adapterUpgraded);
		ReleaseCOM(dxgiFactory);

		DirectX_OnResize();
		DirectX_CreateRasterizerStates();


		//Present1에 활용.
		m_dxgiParam.DirtyRectsCount = 0;
		m_dxgiParam.pDirtyRects = NULL;
		m_dxgiParam.pScrollRect = NULL;
		m_dxgiParam.pScrollOffset = 0;

		hr = DirectX_GetAdapterInfo();

		DirectX_CreateBlendStates();

	}

	void LowLevelDX11::DirectX_OnResize()
	{
		///230807 : 오류 있음, 후에 여유 생길시 재작업 예정!
		HRESULT hr;
		assert(m_d3dMainContext);
		assert(m_d3dDeferredContext);
		assert(m_d3dDevice);
		assert(m_swapChain);
		//
		ReleaseCOM(m_MainRenderTargetView);
		ReleaseCOM(m_pDepthStencilView);
		ReleaseCOM(m_pDepthStencil);

		m_d3dMainContext->ClearState();
		m_d3dDeferredContext->ClearState();

		////일단 여기까지는 세이프 (내 전체 BufferCount는 2개!)
		//hr = m_swapChain->ResizeBuffers(2, m_WindowWidth, m_WindowHeight,
		//	DXGI_FORMAT_R8G8B8A8_UNORM, 0);

		hr = m_swapChain->ResizeBuffers(2, m_WindowWidth, m_WindowHeight,
			DXGI_FORMAT_R32G32B32A32_FLOAT, 0);

		//DXGI_FORMAT_R32G32B32A32_FLOAT
		hr = m_swapChain->GetBuffer(
			0,
			__uuidof(ID3D11Texture2D),
			(void**)&m_pBackBuffer);

		D3D11_TEXTURE2D_DESC tDebugTextureDesc;
		m_pBackBuffer->GetDesc(&tDebugTextureDesc);

		//MainRenderTargetView를 만든다!
		hr = m_d3dDevice->CreateRenderTargetView(
			m_pBackBuffer.Get(),
			nullptr,
			m_MainRenderTargetView.GetAddressOf());

		D3D11_TEXTURE2D_DESC m_bbDesc;
		m_pBackBuffer->GetDesc(&m_bbDesc);

		m_WindowWidth = m_bbDesc.Width;
		m_WindowHeight = m_bbDesc.Height;

		///[TW_DF] 기존의 OnResize 코드에서, Deferred에 호환되게 설정을 바꾸어줌.
		///나중에 BlendState도 추가해야 한다!

		//DepthStencil 관련 세팅!
		//DepthStencil Texture (Screen Quad용) => DESC 만들기.
		ID3D11Texture2D* depthStencilTexture = nullptr;
		D3D11_TEXTURE2D_DESC depthStencilBufferDesc{};
		depthStencilBufferDesc.Width = this->m_WindowWidth;
		depthStencilBufferDesc.Height = this->m_WindowHeight;
		depthStencilBufferDesc.MipLevels = 1;
		depthStencilBufferDesc.ArraySize = 1;
		//depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		depthStencilBufferDesc.SampleDesc.Count = 1;
		depthStencilBufferDesc.SampleDesc.Quality = 0;
		depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		depthStencilBufferDesc.CPUAccessFlags = 0;
		depthStencilBufferDesc.MiscFlags = 0;

		//DepthStencilView (Screen Quad용) => DESC 만들기.
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
		depthStencilViewDesc.Flags = 0;
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		D3D11_SHADER_RESOURCE_VIEW_DESC sr_desc;
		sr_desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		sr_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		sr_desc.Texture2D.MostDetailedMip = 0;
		sr_desc.Texture2D.MipLevels = 1;

		//이건 전체 스크린에 해당되는 DepthStencilView를 세팅하는 것! (Final) -> 다만, 사실 아니다.. Forward 연동 연동.
		HR(m_d3dDevice.Get()->CreateTexture2D(&depthStencilBufferDesc, NULL, &depthStencilTexture));
		HR(m_d3dDevice.Get()->CreateDepthStencilView(depthStencilTexture, &depthStencilViewDesc, &m_pDepthStencilView));
		HR(m_d3dDevice.Get()->CreateShaderResourceView(depthStencilTexture, &sr_desc, &_dsv_sync_srv));

		//ReleaseCOM(depthStencilTexture);

		//m_d3dForwardContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView.Get());

		///
		float paddingX = (m_WindowWidth - 1920.0f) / 2;
		float paddingY = 0;
		float w = 1920.0f;
		float h = 1080.0f;
		
		if (_isEditor)
		{
			paddingX = 0;
			paddingY = 0;
			w = static_cast<float>(m_WindowWidth);
			h = static_cast<float>(m_WindowHeight);
		}

		//float paddingX = 0;
		//float paddingY = 0;
		//float w = static_cast<float>(m_WindowWidth);
		//float h = static_cast<float>(m_WindowHeight);

		ZeroMemory(&m_FullClientViewport, sizeof(D3D11_VIEWPORT));
		m_FullClientViewport.TopLeftX = paddingX;
		m_FullClientViewport.TopLeftY = paddingY;
		m_FullClientViewport.Width = w;
		m_FullClientViewport.Height = h;
		m_FullClientViewport.MinDepth = 0.0f;
		m_FullClientViewport.MaxDepth = 1.0f;

		m_d3dMainContext->RSSetViewports(
			1,
			&m_FullClientViewport
		);

		ZeroMemory(&m_QuadViewport, sizeof(D3D11_VIEWPORT));
		m_QuadViewport.TopLeftX = 0;
		m_QuadViewport.TopLeftY = 0;
		m_QuadViewport.Width = static_cast<float>(m_WindowWidth);
		m_QuadViewport.Height = static_cast<float>(m_WindowHeight);
		m_QuadViewport.MinDepth = 0.0f;
		m_QuadViewport.MaxDepth = 1.0f;

		m_d3dMainContext->RSSetViewports(
			1,
			&m_QuadViewport
		);
		
		//하는 김에 ScissorRect 역시 설정.
		//d3dScissorsRect.left = m_FullClientViewport.TopLeftX;
		//d3dScissorsRect.top = m_FullClientViewport.TopLeftY;
		//d3dScissorsRect.right = m_FullClientViewport.TopLeftX + m_FullClientViewport.Width;
		//d3dScissorsRect.bottom = m_FullClientViewport.TopLeftY + m_FullClientViewport.Height;

		d3dScissorsRect.left = -1;
		d3dScissorsRect.top = -1;
		d3dScissorsRect.right = 1;
		d3dScissorsRect.bottom = 1;

		m_d3dMainContext->RSSetScissorRects(
			1,
			&d3dScissorsRect
		);


	}

	void LowLevelDX11::DirectX_CreateRasterizerStates()
	{
		// Render State 중 Rasterizer State
		D3D11_RASTERIZER_DESC solidDesc;
		ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
		solidDesc.FillMode = D3D11_FILL_SOLID;
		solidDesc.CullMode = D3D11_CULL_BACK;
		solidDesc.FrontCounterClockwise = false;
		solidDesc.DepthClipEnable = true;

		HR(m_d3dDevice->CreateRasterizerState(&solidDesc, &m_SolidRS));

		D3D11_RASTERIZER_DESC wireframeDesc;
		ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
		wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
		wireframeDesc.CullMode = D3D11_CULL_BACK;
		wireframeDesc.FrontCounterClockwise = false;
		wireframeDesc.DepthClipEnable = true;

		HR(m_d3dDevice->CreateRasterizerState(&wireframeDesc, &m_WireFrameRS));


		// 폰트용 DSS
		D3D11_DEPTH_STENCIL_DESC equalsDesc;
		ZeroMemory(&equalsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		equalsDesc.DepthEnable = true;
		equalsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;		// 깊이버퍼에 쓰기는 한다
		equalsDesc.DepthFunc = D3D11_COMPARISON_LESS;

		HR(m_d3dDevice->CreateDepthStencilState(&equalsDesc, &m_NormalDSS));
	}

	HRESULT LowLevelDX11::DirectX_GetAdapterInfo()
	{
		// DXGI버전별로 다름
		IDXGIAdapter1* pAdapter;
		IDXGIFactory2* pFactory = NULL;

		HRESULT hr = S_OK;

		// DXGIFactory 개체 생성(DXGI.lib 필요)
		hr = CreateDXGIFactory2(0, __uuidof(IDXGIFactory2), (void**)&pFactory);
		if (hr != S_OK)
		{
			return E_FAIL;
		}

		hr = pFactory->EnumAdapters1(0, &pAdapter);

		if (hr != DXGI_ERROR_NOT_FOUND)
		{
			pAdapter->GetDesc1(&m_AdapterDesc);
		}

		SAFE_RELEASE(pAdapter);
		SAFE_RELEASE(pFactory);

		return S_OK;
	}

	void LowLevelDX11::DirectX_Finalize()
	{
		m_d3dDevice.Get()->Release();
		m_d3dMainContext.Get()->Release();
		//ReleaseCOM(m_d3dImmediateContext);
		m_d3dDeferredContext.Get()->Release();
		m_swapChain.Get()->Release();
		m_pBackBuffer.Get()->Release();
		m_pDepthStencil.Get()->Release();
		m_MainRenderTargetView.Get()->Release();
		m_pDepthStencilView.Get()->Release();
		ReleaseCOM(m_NoBlend);
		if (_deferredPhongShaderQuad != nullptr)
		{
			delete _deferredPhongShaderQuad;
		}
		if (_deferredPBRShaderQuad != nullptr)
		{
			delete _deferredPBRShaderQuad;
		}


	}

	void LowLevelDX11::DirectX_CreateFullScreenQuads()
	{
		///디퍼드에 적용될 Full Screen Quad들을 만든다.
		//DeferredPhongShader를 찾는다.
		Shader* tDeferPhongShader = ResourceManager::GetInstance().FindShaderInputSetByName("DeferredPhongShader.cso").first->GetShader();
		_deferredPhongShaderQuad = static_cast<ScreenBufferPhong*>(MakeFullScreenQuad<ScreenBufferPhong>(tDeferPhongShader));
		tDeferPhongShader->SetMainTechnique("LightingTech");
		_deferredPhongShaderQuad->Initialize(tDeferPhongShader->GetMainTechnique());
		//BasicPBR을 찾는다.
		Shader* tDeferPBRShader = ResourceManager::GetInstance().FindShaderInputSetByName("BasicPBR.cso").first->GetShader();
		_deferredPBRShaderQuad = static_cast<ScreenBufferPBR*>(MakeFullScreenQuad<ScreenBufferPBR>(tDeferPBRShader));
		tDeferPBRShader->SetMainTechnique("LightingTech");
		_deferredPBRShaderQuad->Initialize(tDeferPBRShader->GetMainTechnique());
		//BasicPBRSkinned를 찾는다.
		Shader* tDeferPBRSkinnedShader = ResourceManager::GetInstance().FindShaderInputSetByName("BasicPBRSkinned.cso").first->GetShader();
		_deferredPBRSkinnedShaderQuad = static_cast<ScreenBufferSkinnedPBR*>(MakeFullScreenQuad<ScreenBufferSkinnedPBR>(tDeferPBRSkinnedShader));
		tDeferPBRSkinnedShader->SetMainTechnique("LightingTech");
		_deferredPBRSkinnedShaderQuad->Initialize(tDeferPBRSkinnedShader->GetMainTechnique());

		///TOFIX : 얘는 절대 이렇게 되어서는 안된다! 일단은 Shader 있는 셰이더 가지고 막이는거 봐야 함.
		//또한, 합치는 작업은 아마 여기서 이루어지지 않을 것이다. 특별하게 Deferred를 거치는거 아니면, 디폴트는 그 자체로 셰이더 요구는 X.
		_deferredPhongFullScreenQuad = MakeFullScreenQuad<ScreenBuffer>(nullptr);
		_deferredPBRFullScreenQuad = MakeFullScreenQuad<ScreenBuffer>(nullptr);
		_deferredPBRSkinnedFullScreenQuad = MakeFullScreenQuad<ScreenBuffer>(nullptr);
		_forwardFullScreenQuad = MakeFullScreenQuad<ScreenBuffer>(nullptr);

		Shader* itPC = ResourceManager::GetInstance().FindShaderInputSetByName("FinalCombination.cso").first->GetShader();
		_combinedFullScreenQuad = static_cast<ScreenBufferPrecombined*>(MakeFullScreenQuad<ScreenBufferPrecombined>(itPC));
	}

	void LowLevelDX11::DirectX_InitGraphicsBuffer()
	{
		///디폴트 Quad들 셋업.
		SetupDefaultQuadResources(_forwardFullScreenQuad);
		SetupDefaultQuadResources(_deferredPhongFullScreenQuad);
		SetupDefaultQuadResources(_deferredPBRFullScreenQuad);
		SetupDefaultQuadResources(_deferredPBRSkinnedFullScreenQuad);
		SetupDefaultQuadResources(_combinedFullScreenQuad);

		//개별적인 Deferred Phong Quad세팅.
		SetupPhongDeferredResources();

		//개별적인 Deferred PBR 세팅.
		SetupPBRDeferredResources();

		//개별적인 Deferred PBR Skinned 세팅.
		SetupPBRSkinnedDeferredResources();

		///따로 Effects Variable 연동.
		//나중에 Double-Pass를 위한 따로 값을 받아놓기 => 이제 Depth Sorting 역시 해야 한다.
		_deferredPhongShaderQuad->AssignRequiredDeferredPhongVariables(
			"gTexDiffuseSRV_GBuffer", "gTexNormalSRV_GBuffer", "gTexPositionSRV_GBuffer", "gTexDepthSRV_GBuffer");
		_deferredPBRShaderQuad->AssignRequiredDeferredPBRVariables(
		"gTexAlbedoSRV_GBuffer", "gTexNormalSRV_GBuffer", "gTexPositionSRV_GBuffer", "gTexArmSRV_GBuffer", "gTexDepthSRV_GBuffer");
		_deferredPBRSkinnedShaderQuad->AssignRequiredDeferredPBRVariables(
			"gTexAlbedoSRV_GBuffer", "gTexNormalSRV_GBuffer", "gTexPositionSRV_GBuffer", "gTexArmSRV_GBuffer", "gTexDepthSRV_GBuffer");
		
		_combinedFullScreenQuad->AssignRequiredPrecombinedBufferVariables(
			"gNearFlip", "gFarFlip",
			"gForward_QuadBuffer", "gForward_DepthBuffer", "gDeferredPhong_QuadBuffer", "gDeferredPhong_DepthBuffer",
			"gDeferredPBR_QuadBuffer", "gDeferredPBR_DepthBuffer", "gDeferredPBR_Skinned_QuadBuffer", "gDeferredPBR_Skinned_DepthBuffer");
	}

	void LowLevelDX11::SetupPhongDeferredResources()
	{
		///MipLevel 조정 : 0으로!
		/*
		miplevel 0 is the full resolution texture, miplevel 1 is half resolution in each dimension, mipl
		evel 2 is quarter resolution in each dimension, etc.
		https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_texture2d_desc
		https://gamedev.stackexchange.com/questions/142402/how-to-resize-a-d3d11texture2d
		*/

		//렌더 타겟 텍스쳐 (Screen Quad용) => DESC 만들기.
		D3D11_TEXTURE2D_DESC textureDesc{};
		textureDesc.Width = this->m_WindowWidth;
		textureDesc.Height = this->m_WindowHeight;
		//textureDesc.Width = this->m_TargetResolutionWidth;
		//textureDesc.Height = this->m_TargetResolutionHeight;
		//textureDesc.MipLevels = 1;
		textureDesc.MipLevels = 0;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

		//RenderTargetView (Screen Quad용) => DESC 만들기.
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		//ShaderResourceView (Screen Quad용) => DESC 만들기.
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{};
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		//shaderResourceViewDesc.Texture2D.MostDetailedMip = -1;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		//shaderResourceViewDesc.Texture2D.MipLevels = 1;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		//모든 DESC들이 세팅되었으니, 이제는 싹다 만들어주자!
		for (unsigned int i = 0; i < ScreenBuffer::DEFERRED_PHONG_BUFFER_COUNT-1; i++)
		{
			HR(m_d3dDevice.Get()->CreateTexture2D(&textureDesc, NULL,
				&(_deferredPhongShaderQuad->_trtPhongGBuffers[i]._texture)));
		}
		for (unsigned int i = 0; i < ScreenBuffer::DEFERRED_PHONG_BUFFER_COUNT - 1; i++)
		{
			HR(m_d3dDevice.Get()->CreateRenderTargetView(_deferredPhongShaderQuad->_trtPhongGBuffers[i]._texture,
				&renderTargetViewDesc, &(_deferredPhongShaderQuad->_trtPhongGBuffers[i]._renderTargetView)));
		}

		//여기서 뻑이 난다. SRV 호환..
		for (unsigned int i = 0; i < ScreenBuffer::DEFERRED_PHONG_BUFFER_COUNT - 1; i++)
		{
			///E_INVALIDARG
			HR(m_d3dDevice.Get()->CreateShaderResourceView(_deferredPhongShaderQuad->_trtPhongGBuffers[i]._texture,
				&shaderResourceViewDesc, &(_deferredPhongShaderQuad->_trtPhongGBuffers[i]._shaderResourceView)));	
		}

		//한번 Depth는 다른 포맷으로 설정해보자.
		D3D11_TEXTURE2D_DESC depth_textureDesc{};
		 depth_textureDesc.Width = this->m_WindowWidth;
		 depth_textureDesc.Height = this->m_WindowHeight;
		//textureDesc.Width = this->m_TargetResolutionWidth;
		//textureDesc.Height = this->m_TargetResolutionHeight;
		//textureDesc.MipLevels = 1;
		 depth_textureDesc.MipLevels = 0;
		 depth_textureDesc.ArraySize = 1;
		 depth_textureDesc.Format = DXGI_FORMAT_R32_FLOAT;
		 depth_textureDesc.SampleDesc.Count = 1;
		 depth_textureDesc.SampleDesc.Quality = 0;
		 depth_textureDesc.Usage = D3D11_USAGE_DEFAULT;
		 depth_textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

		//RenderTargetView (Screen Quad용) => DESC 만들기.
		D3D11_RENDER_TARGET_VIEW_DESC  depth_renderTargetViewDesc{};
		 depth_renderTargetViewDesc.Format = depth_textureDesc.Format;
		 depth_renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		//ShaderResourceView (Screen Quad용) => DESC 만들기.
		D3D11_SHADER_RESOURCE_VIEW_DESC depth_shaderResourceViewDesc{};
		 depth_shaderResourceViewDesc.Format = depth_textureDesc.Format;
		 depth_shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		//shaderResourceViewDesc.Texture2D.MostDetailedMip = -1;
		 depth_shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		//shaderResourceViewDesc.Texture2D.MipLevels = 1;
		 depth_shaderResourceViewDesc.Texture2D.MipLevels = 1;

		 HR(m_d3dDevice.Get()->CreateTexture2D(&depth_textureDesc, NULL,
			 &(_deferredPhongShaderQuad->_phongDepthGBuffer._texture)));
		 HR(m_d3dDevice.Get()->CreateRenderTargetView(_deferredPhongShaderQuad->_phongDepthGBuffer._texture,
			 &depth_renderTargetViewDesc, &(_deferredPhongShaderQuad->_phongDepthGBuffer._renderTargetView)));
		 HR(m_d3dDevice.Get()->CreateShaderResourceView(_deferredPhongShaderQuad->_phongDepthGBuffer._texture,
			 &depth_shaderResourceViewDesc, &(_deferredPhongShaderQuad->_phongDepthGBuffer._shaderResourceView)));
	}

	void LowLevelDX11::SetupPBRDeferredResources()
	{
		///MipLevel 조정 : 0으로!
		/*
		miplevel 0 is the full resolution texture, miplevel 1 is half resolution in each dimension, mipl
		evel 2 is quarter resolution in each dimension, etc.
		https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_texture2d_desc
		https://gamedev.stackexchange.com/questions/142402/how-to-resize-a-d3d11texture2d
		*/

		//렌더 타겟 텍스쳐 (Screen Quad용) => DESC 만들기.
		D3D11_TEXTURE2D_DESC textureDesc{};
		textureDesc.Width = this->m_WindowWidth;
		textureDesc.Height = this->m_WindowHeight;
		//textureDesc.Width = this->m_TargetResolutionWidth;
		//textureDesc.Height = this->m_TargetResolutionHeight;
		//textureDesc.MipLevels = 1;
		textureDesc.MipLevels = 0;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

		//RenderTargetView (Screen Quad용) => DESC 만들기.
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		//ShaderResourceView (Screen Quad용) => DESC 만들기.
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{};
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		//shaderResourceViewDesc.Texture2D.MostDetailedMip = -1;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		//shaderResourceViewDesc.Texture2D.MipLevels = 1;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		//모든 DESC들이 세팅되었으니, 이제는 싹다 만들어주자!
		for (unsigned int i = 0; i < ScreenBuffer::DEFERRED_PBR_BUFFER_COUNT-1; i++)
		{
			HR(m_d3dDevice.Get()->CreateTexture2D(&textureDesc, NULL,
				&(_deferredPBRShaderQuad->_trtPbrGBuffers[i]._texture)));
		}
		for (unsigned int i = 0; i < ScreenBuffer::DEFERRED_PBR_BUFFER_COUNT-1; i++)
		{
			HR(m_d3dDevice.Get()->CreateRenderTargetView(_deferredPBRShaderQuad->_trtPbrGBuffers[i]._texture,
				&renderTargetViewDesc, &(_deferredPBRShaderQuad->_trtPbrGBuffers[i]._renderTargetView)));
		}

		//여기서 뻑이 난다. SRV 호환...?
		for (unsigned int i = 0; i < ScreenBuffer::DEFERRED_PBR_BUFFER_COUNT-1; i++)
		{
			///E_INVALIDARG
			HR(m_d3dDevice.Get()->CreateShaderResourceView(_deferredPBRShaderQuad->_trtPbrGBuffers[i]._texture,
				&shaderResourceViewDesc, &(_deferredPBRShaderQuad->_trtPbrGBuffers[i]._shaderResourceView)));
		}

		//한번 Depth는 다른 포맷으로 설정해보자.
		D3D11_TEXTURE2D_DESC depth_textureDesc{};
		depth_textureDesc.Width = this->m_WindowWidth;
		depth_textureDesc.Height = this->m_WindowHeight;
		//textureDesc.Width = this->m_TargetResolutionWidth;
		//textureDesc.Height = this->m_TargetResolutionHeight;
		//textureDesc.MipLevels = 1;
		depth_textureDesc.MipLevels = 0;
		depth_textureDesc.ArraySize = 1;
		depth_textureDesc.Format = DXGI_FORMAT_R32_FLOAT;
		depth_textureDesc.SampleDesc.Count = 1;
		depth_textureDesc.SampleDesc.Quality = 0;
		depth_textureDesc.Usage = D3D11_USAGE_DEFAULT;
		depth_textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

		//RenderTargetView (Screen Quad용) => DESC 만들기.
		D3D11_RENDER_TARGET_VIEW_DESC  depth_renderTargetViewDesc{};
		depth_renderTargetViewDesc.Format = depth_textureDesc.Format;
		depth_renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		//ShaderResourceView (Screen Quad용) => DESC 만들기.
		D3D11_SHADER_RESOURCE_VIEW_DESC depth_shaderResourceViewDesc{};
		depth_shaderResourceViewDesc.Format = depth_textureDesc.Format;
		depth_shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		//shaderResourceViewDesc.Texture2D.MostDetailedMip = -1;
		depth_shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		//shaderResourceViewDesc.Texture2D.MipLevels = 1;
		depth_shaderResourceViewDesc.Texture2D.MipLevels = 1;

		HR(m_d3dDevice.Get()->CreateTexture2D(&depth_textureDesc, NULL,
			&(_deferredPBRShaderQuad->_pbrDepthGBuffer._texture)));
		HR(m_d3dDevice.Get()->CreateRenderTargetView(_deferredPBRShaderQuad->_pbrDepthGBuffer._texture,
			&depth_renderTargetViewDesc, &(_deferredPBRShaderQuad->_pbrDepthGBuffer._renderTargetView)));
		HR(m_d3dDevice.Get()->CreateShaderResourceView(_deferredPBRShaderQuad->_pbrDepthGBuffer._texture,
			&depth_shaderResourceViewDesc, &(_deferredPBRShaderQuad->_pbrDepthGBuffer._shaderResourceView)));

	}

	void LowLevelDX11::SetupPBRSkinnedDeferredResources()
	{
		///MipLevel 조정 : 0으로!
		/*
		miplevel 0 is the full resolution texture, miplevel 1 is half resolution in each dimension, mipl
		evel 2 is quarter resolution in each dimension, etc.
		https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_texture2d_desc
		https://gamedev.stackexchange.com/questions/142402/how-to-resize-a-d3d11texture2d
		*/

		//렌더 타겟 텍스쳐 (Screen Quad용) => DESC 만들기.
		D3D11_TEXTURE2D_DESC textureDesc{};
		textureDesc.Width = this->m_WindowWidth;
		textureDesc.Height = this->m_WindowHeight;
		//textureDesc.Width = this->m_TargetResolutionWidth;
		//textureDesc.Height = this->m_TargetResolutionHeight;
		//textureDesc.MipLevels = 1;
		textureDesc.MipLevels = 0;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

		//RenderTargetView (Screen Quad용) => DESC 만들기.
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		//ShaderResourceView (Screen Quad용) => DESC 만들기.
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{};
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		//shaderResourceViewDesc.Texture2D.MostDetailedMip = -1;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		//shaderResourceViewDesc.Texture2D.MipLevels = 1;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		//모든 DESC들이 세팅되었으니, 이제는 싹다 만들어주자!
		for (unsigned int i = 0; i < ScreenBuffer::DEFERRED_PBR_BUFFER_COUNT-1; i++)
		{
			HR(m_d3dDevice.Get()->CreateTexture2D(&textureDesc, NULL,
				&(_deferredPBRSkinnedShaderQuad->_trtPbrGBuffers[i]._texture)));
		}
		for (unsigned int i = 0; i < ScreenBuffer::DEFERRED_PBR_BUFFER_COUNT-1; i++)
		{
			HR(m_d3dDevice.Get()->CreateRenderTargetView(_deferredPBRSkinnedShaderQuad->_trtPbrGBuffers[i]._texture,
				&renderTargetViewDesc, &(_deferredPBRSkinnedShaderQuad->_trtPbrGBuffers[i]._renderTargetView)));
		}

		//여기서 뻑이 난다. SRV 호환...?
		for (unsigned int i = 0; i < ScreenBuffer::DEFERRED_PBR_BUFFER_COUNT-1; i++)
		{
			///E_INVALIDARG
			HR(m_d3dDevice.Get()->CreateShaderResourceView(_deferredPBRSkinnedShaderQuad->_trtPbrGBuffers[i]._texture,
				&shaderResourceViewDesc, &(_deferredPBRSkinnedShaderQuad->_trtPbrGBuffers[i]._shaderResourceView)));
		}

		////한번 Depth는 다른 포맷으로 설정해보자.
		//D3D11_TEXTURE2D_DESC arm_textureDesc{};
		//arm_textureDesc.Width = this->m_WindowWidth;
		//arm_textureDesc.Height = this->m_WindowHeight;
		////textureDesc.Width = this->m_TargetResolutionWidth;
		////textureDesc.Height = this->m_TargetResolutionHeight;
		////textureDesc.MipLevels = 1;
		//arm_textureDesc.MipLevels = 0;
		//arm_textureDesc.ArraySize = 1;
		//arm_textureDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		//arm_textureDesc.SampleDesc.Count = 1;
		//arm_textureDesc.SampleDesc.Quality = 0;
		//arm_textureDesc.Usage = D3D11_USAGE_DEFAULT;
		//arm_textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		//
		////RenderTargetView (Screen Quad용) => DESC 만들기.
		//D3D11_RENDER_TARGET_VIEW_DESC  arm_renderTargetViewDesc{};
		//arm_renderTargetViewDesc.Format = arm_textureDesc.Format;
		//arm_renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		//
		////ShaderResourceView (Screen Quad용) => DESC 만들기.
		//D3D11_SHADER_RESOURCE_VIEW_DESC arm_shaderResourceViewDesc{};
		//arm_shaderResourceViewDesc.Format = arm_textureDesc.Format;
		//arm_shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		////shaderResourceViewDesc.Texture2D.MostDetailedMip = -1;
		//arm_shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		////shaderResourceViewDesc.Texture2D.MipLevels = 1;
		//arm_shaderResourceViewDesc.Texture2D.MipLevels = 1;
		//
		//HR(m_d3dDevice.Get()->CreateTexture2D(&arm_textureDesc, NULL,
		//	&(_deferredPBRSkinnedShaderQuad->_pbrArmGBuffer._texture)));
		//HR(m_d3dDevice.Get()->CreateRenderTargetView(_deferredPBRSkinnedShaderQuad->_pbrArmGBuffer._texture,
		//	&arm_renderTargetViewDesc, &(_deferredPBRSkinnedShaderQuad->_pbrArmGBuffer._renderTargetView)));
		//HR(m_d3dDevice.Get()->CreateShaderResourceView(_deferredPBRSkinnedShaderQuad->_pbrArmGBuffer._texture,
		//	&arm_shaderResourceViewDesc, &(_deferredPBRSkinnedShaderQuad->_pbrArmGBuffer._shaderResourceView)));

		//한번 Depth는 다른 포맷으로 설정해보자.
		D3D11_TEXTURE2D_DESC depth_textureDesc{};
		depth_textureDesc.Width = this->m_WindowWidth;
		depth_textureDesc.Height = this->m_WindowHeight;
		//textureDesc.Width = this->m_TargetResolutionWidth;
		//textureDesc.Height = this->m_TargetResolutionHeight;
		//textureDesc.MipLevels = 1;
		depth_textureDesc.MipLevels = 0;
		depth_textureDesc.ArraySize = 1;
		depth_textureDesc.Format = DXGI_FORMAT_R32_FLOAT;
		depth_textureDesc.SampleDesc.Count = 1;
		depth_textureDesc.SampleDesc.Quality = 0;
		depth_textureDesc.Usage = D3D11_USAGE_DEFAULT;
		depth_textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

		//RenderTargetView (Screen Quad용) => DESC 만들기.
		D3D11_RENDER_TARGET_VIEW_DESC  depth_renderTargetViewDesc{};
		depth_renderTargetViewDesc.Format = depth_textureDesc.Format;
		depth_renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		//ShaderResourceView (Screen Quad용) => DESC 만들기.
		D3D11_SHADER_RESOURCE_VIEW_DESC depth_shaderResourceViewDesc{};
		depth_shaderResourceViewDesc.Format = depth_textureDesc.Format;
		depth_shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		//shaderResourceViewDesc.Texture2D.MostDetailedMip = -1;
		depth_shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		//shaderResourceViewDesc.Texture2D.MipLevels = 1;
		depth_shaderResourceViewDesc.Texture2D.MipLevels = 1;

		HR(m_d3dDevice.Get()->CreateTexture2D(&depth_textureDesc, NULL,
			&(_deferredPBRSkinnedShaderQuad->_pbrDepthGBuffer._texture)));
		HR(m_d3dDevice.Get()->CreateRenderTargetView(_deferredPBRSkinnedShaderQuad->_pbrDepthGBuffer._texture,
			&depth_renderTargetViewDesc, &(_deferredPBRSkinnedShaderQuad->_pbrDepthGBuffer._renderTargetView)));
		HR(m_d3dDevice.Get()->CreateShaderResourceView(_deferredPBRSkinnedShaderQuad->_pbrDepthGBuffer._texture,
			&depth_shaderResourceViewDesc, &(_deferredPBRSkinnedShaderQuad->_pbrDepthGBuffer._shaderResourceView)));
	}

	void LowLevelDX11::DirectX_CreateBlendStates()
	{
		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(blendDesc));

		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;

		for (UINT i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {
			blendDesc.RenderTarget[i].BlendEnable = FALSE;
			blendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_ZERO;
			blendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
			blendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		}
		m_d3dDevice->CreateBlendState(&blendDesc, m_NoBlend.GetAddressOf());
	}

	void LowLevelDX11::SetupDefaultQuadResources(ScreenBuffer* screenBuffer)
	{

		//m_d3dMainContext->RSSetViewports(
		//	1,
		//	&(m_FullClientViewport)
		//);
		//m_d3dMainContext->RSSetScissorRects(
		//	1,
		//	&(d3dScissorsRect)
		//);

		//렌더 타겟 텍스쳐 (Screen Quad용) => DESC 만들기.
		D3D11_TEXTURE2D_DESC textureDesc{};
		textureDesc.Width = this->m_WindowWidth;
		textureDesc.Height = this->m_WindowHeight;
		//textureDesc.Width = this->m_TargetResolutionWidth;
		//textureDesc.Height = this->m_TargetResolutionHeight;
		///MipLevel 조정 : 0으로!
		/*
		miplevel 0 is the full resolution texture, miplevel 1 is half resolution in each dimension, miplevel 2 is quarter resolution in each dimension, etc.
		*/
		textureDesc.MipLevels = 0;
		//textureDesc.MipLevels = 1; 
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

		//RenderTargetView (Screen Quad용) => DESC 만들기.
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		//ShaderResourceView (Screen Quad용) => DESC 만들기.
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{};
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		//shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		//shaderResourceViewDesc.Texture2D.MipLevels = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;
		
		//기본 TextureRenderTarget 세팅.

		HR(m_d3dDevice.Get()->CreateTexture2D(&textureDesc, NULL,
			&(screenBuffer->_defTextureRenderTarget._texture)));

		HR(m_d3dDevice.Get()->CreateRenderTargetView(screenBuffer->_defTextureRenderTarget._texture,
			&renderTargetViewDesc, &(screenBuffer->_defTextureRenderTarget._renderTargetView)));

		HR(m_d3dDevice.Get()->CreateShaderResourceView(screenBuffer->_defTextureRenderTarget._texture,
			&shaderResourceViewDesc, &(screenBuffer->_defTextureRenderTarget._shaderResourceView)));

		//여기는 디버깅용으로 잘 코드가 돌아가는지 체크하는 영역.
		D3D11_TEXTURE2D_DESC textureDescDebug;
		screenBuffer->_defTextureRenderTarget._texture->GetDesc(&textureDescDebug);

	}

	
	
	

}

