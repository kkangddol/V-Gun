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
		///월드 좌표 뒤집기!
		///Z가 뒤집히는 문제, worldTM에다가 Rotation을 곱해서 처리할 것.
		//World TM 멤버 변수에 할당. 
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
		
		//OriginScale : 1/100로 줄이기.
		DirectX::XMMATRIX ftHundredToOne = XMMatrixScaling(0.01f, 0.01f, 0.01f);

		DirectX::XMMATRIX tFFMat =
			XMMatrixMultiply(XMMatrixMultiply(XMMatrixMultiply(ftOriginScale, ftHundredToOne), ftOriginRot), ftOriginTrans);
		DirectX::XMFLOAT4X4 tFF;
		XMStoreFloat4x4(&tFF, tFFMat);
		///좌표 돌리기 끝냄. 
		
		///일단 셰이더 위치에 따라 Light Index 업데이트!
		DirectX::XMFLOAT3 ttIndexTrans;
		DirectX::XMStoreFloat3(&ttIndexTrans, ftTrans);
		_sceneConstantsUpdater->AdjustShaderConstantsPerObject(ttIndexTrans); ///
		
		//Material, Texture 시스템 마련되면 이거 되어야.
		_devcon->IASetInputLayout(_formation->GetInputLayout());
		_devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// 렌더 스테이트
		_devcon->RSSetState(LowLevelDX11::GetInstance()->m_SolidRS.Get());

		UINT stride = _formation->GetSingleBufferSize();
		UINT offset = 0;

		MeshFormation* tMeshForm = static_cast<MeshFormation*>(_formation);

		_devcon->IASetVertexBuffers(0, 1, &(tMeshForm->m_VB), &stride, &offset);
		_devcon->IASetIndexBuffer(tMeshForm->m_IB, DXGI_FORMAT_R32_UINT, 0);

		ID3DX11EffectTechnique* tTech = _material->GetShader()->GetMainTechnique();
		D3DX11_TECHNIQUE_DESC tTechDesc;
		tTech->GetDesc(&tTechDesc);

		////Variable들, 모아서 UpdateSingleVariable들 호출.
		//UpdateVariables(tFF);
		////그 사이에 값을 업데이트하는 정보.
		//_material->GetShader()->UpdateShader();
		
		///전에 있던 Shader Updating을 통칭해서 호출하는 함수. 
		this->UpdateShaderVariables(tFF, 0.016); //Interval이 그냥 MeshObject에서는 쓰이지 않음.

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