#include "Axis.h"
#include <DirectXColors.h>	// �̹� ���� �κ� DX11�� ��ģ��.

namespace RocketCore::Graphics
{
	Axis::Axis(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS)
		: md3dDevice(pDevice), md3dImmediateContext(pDeviceContext), m_pRenderstate(pRS),
		mVB(nullptr), mIB(nullptr), mFX(nullptr), mTech(nullptr), mfxWorldViewProj(nullptr), mInputLayout(nullptr),
		mWorld(), mView(), mProj()
	{

	}

	Axis::~Axis()
	{
		ReleaseCOM(mFX);
		ReleaseCOM(mInputLayout);

		ReleaseCOM(mVB);
		ReleaseCOM(mIB);
	}


	void Axis::Initialize()
	{
		BuildGeometryBuffers();
		BuildFX();
		BuildVertexLayout();
	}

	void Axis::Update(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj)
	{
		mWorld = world;
		mView = view;
		mProj = proj;
	}

	void Axis::Render()
	{
		// �Է� ��ġ ��ü ����
		md3dImmediateContext->IASetInputLayout(mInputLayout);
		md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		// �ε������ۿ� ���ؽ����� ����
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		md3dImmediateContext->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
		md3dImmediateContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

		/// WVP TM���� ����
		// Set constants
		XMMATRIX worldViewProj = mWorld * mView * mProj;
		mfxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&worldViewProj));

		// ����������Ʈ
		md3dImmediateContext->RSSetState(m_pRenderstate);

		// ��ũ����...
		D3DX11_TECHNIQUE_DESC techDesc;
		mTech->GetDesc(&techDesc);

		// �����н���...
		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			mTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);

			// 6���� �ε����� ���� �׸���.
			md3dImmediateContext->DrawIndexed(6, 0, 0);
		}
	}

	void Axis::BuildGeometryBuffers()
	{
		// ���� ���۸� �����Ѵ�. 
		// �� �࿡ �µ��� 6���� ������ �������.
		Vertex vertices[] =
		{
			{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Red)  },	// x�� (����)
			{ XMFLOAT3(100.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Red)  },

			{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Green)},	// y�� (�ʷ�)
			{ XMFLOAT3(0.0f, 100.0f, 0.0f), XMFLOAT4((const float*)&Colors::Green)},

			{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Blue)	},	// z�� (�Ķ�)
			{ XMFLOAT3(0.0f, 0.0f, 100.0f), XMFLOAT4((const float*)&Colors::Blue) }
		};

		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(Vertex) * 6;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = vertices;
		HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));


		// �ε��� ���۸� �����Ѵ�.
		// ���� 3���� ���� ��Ÿ������ �ߴ�.

		UINT indices[] = {
			// x��
			0, 1,

			// y��
			2, 3,

			// z��
			4, 5,
		};

		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(UINT) * 6;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = indices;
		HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
	}

	void Axis::BuildFX()
	{
		std::ifstream fin("../FX/Default/color.fxo", std::ios::binary);

		fin.seekg(0, std::ios_base::end);
		int size = (int)fin.tellg();
		fin.seekg(0, std::ios_base::beg);
		std::vector<char> compiledShader(size);

		fin.read(&compiledShader[0], size);
		fin.close();

		HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size,
			0, md3dDevice, &mFX));

		mTech = mFX->GetTechniqueByName("ColorTech");
		mfxWorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	}

	void Axis::BuildVertexLayout()
	{
		// Create the vertex input layout.
		D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		// Create the input layout
		D3DX11_PASS_DESC passDesc;
		mTech->GetPassByIndex(0)->GetDesc(&passDesc);
		HR(md3dDevice->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature,
			passDesc.IAInputSignatureSize, &mInputLayout));
	}
}