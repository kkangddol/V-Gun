#include "SkinnedMeshObject.h"
#include "SkinnedMeshFormation.h"
#include "Transform.h"
#include "ShaderMaterial.h"
#include "Shader.h"
#include "MathHelper.h"
#include "ByteBuffer.hpp"
#include "LowLevelDX11.h"
#include "Texture.h"
#include "MathHelper.h"
#include "CameraMatrices.h"
#include "AnimationHandler.h"
#include "SceneConstantsUpdater.h"
#include "ResourceManager.h"

#include "RenderUsageBoneInfo.h"
#include "RenderUsageMesh.h"
#include "RenderUsageVertexBone.h"
#include "RenderAnimation.h"
#include <d3d11.h>
#include <cassert>

namespace RocketCore::Graphics
{
	//Material�� ���� Formation�� �޶�����.
	//�ϴ��� Formation�� Material�� ���ÿ� �ް�, ���߿� �ʿ���� ���踦 ������.
	SkinnedMeshObject::SkinnedMeshObject(ShaderMaterial* _mat, BaseFormation* _form, 
		CameraMatrices* cameraMat, TextureStorage* tStorage, eRenderType renderType,
		DirectX::XMFLOAT3 minBounds, DirectX::XMFLOAT3 maxBounds)
		: BaseRenderable(_mat, _form, cameraMat, tStorage, renderType, minBounds, maxBounds)
	{
		///���� ���⼭ ShaderUpdater�� �����͸� Initialize���� �ʴ´�.

		//���� ���� ��Ȳ�� ����, �⺻ ����Ʈ���� ����.
		//-> �̹� RenderAnimation���� �������!
		//�ٸ�, Model�� �������Ѿ� �ϱ⿡, ó�� ��� ������ ���̴�. (Animation ������)
		this->m_AnimHandler = std::make_unique<AnimationHandler>(this);
	}

	SkinnedMeshObject::~SkinnedMeshObject()
	{
		//
	}

	///���⼭ ���� �����ؾ� �Ѵ�.
	void SkinnedMeshObject::Render(const RocketEngine::RMFLOAT4X4& worldTM)
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
		_sceneConstantsUpdater->AdjustShaderConstantsPerObject(ttIndexTrans);


		///���⼭ ������ �ɸ��ٸ� Animation ���� SkinnedMeshObject�� ��������ٴ� ���̴�.
		SkinnedMeshFormation* tSMF = static_cast<SkinnedMeshFormation*>(_formation);
		assert(m_AnimHandler->_nowPlayingAnim);


		//Material, Texture �ý��� ���õǸ� �̰� �Ǿ��.
		_devcon->IASetInputLayout(_formation->GetInputLayout());
		_devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		// ���� ������Ʈ
		_devcon->RSSetState(LowLevelDX11::GetInstance()->m_SolidRS.Get());

		UINT stride = _formation->GetSingleBufferSize();
		UINT offset = 0;

		//Animation ���� ���� ������Ʈ
		SkinnedMeshFormation* tAnimForm = static_cast<SkinnedMeshFormation*>(_formation);

		_devcon->IASetVertexBuffers(0, 1, &(tAnimForm->m_VB), &stride, &offset);
		_devcon->IASetIndexBuffer(tAnimForm->m_IB, DXGI_FORMAT_R32_UINT, 0);

		ID3DX11EffectTechnique* tTech = _material->GetShader()->GetMainTechnique();
		D3DX11_TECHNIQUE_DESC tTechDesc;
		tTech->GetDesc(&tTechDesc);

		//Animation ���� Ŭ���� ��, �� ���̿� ���� ������Ʈ�ϴ� ����.
		//UpdateVariablesSkinned(tFF, RenderAnimation::_dTimeRecord);
		//_material->GetShader()->UpdateShader();

		///���� �ִ� Shader Updating�� ��Ī�ؼ� ȣ���ϴ� �Լ�.
		this->UpdateShaderVariables(tFF, RenderAnimation::_dTimeRecord);
		
		//�̰� �̷��� �ȵȴ�.
		//Draw�ϰ�, Update�ϰ�. Draw�ϰ�, Update�ϰ�, �׷��� �Ѵ�. 
		for (UINT p = 0; p < tTechDesc.Passes; ++p)
		{
			tTech->GetPassByIndex(p)->Apply(0, _devcon);
			_devcon->DrawIndexed(tAnimForm->m_IndexCount, 0, 0);
		}
	}

	void SkinnedMeshObject::Render(const RocketEngine::RMFLOAT4X4& worldTM, const LegacyMaterialData& m)
	{
		this->_legacyMaterialData = m;
		Render(worldTM);
	}

	void SkinnedMeshObject::Render(const RocketEngine::RMFLOAT4X4& worldTM, const PBRMaterialData& m)
	{
		this->_pbrMaterialData = m;
		Render(worldTM);
	}

	void SkinnedMeshObject::PlayAnim(AnimData animData)
	{
		if (animData.name == "")
		{
			return;
		}
		//SkinnedMeshFormation* tSkinForm = static_cast<SkinnedMeshFormation*>(_formation);
		m_AnimHandler->Play(animData);
	}

	

}


