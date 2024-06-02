#include "MeshObject.h"
#include "MeshFormation.h"
#include "MathHelper.h"
#include "CameraMatrices.h"
#include "ShaderMaterial.h"
#include "LowLevelDX11.h"
#include "Shader.h"
#include "Texture.h"
#include <d3d11.h>
#include <cstdlib>
#include "TypedefGraphicHelper.h"
#include "SceneConstantsUpdater.h"

namespace RocketCore::Graphics
{
	MeshObject::MeshObject(ShaderMaterial* _mat, BaseFormation* _form, 
		CameraMatrices* cameraMat, TextureStorage* tStorage, eRenderType renderType, 
		DirectX::XMFLOAT3 minBounds, DirectX::XMFLOAT3 maxBounds)
		: BaseRenderable(_mat, _form, cameraMat, tStorage, renderType, minBounds, maxBounds)
	{
		
	}

	void MeshObject::Render(const RocketEngine::RMFLOAT4X4& worldTM)
{
		///���� ��ǥ ������!
		///Z�� �������� ����, worldTM���ٰ� Rotation�� ���ؼ� ó���� ��.
		//World TM ��� ������ �Ҵ�. 
		DirectX::XMFLOAT4X4 tempFF = MathHelper::RMToXMFloat4x4(worldTM);
		DirectX::XMMATRIX tempFFMat = XMLoadFloat4x4(&tempFF);
		DirectX::XMVECTOR ftScale;
		DirectX::XMVECTOR ftRot;
		DirectX::XMVECTOR ftTrans;
		DirectX::XMMatrixDecompose(&ftScale, &ftRot, &ftTrans, tempFFMat);

		///[TW] Culling Check!
		DirectX::XMFLOAT3 ftPos;
		DirectX::XMStoreFloat3(&ftPos, ftTrans);
		if (MathHelper::CheckIfFrustumCulled(_cameraMatrix->_frustumPlanes, ftPos, _minBoundingBox, _maxBoundingBox))
		{
			return;
		}

		DirectX::XMMATRIX ftOriginScale = XMMatrixScalingFromVector(ftScale);
		DirectX::XMMATRIX ftOriginRot = XMMatrixRotationQuaternion(ftRot);
		DirectX::XMMATRIX tRotYHalfPi = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(180.0f));
		DirectX::XMMATRIX ftOriginTrans = XMMatrixTranslationFromVector(ftTrans);
		
		//OriginScale : 1/100�� ���̱�.
		DirectX::XMMATRIX ftHundredToOne = XMMatrixScaling(0.01f, 0.01f, 0.01f);

		DirectX::XMMATRIX tFFMat =
			XMMatrixMultiply(XMMatrixMultiply(XMMatrixMultiply(ftOriginScale, ftHundredToOne), ftOriginRot), ftOriginTrans);
		DirectX::XMFLOAT4X4 tFF;
		XMStoreFloat4x4(&tFF, tFFMat);
		///��ǥ ������ ����. 
		
		///�ϴ� ���̴� ��ġ�� ���� Light Index ������Ʈ!
		DirectX::XMFLOAT3 ttIndexTrans;
		DirectX::XMStoreFloat3(&ttIndexTrans, ftTrans);
		_sceneConstantsUpdater->AdjustShaderConstantsPerObject(ttIndexTrans); ///
		
		//Material, Texture �ý��� ���õǸ� �̰� �Ǿ��.
		_devcon->IASetInputLayout(_formation->GetInputLayout());
		_devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// ���� ������Ʈ
		_devcon->RSSetState(LowLevelDX11::GetInstance()->m_SolidRS.Get());

		UINT stride = _formation->GetSingleBufferSize();
		UINT offset = 0;

		MeshFormation* tMeshForm = static_cast<MeshFormation*>(_formation);

		_devcon->IASetVertexBuffers(0, 1, &(tMeshForm->m_VB), &stride, &offset);
		_devcon->IASetIndexBuffer(tMeshForm->m_IB, DXGI_FORMAT_R32_UINT, 0);

		ID3DX11EffectTechnique* tTech = _material->GetShader()->GetMainTechnique();
		D3DX11_TECHNIQUE_DESC tTechDesc;
		tTech->GetDesc(&tTechDesc);

		////Variable��, ��Ƽ� UpdateSingleVariable�� ȣ��.
		//UpdateVariables(tFF);
		////�� ���̿� ���� ������Ʈ�ϴ� ����.
		//_material->GetShader()->UpdateShader();
		
		///���� �ִ� Shader Updating�� ��Ī�ؼ� ȣ���ϴ� �Լ�. 
		this->UpdateShaderVariables(tFF, 0.016); //Interval�� �׳� MeshObject������ ������ ����.

		for (UINT p = 0; p < tTechDesc.Passes; ++p)
		{
			tTech->GetPassByIndex(p)->Apply(0, _devcon);
			_devcon->DrawIndexed(tMeshForm->m_IndexCount, 0, 0);
		}
	}

	void MeshObject::Render(const RocketEngine::RMFLOAT4X4& worldTM, const LegacyMaterialData& m)
	{
		this->_legacyMaterialData = m;
		Render(worldTM);
	}

	void MeshObject::Render(const RocketEngine::RMFLOAT4X4& worldTM, const PBRMaterialData& m)
	{
		this->_pbrMaterialData = m;
		Render(worldTM);
	}
}