#pragma once
#include "Define3D.h"
#include "GeometryGenerator.h"
namespace RocketCore::Graphics
{
	class Grid
	{
	public:
		Grid(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS);
		~Grid();

		void Initialize();

		void Update(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj);
		void Render();

		void BuildGeometryBuffers();
		void BuildFX();
		void BuildVertexLayout();
	private:
		unsigned int m_HorizontalCount;
		unsigned int m_VerticalCount;

		ID3D11Device* md3dDevice;						// D3D11 디바이스
		ID3D11DeviceContext* md3dImmediateContext;		// 디바이스 컨텍스트

		ID3D11Buffer* mVB;
		ID3D11Buffer* mIB;

		ID3DX11Effect* mFX;
		ID3DX11EffectTechnique* mTech;
		ID3DX11EffectMatrixVariable* mfxWorldViewProj;

		ID3D11InputLayout* mInputLayout;

		ID3D11RasterizerState* m_pRenderstate;

		XMMATRIX mWorld;	// Transform Matrix
		XMMATRIX mView;
		XMMATRIX mProj;

		GeometryGenerator::MeshData_PosColor _geomMeshData;
	};
}


