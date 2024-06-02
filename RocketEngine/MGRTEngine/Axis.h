#pragma once

#include "Define3D.h"
#include "d3dx11Effect.h"	// effect, tech

/// <summary>
/// 3D������ ���� ��Ÿ���� ������Ʈ
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


		// �ڽ��� �׸��� ���ؼ��� ���� �־�� �ϴ�..
	private:
		ID3D11Device* md3dDevice;						// D3D11 ����̽�
		ID3D11DeviceContext* md3dImmediateContext;		// ����̽� ���ؽ�Ʈ

		ID3D11Buffer* mVB;
		ID3D11Buffer* mIB;

		ID3DX11Effect* mFX;
		ID3DX11EffectTechnique* mTech;
		ID3DX11EffectMatrixVariable* mfxWorldViewProj;

		ID3D11InputLayout* mInputLayout;

		// ��ȯ ����
		XMMATRIX mWorld;	// ���� ��ȯ ��� (����->����)
		XMMATRIX mView;		// �þ� ��ȯ ��� (ī�޶� ��)
		XMMATRIX mProj;		// ���� ��ȯ ��� (����/����)


		// ���̾�� �׸��� ���� RenderState. ���̴����� �ص� �ȴ�.
		// �������� On/Off�� �ٲٴ� ���� ���� �ϴ� ��� �̰��� �ٲ��൵ �� �� ����.
		// ���̴����� �ϴ� ��쵵 ����Ī ��뵵 ���� �� �ִ� ����� ����.
		ID3D11RasterizerState* m_pRenderstate;

	};
}