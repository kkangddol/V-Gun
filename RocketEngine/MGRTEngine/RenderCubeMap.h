#pragma once
#include <string>
#include "GeometryGenerator.h"

struct ID3D11Buffer;
struct ID3DX11Effect;
struct ID3DX11EffectTechnique;
struct ID3DX11EffectMatrixVariable;
struct ID3D11InputLayout;

namespace RocketCore::Graphics
{
	class Texture;
	class CameraMatrices;

	class RenderCubeMap
	{
	public:
		RenderCubeMap(Texture* texture, CameraMatrices* camMatrix);
		~RenderCubeMap();

		void Render();

	private:
		CameraMatrices* _camMatrix = nullptr;
		Texture* _cubeTexture = nullptr;

		GeometryGenerator::MeshData_PosColor _geomMeshData;
		ID3D11Buffer* _VB = nullptr;
		ID3D11Buffer* _IB = nullptr;

		//Cubemap ���̴��� ���� �ڵ�.
		ID3DX11Effect* _FX = nullptr;
		ID3DX11EffectTechnique* _tech = nullptr;
		ID3DX11EffectMatrixVariable* _wvp = nullptr;
		ID3D11InputLayout* _layout = nullptr;
		//���� ť��� SRV:
		ID3DX11EffectShaderResourceVariable* _cubeShaderVar = nullptr;

		int _indexCount = 0;

		//�� ������� GeometryBuffer�� �����Ѵ�.
		void BuildGeometryBuffers();
		void BuildVertexLayout();
		void BuildFX();
	};
}



