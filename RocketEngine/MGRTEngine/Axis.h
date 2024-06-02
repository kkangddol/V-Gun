#pragma once

#include "Define3D.h"
#include "d3dx11Effect.h"	// effect, tech

/// <summary>
/// 3D공간의 축을 나타내는 오브젝트
/// 
/// 2021.02.21 LeHideOffice
/// </summary>

namespace RocketCore::Graphics
{
	class Axis
	{
	public:
		Axis(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS);
		~Axis();

		struct Vertex
		{
			XMFLOAT3 Pos;
			XMFLOAT4 Color;
		};

	public:
		void Initialize();
		void Update(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj);
		void Render();


	private:
		void BuildGeometryBuffers();
		void BuildFX();
		void BuildVertexLayout();


		// 박스를 그리기 위해서는 뭐뭐 있어야 하니..
	private:
		ID3D11Device* md3dDevice;						// D3D11 디바이스
		ID3D11DeviceContext* md3dImmediateContext;		// 디바이스 컨텍스트

		ID3D11Buffer* mVB;
		ID3D11Buffer* mIB;

		ID3DX11Effect* mFX;
		ID3DX11EffectTechnique* mTech;
		ID3DX11EffectMatrixVariable* mfxWorldViewProj;

		ID3D11InputLayout* mInputLayout;

		// 변환 관련
		XMMATRIX mWorld;	// 월드 변환 행렬 (로컬->월드)
		XMMATRIX mView;		// 시야 변환 행렬 (카메라 뷰)
		XMMATRIX mProj;		// 투영 변환 행렬 (원근/직교)


		// 와이어로 그리기 위한 RenderState. 쉐이더에서 해도 된다.
		// 예제에서 On/Off로 바꾸는 등의 일을 하는 경우 이것을 바꿔줘도 될 것 같다.
		// 쉐이더에서 하는 경우도 스위칭 비용도 줄일 수 있는 방법은 많다.
		ID3D11RasterizerState* m_pRenderstate;

	};
}