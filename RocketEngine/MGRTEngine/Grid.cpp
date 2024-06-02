#include "Grid.h"
#include <DirectXColors.h>


namespace RocketCore::Graphics
{
	Grid::Grid(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS)
		: md3dDevice(pDevice), md3dImmediateContext(pDeviceContext), m_pRenderstate(pRS),
		mVB(nullptr), mIB(nullptr), mFX(nullptr), mTech(nullptr), mfxWorldViewProj(nullptr), mInputLayout(nullptr),
		mWorld(), mView(), mProj()
	{

	}

	Grid::~Grid()
	{
		ReleaseCOM(mFX);
		ReleaseCOM(mInputLayout);

		ReleaseCOM(mVB);
		ReleaseCOM(mIB);
	}


	void Grid::Initialize()
	{
		BuildGeometryBuffers();
		BuildFX();
		BuildVertexLayout();
	}

	void Grid::Update(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj)
	{
		mWorld = world;
		mView = view;
		mProj = proj;
	}

	void Grid::Render()
	{
		// 입력 배치 객체 셋팅
		md3dImmediateContext->IASetInputLayout(mInputLayout);
		md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		// 인덱스버퍼와 버텍스버퍼 셋팅
		UINT stride = sizeof(GeometryGenerator::GeomVertex_PosColor);
		UINT offset = 0;
		md3dImmediateContext->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
		md3dImmediateContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

		/// WVP TM등을 셋팅
		// Set constants
		XMMATRIX worldViewProj = mWorld * mView * mProj;
		mfxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&worldViewProj));

		// 렌더스테이트
		md3dImmediateContext->RSSetState(m_pRenderstate);

		// 테크닉은...
		D3DX11_TECHNIQUE_DESC techDesc;
		mTech->GetDesc(&techDesc);

		// 랜더패스는...
		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			mTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);

			// 20개의 인덱스로 그리드를 그린다.
			md3dImmediateContext->DrawIndexed(_geomMeshData.Indices.size(), 0, 0);
		}
	}


	void Grid::BuildGeometryBuffers()
	{
		// 정점 버퍼를 생성한다. 
		// 40개의 정점을 만들었다.
		GeometryGenerator tGen;
		tGen.CreateGrid(60, 60, 61, 61, _geomMeshData);


		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(GeometryGenerator::GeomVertex_PosColor) * _geomMeshData.Vertices.size();
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = &(_geomMeshData.Vertices[0]);
		HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));

		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(UINT) * _geomMeshData.Indices.size();
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = &(_geomMeshData.Indices[0]);
		HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
	}

	void Grid::BuildFX()
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

		//assert(mfxWorldViewProj);
		//size++;
	}

	void Grid::BuildVertexLayout()
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