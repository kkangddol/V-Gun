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
	//Material에 따라 Formation이 달라진다.
	//일단은 Formation과 Material을 동시에 받고, 나중에 필요없는 관계를 지우자.
	SkinnedMeshObject::SkinnedMeshObject(ShaderMaterial* _mat, BaseFormation* _form, 
		CameraMatrices* cameraMat, TextureStorage* tStorage, eRenderType renderType,
		DirectX::XMFLOAT3 minBounds, DirectX::XMFLOAT3 maxBounds)
		: BaseRenderable(_mat, _form, cameraMat, tStorage, renderType, minBounds, maxBounds)
	{
		///더는 여기서 ShaderUpdater의 데이터를 Initialize하지 않는다.

		//들어가지 않을 상황을 위해, 기본 바이트버퍼 생성.
		//-> 이미 RenderAnimation에서 만들었다!
		//다만, Model을 유지시켜야 하기에, 처음 포즈를 보관할 것이다. (Animation 딴에서)
		this->m_AnimHandler = std::make_unique<AnimationHandler>(this);
	}

	SkinnedMeshObject::~SkinnedMeshObject()
	{
		//
	}

	///여기서 만들어서 실행해야 한다.
	void SkinnedMeshObject::Render(const RocketEngine::RMFLOAT4X4& worldTM)
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
		_sceneConstantsUpdater->AdjustShaderConstantsPerObject(ttIndexTrans);


		///여기서 오류가 걸린다면 Animation 없이 SkinnedMeshObject가 만들어졌다는 뜻이다.
		SkinnedMeshFormation* tSMF = static_cast<SkinnedMeshFormation*>(_formation);
		assert(m_AnimHandler->_nowPlayingAnim);


		//Material, Texture 시스템 마련되면 이거 되어야.
		_devcon->IASetInputLayout(_formation->GetInputLayout());
		_devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		// 렌더 스테이트
		_devcon->RSSetState(LowLevelDX11::GetInstance()->m_SolidRS.Get());

		UINT stride = _formation->GetSingleBufferSize();
		UINT offset = 0;

		//Animation 관련 버퍼 업데이트
		SkinnedMeshFormation* tAnimForm = static_cast<SkinnedMeshFormation*>(_formation);

		_devcon->IASetVertexBuffers(0, 1, &(tAnimForm->m_VB), &stride, &offset);
		_devcon->IASetIndexBuffer(tAnimForm->m_IB, DXGI_FORMAT_R32_UINT, 0);

		ID3DX11EffectTechnique* tTech = _material->GetShader()->GetMainTechnique();
		D3DX11_TECHNIQUE_DESC tTechDesc;
		tTech->GetDesc(&tTechDesc);

		//Animation 버퍼 클리어 후, 그 사이에 값을 업데이트하는 정보.
		//UpdateVariablesSkinned(tFF, RenderAnimation::_dTimeRecord);
		//_material->GetShader()->UpdateShader();

		///전에 있던 Shader Updating을 통칭해서 호출하는 함수.
		this->UpdateShaderVariables(tFF, RenderAnimation::_dTimeRecord);
		
		//이거 이러면 안된다.
		//Draw하고, Update하고. Draw하고, Update하고, 그래야 한다. 
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


