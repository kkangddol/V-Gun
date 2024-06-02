#include "RenderCubeMap.h"
#include "Texture.h"
#include "CameraMatrices.h"
#include "Define3D.h"
#include "LowLevelDX11.h"

namespace RocketCore::Graphics
{
	RenderCubeMap::RenderCubeMap(Texture* texture, CameraMatrices* camMatrix) :
		_camMatrix(camMatrix), _cubeTexture(texture)
	{
		//여기서 NULLPTR가 들어올 일이 없다!
		//이 Texture를 가지고 값을 돌려야 한다.
		///Initialize의 역할을 같이 한다.
		BuildGeometryBuffers();
		BuildFX();
		BuildVertexLayout();
	}

	RenderCubeMap::~RenderCubeMap()
	{
		ReleaseCOM(_FX);
		ReleaseCOM(_layout);
		ReleaseCOM(_VB);
		ReleaseCOM(_IB);
	}

	void RenderCubeMap::Render()
	{
		//큐브맵은 마지막으로 그려야 한다. (자원의 효율적인 사용을 위해서)
		//Shader의 Variable들 업데이트해 주기.
		//Cubemap은 Camera랑 동일한 위치에 존재한다.

		ID3D11DeviceContext* tDevcon = LowLevelDX11::GetInstance()->m_d3dMainContext.Get();

		LowLevelDX11::GetInstance()->m_d3dMainContext->IASetInputLayout(_layout);
		LowLevelDX11::GetInstance()->m_d3dMainContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		LowLevelDX11::GetInstance()->m_d3dMainContext->RSSetState(LowLevelDX11::GetInstance()->m_SolidRS.Get());
		
		UINT stride = sizeof(GeometryGenerator::GeomVertex_PosColor);
		UINT offset = 0;

		tDevcon->IASetVertexBuffers(0, 1, &_VB, &stride, &offset);
		tDevcon->IASetIndexBuffer(_IB, DXGI_FORMAT_R32_UINT, 0);

		D3DX11_TECHNIQUE_DESC tTechDesc;
		_tech->GetDesc(&tTechDesc);

		XMMATRIX tWorld = DirectX::XMMatrixTranslation(_camMatrix->mPosition.x, _camMatrix->mPosition.y, _camMatrix->mPosition.z);
		XMMATRIX worldViewProj = tWorld * _camMatrix->View() * _camMatrix->Proj();
		_wvp->SetMatrix(reinterpret_cast<float*>(&worldViewProj));
		_cubeShaderVar->SetResource(_cubeTexture->GetSRV());

		for (UINT p = 0; p < tTechDesc.Passes; ++p)
		{
			_tech->GetPassByIndex(p)->Apply(0, tDevcon);
			tDevcon->DrawIndexed(_indexCount, 0, 0);
		}
	}

	void RenderCubeMap::BuildGeometryBuffers()
	{
		// 정점 버퍼를 원을 렌더하는 Vertex Buffer를 만든다.
		GeometryGenerator tGen;
		tGen.CreateSphere(1000, 100, 100, _geomMeshData);

		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(GeometryGenerator::GeomVertex_PosColor) * _geomMeshData.Vertices.size();
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = &(_geomMeshData.Vertices[0]);
		HR(LowLevelDX11::GetInstance()->m_d3dDevice->CreateBuffer(&vbd, &vinitData, &_VB));

		this->_indexCount = _geomMeshData.Indices.size();

		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(UINT) * _geomMeshData.Indices.size();
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = &(_geomMeshData.Indices[0]);
		HR(LowLevelDX11::GetInstance()->m_d3dDevice->CreateBuffer(&ibd, &iinitData, &_IB));
	}

	void RenderCubeMap::BuildVertexLayout()
	{
		D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		// Create the input layout
		D3DX11_PASS_DESC passDesc;
		_tech->GetPassByIndex(0)->GetDesc(&passDesc);
		HR(LowLevelDX11::GetInstance()->m_d3dDevice->CreateInputLayout(vertexDesc, 1, passDesc.pIAInputSignature,
			passDesc.IAInputSignatureSize, &_layout));
	}

	void RenderCubeMap::BuildFX()
	{
		std::ifstream fin("../FX/Default/CubemapShader.cso", std::ios::binary);

		fin.seekg(0, std::ios_base::end);
		int size = (int)fin.tellg();
		fin.seekg(0, std::ios_base::beg);
		std::vector<char> compiledShader(size);

		fin.read(&compiledShader[0], size);
		fin.close();

		HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size,
			0, LowLevelDX11::GetInstance()->m_d3dDevice.Get(), &_FX));

		_tech = _FX->GetTechniqueByName("SkyTech");
		_wvp = _FX->GetVariableByName("gWorldViewProj")->AsMatrix();
		_cubeShaderVar = _FX->GetVariableByName("gCubeMap")->AsShaderResource();
	}



}

