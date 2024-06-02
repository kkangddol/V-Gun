#include "ShaderUpdater.h"
#include "SkinnedMeshFormation.h"
#include "AnimationHandler.h"
#include "RenderAnimation.h"
#include "SceneConstantsUpdater.h"
#include "LowLevelDX11.h"
#include "ScreenBufferPhong.h"
#include "ScreenBufferPBR.h"
#include "ResourceManager.h"
#include "TextureRenderTarget.h"
#include "CameraMatrices.h"
#include "MathHelper.h"

namespace RocketCore::Graphics
{
	///Ư��ȭ�� ���ø� �Լ����� CPP�� ���°� �´�! �� ��찡 �ƴϸ�, ����� ���� �Ű�.
	///Ư��ȭ�� �� �������� �Ϲ� �Լ���� �����ϰ� ó���ȴٰ� �����ϸ� �ȴ�.

	///Ư��ȭ�� Shader Update �ڵ�. Initialize, UpdateVariables.
	//////////////////////////////////////////////////////////////////////////
	//BasicTex.cso
	template<>
	void ShaderUpdater::Initialize<BasicTexLegacy>(BaseRenderable* baseRenderable)
	{
		//�ʼ���, ���߿� Shader�� �޸𸮸� �����ϴµ� �ʼ����̴�.
		this->_assignedObject = baseRenderable;

		MemoryVariant tWorldMatDX = new DirectX::XMMATRIX;
		MemoryVariant tWorldInvTransposeDX = new DirectX::XMMATRIX;
		MemoryVariant tWorldViewProjDX = new DirectX::XMMATRIX;
		MemoryVariant tDiffuseMap = baseRenderable->GetTextureStorage()->GetDiffuseTexture(0)->GetSRV();
		//��ġ�� ������ �ʱ⿡, �����ϴ�. ��ġ�� �����ϴ� �뵵.

		//���߿� get<T>�� ���� �ε�������, ���� �Բ� Updater�� Vector�� �����Ѵ�.
		//(���ø� Ư¡ ������ ���� enum���� ġȯ�� ����ϰ�����, �ϴ��� ���� �������ش�.)
		_varVariantList.push_back(std::make_pair(tWorldMatDX, tWorldMatDX.index()));
		_varVariantList.push_back(std::make_pair(tWorldInvTransposeDX, tWorldInvTransposeDX.index()));
		_varVariantList.push_back(std::make_pair(tWorldViewProjDX, tWorldViewProjDX.index()));
		_varVariantList.push_back(std::make_pair(tDiffuseMap, tDiffuseMap.index()));
	}

	template<>
	void ShaderUpdater::UpdateVariables<BasicTexLegacy>(const DirectX::XMFLOAT4X4& worldTM, double interval)
	{
		//�������, Initialize�� �ڵ带 ������� Shader ���� ������Ʈ ��Ų��.
		CameraMatrices* _cameraMatrix = _assignedObject->GetCameraMatrices();
		ShaderMaterial* _material = _assignedObject->GetShaderMaterial();

		DirectX::XMMATRIX tWorldMatDX = DirectX::XMLoadFloat4x4(&worldTM);
		DirectX::XMMATRIX tWorldInvTransposeDX = MathHelper::InverseTranspose(tWorldMatDX);
		DirectX::XMMATRIX tWorldViewProjDX = (tWorldMatDX)*_cameraMatrix->View() * _cameraMatrix->Proj();
		ID3D11ShaderResourceView* tDiffuseSRV = _assignedObject->GetTextureStorage()->GetDiffuseTexture(0)->GetSRV();

		*(std::get<XMMATRIX_POINTER>(_varVariantList.at(0).first)) = tWorldMatDX;
		*(std::get<XMMATRIX_POINTER>(_varVariantList.at(1).first)) = tWorldInvTransposeDX;
		*(std::get<XMMATRIX_POINTER>(_varVariantList.at(2).first)) = tWorldViewProjDX;
		std::get<SRV_POINTER>(_varVariantList.at(3).first) = tDiffuseSRV;

		_material->GetShader()->UpdateSingleVariable("gWorld", std::get<XMMATRIX_POINTER>(_varVariantList.at(0).first));
		_material->GetShader()->UpdateSingleVariable("gWorldInvTranspose", std::get<XMMATRIX_POINTER>(_varVariantList.at(1).first));
		_material->GetShader()->UpdateSingleVariable("gWorldViewProj", std::get<XMMATRIX_POINTER>(_varVariantList.at(2).first));
		_material->GetShader()->UpdateSingleVariable("gDiffuseMap", std::get<SRV_POINTER>(_varVariantList.at(3).first));

		//�׻� ���������δ� UpdateShader()�� ȣ���ؾ� �ݿ��ȴ�.
		_material->GetShader()->UpdateShader();
	}
	//////////////////////////////////////////////////////////////////////////
	//SkinnedMeshTex.cso
	template<>
	void ShaderUpdater::Initialize<SkinnedMeshTexLegacy>(BaseRenderable* baseRenderable)
	{
		//�ʼ���, ���߿� Shader�� �޸𸮸� �����ϴµ� �ʼ����̴�.
		this->_assignedObject = baseRenderable;

		MemoryVariant tWorldMatDX = new DirectX::XMMATRIX;
		MemoryVariant tWorldInvTransposeDX = new DirectX::XMMATRIX;
		MemoryVariant tWorldViewProjDX = new DirectX::XMMATRIX;
		MemoryVariant tTexTransformDX = new DirectX::XMMATRIX;
		MemoryVariant tDiffuseMap = baseRenderable->GetTextureStorage()->GetDiffuseTexture(0)->GetSRV();

		//ByteBuffer* tEmptyByteBuffer = nullptr;
		MemoryVariant tBoneMat = new ByteBuffer;
		//��ġ�� ������ �ʱ⿡, �����ϴ�. ��ġ�� �����ϴ� �뵵.

		//���߿� get<T>�� ���� �ε�������, ���� �Բ� Updater�� Vector�� �����Ѵ�.
		//(���ø� Ư¡ ������ ���� enum���� ġȯ�� ����ϰ�����, �ϴ��� ���� �������ش�.)
		_varVariantList.push_back(std::make_pair(tWorldMatDX, tWorldMatDX.index())); //0
		_varVariantList.push_back(std::make_pair(tWorldInvTransposeDX, tWorldInvTransposeDX.index())); //1
		_varVariantList.push_back(std::make_pair(tWorldViewProjDX, tWorldViewProjDX.index())); //2
		_varVariantList.push_back(std::make_pair(tTexTransformDX, tTexTransformDX.index())); //3
		_varVariantList.push_back(std::make_pair(tDiffuseMap, tDiffuseMap.index())); //4
		_varVariantList.push_back(std::make_pair(tBoneMat, tBoneMat.index())); //5
	}

	template<>
	void ShaderUpdater::UpdateVariables<SkinnedMeshTexLegacy>(const DirectX::XMFLOAT4X4& worldTM, double interval)
	{
		//�������, Initialize�� �ڵ带 ������� Shader ���� ������Ʈ ��Ų��.
		CameraMatrices* _cameraMatrix = _assignedObject->GetCameraMatrices();
		ShaderMaterial* _material = _assignedObject->GetShaderMaterial();
		BaseFormation* _formation = _assignedObject->GetBaseFormation();
		SkinnedMeshObject* _obj = static_cast<SkinnedMeshObject*>(_assignedObject);

		DirectX::XMMATRIX tWorldMatDX = DirectX::XMLoadFloat4x4(&worldTM);
		DirectX::XMMATRIX tWorldInvTransposeDX = MathHelper::InverseTranspose(tWorldMatDX);
		DirectX::XMMATRIX tWorldViewProjDX = (tWorldMatDX)*_cameraMatrix->View() * _cameraMatrix->Proj();
		DirectX::XMMATRIX tTexTransformDX = DirectX::XMMatrixIdentity();
		ID3D11ShaderResourceView* tDiffuseSRV = _assignedObject->GetTextureStorage()->GetDiffuseTexture(0)->GetSRV();

		SkinnedMeshFormation* tForm = static_cast<SkinnedMeshFormation*>(_formation);
		ByteBuffer* tBoneMat = nullptr;

		tBoneMat = _obj->m_AnimHandler->_nowPlayingAnim->UpdateAnimationInfo(interval).first;

		*(std::get<XMMATRIX_POINTER>(_varVariantList.at(0).first)) = tWorldMatDX;
		*(std::get<XMMATRIX_POINTER>(_varVariantList.at(1).first)) = tWorldInvTransposeDX;
		*(std::get<XMMATRIX_POINTER>(_varVariantList.at(2).first)) = tWorldViewProjDX;
		*(std::get<XMMATRIX_POINTER>(_varVariantList.at(3).first)) = tTexTransformDX;
		std::get<SRV_POINTER>(_varVariantList.at(4).first) = tDiffuseSRV;
		std::get<BYTEBUFFER_POINTER>(_varVariantList.at(5).first) = tBoneMat;

		_material->GetShader()->UpdateSingleVariable("gWorld", std::get<XMMATRIX_POINTER>(_varVariantList.at(0).first));
		_material->GetShader()->UpdateSingleVariable("gWorldInvTranspose", std::get<XMMATRIX_POINTER>(_varVariantList.at(1).first));
		_material->GetShader()->UpdateSingleVariable("gWorldViewProj", std::get<XMMATRIX_POINTER>(_varVariantList.at(2).first));
		_material->GetShader()->UpdateSingleVariable("gTexTransform", std::get<XMMATRIX_POINTER>(_varVariantList.at(3).first));
		_material->GetShader()->UpdateSingleVariable("gDiffuseMap", std::get<SRV_POINTER>(_varVariantList.at(4).first));
		_material->GetShader()->UpdateSingleVariable("bones", std::get<BYTEBUFFER_POINTER>(_varVariantList.at(5).first));

		//�׻� ���������δ� UpdateShader()�� ȣ���ؾ� �ݿ��ȴ�.
		_material->GetShader()->UpdateShader();
	}
	//////////////////////////////////////////////////////////////////////////
	//PosNormalTex.cso
	template<>
	void ShaderUpdater::Initialize<PosNormalTexLegacy>(BaseRenderable* baseRenderable)
	{
		//�ʼ���, ���߿� Shader�� �޸𸮸� �����ϴµ� �ʼ����̴�.
		this->_assignedObject = baseRenderable;

		MemoryVariant tEyePosDX = new float[4];						//gEyePosW												//0
		MemoryVariant tWorldMatDX = new DirectX::XMMATRIX;			//gWorld												//1
		MemoryVariant tWorldInvTransposeDX = new DirectX::XMMATRIX; //gWorldInvTranspose									//2
		MemoryVariant tWorldViewProjDX = new DirectX::XMMATRIX;		//gWorldViewProj										//3
		MemoryVariant tTexTransformDX = new DirectX::XMMATRIX;		//gTexTransform											//4
		MemoryVariant tLegacyMaterial = new LegacyMaterialData;		//gLegacyMaterial										//5
		MemoryVariant tDiffuseMap = baseRenderable->GetTextureStorage()->GetDiffuseTexture(0)->GetSRV(); // gDiffuseMap		//6
		MemoryVariant tNormalMap = baseRenderable->GetTextureStorage()->GetNormalTexture(0)->GetSRV();   // gNormalMap		//7

		//����Ʈ�� ������ �ǰ�, CPU���� �����ؼ� �Ѱ��ֱ�� �����Ƿ�, ���� �����ϰ� �Ѿ�� �Ѵ�.
		//CustomFXInstances���� �������� �� ������ �����ȴ�. (w/ POSNORMALTEX_MAXIMUM_LIGHT_COUNT) 
		MemoryVariant tDirectionalLightList = new DirectionalLightData[10]; // gLightDataList
		MemoryVariant tDirectionalLightCount = new int; //gDirLightListCount

		//���߿� get<T>�� ���� �ε�������, ���� �Բ� Updater�� Vector�� �����Ѵ�.
		//(���ø� Ư¡ ������ ���� enum���� ġȯ�� ����ϰ�����, �ϴ��� ���� �������ش�.)

		_varVariantList.push_back(std::make_pair(tEyePosDX, tEyePosDX.index()));							//0
		_varVariantList.push_back(std::make_pair(tWorldMatDX, tWorldMatDX.index()));						//1
		_varVariantList.push_back(std::make_pair(tWorldInvTransposeDX, tWorldInvTransposeDX.index()));		//2
		_varVariantList.push_back(std::make_pair(tWorldViewProjDX, tWorldViewProjDX.index()));				//3
		_varVariantList.push_back(std::make_pair(tTexTransformDX, tTexTransformDX.index()));				//4
		_varVariantList.push_back(std::make_pair(tLegacyMaterial, tLegacyMaterial.index()));				//5
		_varVariantList.push_back(std::make_pair(tDiffuseMap, tDiffuseMap.index()));						//6
		_varVariantList.push_back(std::make_pair(tNormalMap, tNormalMap.index()));							//7
		_varVariantList.push_back(std::make_pair(tDirectionalLightList, tDirectionalLightList.index()));	//8
		_varVariantList.push_back(std::make_pair(tDirectionalLightCount, tDirectionalLightCount.index()));	//9

	}

	template<>
	void ShaderUpdater::UpdateVariables<PosNormalTexLegacy>(const DirectX::XMFLOAT4X4& worldTM, double interval)
	{
		//�������, Initialize�� �ڵ带 ������� Shader ���� ������Ʈ ��Ų��.
		CameraMatrices* _cameraMatrix = _assignedObject->GetCameraMatrices();
		ShaderMaterial* _material = _assignedObject->GetShaderMaterial();
		BaseFormation* _formation = _assignedObject->GetBaseFormation();
		SceneConstantsUpdater* _sceneConstantsUpdater = SceneConstantsUpdater::GetInstance();
		RenderConstantData* _rcData = _sceneConstantsUpdater->GetRenderConstantData();

		DirectX::XMFLOAT3 tEyePosDX = _cameraMatrix->mPosition; //0
		DirectX::XMMATRIX tWorldMatDX = DirectX::XMLoadFloat4x4(&worldTM); //1
		DirectX::XMMATRIX tWorldInvTransposeDX = MathHelper::InverseTranspose(tWorldMatDX); //2
		DirectX::XMMATRIX tWorldViewProjDX = (tWorldMatDX)*_cameraMatrix->View() * _cameraMatrix->Proj(); //3
		DirectX::XMMATRIX tTexTransformDX = DirectX::XMMatrixIdentity(); //4
		LegacyMaterialData tLegacyMaterial = _assignedObject->GetLegacyMaterialData(); //5
		ID3D11ShaderResourceView* tDiffuseSRV = _assignedObject->GetTextureStorage()->GetDiffuseTexture(0)->GetSRV(); //6
		ID3D11ShaderResourceView* tNormalSRV = _assignedObject->GetTextureStorage()->GetNormalTexture(0)->GetSRV();	//7

		//XMFLOAT ������ Ǯ� ����.
		std::get<FLOAT_POINTER>(_varVariantList.at(0).first)[0] = tEyePosDX.x;
		std::get<FLOAT_POINTER>(_varVariantList.at(0).first)[1] = tEyePosDX.y;
		std::get<FLOAT_POINTER>(_varVariantList.at(0).first)[2] = tEyePosDX.z;

		*(std::get<XMMATRIX_POINTER>(_varVariantList.at(1).first)) = tWorldMatDX;
		*(std::get<XMMATRIX_POINTER>(_varVariantList.at(2).first)) = tWorldInvTransposeDX;
		*(std::get<XMMATRIX_POINTER>(_varVariantList.at(3).first)) = tWorldViewProjDX;
		*(std::get<XMMATRIX_POINTER>(_varVariantList.at(4).first)) = tTexTransformDX;
		*(std::get<CUSTOM_LEGACY_MATERIAL_POINTER>(_varVariantList.at(5).first)) = tLegacyMaterial;
		std::get<SRV_POINTER>(_varVariantList.at(6).first) = tDiffuseSRV;
		std::get<SRV_POINTER>(_varVariantList.at(7).first) = tNormalSRV;

		//Light �ø������ �������� �׷��� �ѹ��� �� �޾ƿ��� + GPU ���ޱ���.
		//���� �߰��� Light. (POSNORMALTEX_MAXIMUM_LIGHT_COUNT �� �Ѿ��)
		int tCount = min(10, _sceneConstantsUpdater->GetDirLightCount());
		for (int i = 0; i < tCount; i++)
		{
			std::get<CUSTOM_LEGACY_DIRECTIONAL_LIGHT_POINTER>(_varVariantList.at(8).first)[i] =				//8
				_rcData->dirLightList.at(_sceneConstantsUpdater->GetDirLightBrightIndex(i));
		}
		*(std::get<INT_POINTER>(_varVariantList.at(9).first)) = _sceneConstantsUpdater->GetDirLightCount();			//9

		///���� ������Ʈ�ϴ� �κ�.
		_material->GetShader()->UpdateSingleVariable("gEyePosW", std::get<FLOAT_POINTER>(_varVariantList.at(0).first));
		_material->GetShader()->UpdateSingleVariable("gWorld", std::get<XMMATRIX_POINTER>(_varVariantList.at(1).first));
		_material->GetShader()->UpdateSingleVariable("gWorldInvTranspose", std::get<XMMATRIX_POINTER>(_varVariantList.at(2).first));
		_material->GetShader()->UpdateSingleVariable("gWorldViewProj", std::get<XMMATRIX_POINTER>(_varVariantList.at(3).first));
		_material->GetShader()->UpdateSingleVariable("gTexTransform", std::get<XMMATRIX_POINTER>(_varVariantList.at(4).first));
		_material->GetShader()->UpdateSingleVariable("gLegacyMaterial", std::get<CUSTOM_LEGACY_MATERIAL_POINTER>(_varVariantList.at(5).first));
		_material->GetShader()->UpdateSingleVariable("gDiffuseMap", std::get<SRV_POINTER>(_varVariantList.at(6).first));
		_material->GetShader()->UpdateSingleVariable("gNormalMap", std::get<SRV_POINTER>(_varVariantList.at(7).first));
		_material->GetShader()->UpdateSingleVariable("gLightDataList", std::get<CUSTOM_LEGACY_DIRECTIONAL_LIGHT_POINTER>(_varVariantList.at(8).first));
		_material->GetShader()->UpdateSingleVariable("gDirLightListCount", std::get<INT_POINTER>(_varVariantList.at(9).first));
		//�׻� ���������δ� UpdateShader()�� ȣ���ؾ� �ݿ��ȴ�.
		_material->GetShader()->UpdateShader();
	}

	///Deferred.

	///�߿��� ��� : ���� SetShaderUpdateFunction�� Basic �������� �ش��� �Ǵ� ���̴�. 
	///�׷��Ƿ�, Lighting(Deferred�� ����)�� ���� Engine ���� ������ ȣ��Ǿ��!
	///�̴� �� �̻� "������Ʈ" ������ �ƴ϶� "���̴�" ������ ó���ؾ� �ϴ� ���̶�, ��ó�� �����Ǵ� ���̴�.
	//////////////////////////////////////////////////////////////////////////
	//DeferredPhongShader.cso
	template<>
	void ShaderUpdater::Initialize<DeferredPhongLegacy>(BaseRenderable* baseRenderable)
	{
		//�ʼ���, ���߿� Shader�� �޸𸮸� �����ϴµ� �ʼ����̴�.
		this->_assignedObject = baseRenderable;

		LowLevelDX11* tLowDX = LowLevelDX11::GetInstance();
		////�ܼ��� SRV�� ��ó�� �ȵ��ư� ��. �ܼ��� ���� �����ϱ� ����. (GetTextureStorage�� ��� ������Ʈ ����! ���� ���� ���� SRV �����ؾ�)
		MemoryVariant tDiffuse_GBufferSRV = tLowDX->_deferredPhongShaderQuad->_phongDiffuseGBuffer._shaderResourceView;		 									//0
		MemoryVariant tNormal_GBufferSRV = tLowDX->_deferredPhongShaderQuad->_phongNormalGBuffer._shaderResourceView;		 									//0
		MemoryVariant tPosition_GBufferSRV = tLowDX->_deferredPhongShaderQuad->_phongPositionGBuffer._shaderResourceView;										//0
		MemoryVariant tDepth_GBufferSRV = tLowDX->_deferredPhongShaderQuad->_phongDepthGBuffer._shaderResourceView;											//0

		MemoryVariant tEyePosDX = new float[4];						//gEyePosW												
		MemoryVariant tWorldMatDX = new DirectX::XMMATRIX;			//gWorld												
		MemoryVariant tWorldInvTransposeDX = new DirectX::XMMATRIX; //gWorldInvTranspose									
		MemoryVariant tWorldViewProjDX = new DirectX::XMMATRIX;		//gWorldViewProj										
		MemoryVariant tTexTransformDX = new DirectX::XMMATRIX;		//gTexTransform											
		MemoryVariant tLegacyMaterial = new LegacyMaterialData;		//gLegacyMaterial										
		MemoryVariant tDiffuseMap = baseRenderable->GetTextureStorage()->GetDiffuseTexture(0)->GetSRV(); // gDiffuseMap		
		MemoryVariant tNormalMap = baseRenderable->GetTextureStorage()->GetNormalTexture(0)->GetSRV();   // gNormalMap		

		//DEFERRED_PHONG_MAXIMUM_DIRECTIONAL_COUNT�� ������ �����Ǿ�� �Ѵ�.
		MemoryVariant tDirectionalLightList = new DirectionalLightData[10]; // gLightDataList
		MemoryVariant tDirectionalLightCount = new int; //gDirLightListCount

		////���߿� get<T>�� ���� �ε�������, ���� �Բ� Updater�� Vector�� �����Ѵ�.
		////(���ø� Ư¡ ������ ���� enum���� ġȯ�� ����ϰ�����, �ϴ��� ���� �������ش�.)
		//
		_varVariantList.push_back(std::make_pair(tDiffuse_GBufferSRV, tDiffuse_GBufferSRV.index()));		//0
		_varVariantList.push_back(std::make_pair(tNormal_GBufferSRV, tNormal_GBufferSRV.index()));			//1
		_varVariantList.push_back(std::make_pair(tPosition_GBufferSRV, tPosition_GBufferSRV.index()));		//2
		_varVariantList.push_back(std::make_pair(tDepth_GBufferSRV, tDepth_GBufferSRV.index()));			//3
		_varVariantList.push_back(std::make_pair(tEyePosDX, tEyePosDX.index()));							//4
		_varVariantList.push_back(std::make_pair(tWorldMatDX, tWorldMatDX.index()));						//5
		_varVariantList.push_back(std::make_pair(tWorldInvTransposeDX, tWorldInvTransposeDX.index()));		//6
		_varVariantList.push_back(std::make_pair(tWorldViewProjDX, tWorldViewProjDX.index()));				//7
		_varVariantList.push_back(std::make_pair(tTexTransformDX, tTexTransformDX.index()));				//8
		_varVariantList.push_back(std::make_pair(tLegacyMaterial, tLegacyMaterial.index()));				//9
		_varVariantList.push_back(std::make_pair(tDiffuseMap, tDiffuseMap.index()));						//10
		_varVariantList.push_back(std::make_pair(tNormalMap, tNormalMap.index()));							//11
		_varVariantList.push_back(std::make_pair(tDirectionalLightList, tDirectionalLightList.index()));	//12
		_varVariantList.push_back(std::make_pair(tDirectionalLightCount, tDirectionalLightCount.index()));	//13

	}

	template<>
	void ShaderUpdater::UpdateVariables<DeferredPhongLegacy>(const DirectX::XMFLOAT4X4& worldTM, double interval)
	{
		///�������! ��� �׳� Basic�� �ߵ����Ѿ� ��. (Tech) Not Lighting!)

		//�ϴ� ���� �޾Ƴ���.
		CameraMatrices* _cameraMatrix = _assignedObject->GetCameraMatrices();
		ShaderMaterial* _material = _assignedObject->GetShaderMaterial();
		BaseFormation* _formation = _assignedObject->GetBaseFormation();
		SceneConstantsUpdater* _sceneConstantsUpdater = SceneConstantsUpdater::GetInstance();
		RenderConstantData* _rcData = _sceneConstantsUpdater->GetRenderConstantData();

		//�������, Initialize�� �ڵ带 ������� Shader ���� ������Ʈ ��Ų��.
		LowLevelDX11* tLowDX = LowLevelDX11::GetInstance();

		//�̹� ���̳ο��� ��SRV ������ �ϴ�, �̸� Pixel �ϳ��� �޴°ɷ� �ٿ�����!
		ID3D11ShaderResourceView* tDiffuseGBuffer = ResourceManager::GetInstance().GetDefaultPixelTexture()->GetSRV();  //0
		ID3D11ShaderResourceView* tNormalGBuffer = ResourceManager::GetInstance().GetDefaultPixelTexture()->GetSRV();	//1
		ID3D11ShaderResourceView* tPositionGBuffer = ResourceManager::GetInstance().GetDefaultPixelTexture()->GetSRV(); //2
		ID3D11ShaderResourceView* tDepthGBuffer = ResourceManager::GetInstance().GetDefaultPixelTexture()->GetSRV();	//3
		//ID3D11ShaderResourceView* tDepthGBuffer = ;	//3



		DirectX::XMFLOAT3 tEyePosDX = _cameraMatrix->mPosition;															//4
		DirectX::XMMATRIX tWorldMatDX = DirectX::XMLoadFloat4x4(&worldTM);												//5
		DirectX::XMMATRIX tWorldInvTransposeDX = MathHelper::InverseTranspose(tWorldMatDX);								//6
		DirectX::XMMATRIX tWorldViewProjDX = (tWorldMatDX)*_cameraMatrix->View() * _cameraMatrix->Proj();				//7
		DirectX::XMMATRIX tTexTransformDX = DirectX::XMMatrixIdentity();												//8
		LegacyMaterialData tLegacyMaterial = _assignedObject->GetLegacyMaterialData();									//9
		ID3D11ShaderResourceView* tDiffuseSRV = _assignedObject->GetTextureStorage()->GetDiffuseTexture(0)->GetSRV();	//10
		ID3D11ShaderResourceView* tNormalSRV = _assignedObject->GetTextureStorage()->GetNormalTexture(0)->GetSRV();		//11
		//12, 13�� �ڿ�!

		std::get<SRV_POINTER>(_varVariantList.at(0).first) = tDiffuseGBuffer;
		std::get<SRV_POINTER>(_varVariantList.at(1).first) = tNormalGBuffer;
		std::get<SRV_POINTER>(_varVariantList.at(2).first) = tPositionGBuffer;
		std::get<SRV_POINTER>(_varVariantList.at(3).first) = tDepthGBuffer;

		//XMFLOAT ������ Ǯ� ����.
		std::get<FLOAT_POINTER>(_varVariantList.at(4).first)[0] = tEyePosDX.x;
		std::get<FLOAT_POINTER>(_varVariantList.at(4).first)[1] = tEyePosDX.y;
		std::get<FLOAT_POINTER>(_varVariantList.at(4).first)[2] = tEyePosDX.z;

		*(std::get<XMMATRIX_POINTER>(_varVariantList.at(5).first)) = tWorldMatDX; //gModel�̶� World�� �����ϰ� ����!
		*(std::get<XMMATRIX_POINTER>(_varVariantList.at(6).first)) = tWorldInvTransposeDX;
		*(std::get<XMMATRIX_POINTER>(_varVariantList.at(7).first)) = tWorldViewProjDX;
		*(std::get<XMMATRIX_POINTER>(_varVariantList.at(8).first)) = tTexTransformDX;
		*(std::get<CUSTOM_LEGACY_MATERIAL_POINTER>(_varVariantList.at(9).first)) = tLegacyMaterial;
		std::get<SRV_POINTER>(_varVariantList.at(10).first) = tDiffuseSRV;
		std::get<SRV_POINTER>(_varVariantList.at(11).first) = tNormalSRV;

		///�ѹ��� ����.
		//Light �ø������ �������� �׷��� �ѹ��� �� �޾ƿ��� + GPU ���ޱ���.
		//���� �߰��� Light. (POSNORMALTEX_MAXIMUM_LIGHT_COUNT �� �Ѿ��)
		int tCount = min(10, _sceneConstantsUpdater->GetDirLightCount());
		for (int i = 0; i < tCount; i++)
		{
			std::get<CUSTOM_LEGACY_DIRECTIONAL_LIGHT_POINTER>(_varVariantList.at(12).first)[i] =						//12
				_rcData->dirLightList.at(_sceneConstantsUpdater->GetDirLightBrightIndex(i));
		}
		*(std::get<INT_POINTER>(_varVariantList.at(13).first)) = _sceneConstantsUpdater->GetDirLightCount();			//13

		///���� ���� ���̴��� ����.
		_material->GetShader()->UpdateSingleVariable("gTexDiffuseSRV_GBuffer", std::get<SRV_POINTER>(_varVariantList.at(0).first));
		_material->GetShader()->UpdateSingleVariable("gTexNormalSRV_GBuffer", std::get<SRV_POINTER>(_varVariantList.at(1).first));
		_material->GetShader()->UpdateSingleVariable("gTexPositionSRV_GBuffer", std::get<SRV_POINTER>(_varVariantList.at(2).first));
		_material->GetShader()->UpdateSingleVariable("gTexDepthSRV_GBuffer", std::get<SRV_POINTER>(_varVariantList.at(3).first));

		_material->GetShader()->UpdateSingleVariable("gEyePosW", std::get<FLOAT_POINTER>(_varVariantList.at(4).first));
		_material->GetShader()->UpdateSingleVariable("gWorld", std::get<XMMATRIX_POINTER>(_varVariantList.at(5).first));
		_material->GetShader()->UpdateSingleVariable("gWorldInvTranspose", std::get<XMMATRIX_POINTER>(_varVariantList.at(6).first));
		_material->GetShader()->UpdateSingleVariable("gWorldViewProj", std::get<XMMATRIX_POINTER>(_varVariantList.at(7).first));
		_material->GetShader()->UpdateSingleVariable("gTexTransform", std::get<XMMATRIX_POINTER>(_varVariantList.at(8).first));
		_material->GetShader()->UpdateSingleVariable("gLegacyMaterial", std::get<CUSTOM_LEGACY_MATERIAL_POINTER>(_varVariantList.at(9).first));
		_material->GetShader()->UpdateSingleVariable("gDiffuseMap", std::get<SRV_POINTER>(_varVariantList.at(10).first));
		_material->GetShader()->UpdateSingleVariable("gNormalMap", std::get<SRV_POINTER>(_varVariantList.at(11).first));
		_material->GetShader()->UpdateSingleVariable("gLightDataList", std::get<CUSTOM_LEGACY_DIRECTIONAL_LIGHT_POINTER>(_varVariantList.at(12).first));
		_material->GetShader()->UpdateSingleVariable("gDirLightListCount", std::get<INT_POINTER>(_varVariantList.at(13).first));

		//�׻� ���������δ� UpdateShader()�� ȣ���ؾ� �ݿ��ȴ�.
		_material->GetShader()->UpdateShader();
	}

	//////////////////////////////////////////////////////////////////////////
	//BasicPBR.cso
	template<>
	void ShaderUpdater::Initialize<DeferredPBRType>(BaseRenderable* baseRenderable)
	{
		//�ʼ���, ���߿� Shader�� �޸𸮸� �����ϴµ� �ʼ����̴�.
		this->_assignedObject = baseRenderable;

		LowLevelDX11* tLowDX = LowLevelDX11::GetInstance();
		////�ܼ��� SRV�� ��ó�� �ȵ��ư� ��. �ܼ��� ���� �����ϱ� ����. (GetTextureStorage�� ��� ������Ʈ ����! ���� ���� ���� SRV �����ؾ�)
		MemoryVariant tAlbedo_GBufferSRV = tLowDX->_deferredPBRShaderQuad->_pbrAlbedoGBuffer._shaderResourceView;		 									//0
		MemoryVariant tNormal_GBufferSRV = tLowDX->_deferredPBRShaderQuad->_pbrNormalGBuffer._shaderResourceView;		 									//0
		MemoryVariant tPosition_GBufferSRV = tLowDX->_deferredPBRShaderQuad->_pbrPositionGBuffer._shaderResourceView;										//0
		MemoryVariant tArm_GBufferSRV = tLowDX->_deferredPBRShaderQuad->_pbrArmGBuffer._shaderResourceView;											//0
		MemoryVariant tDepth_GBufferSRV = tLowDX->_deferredPBRShaderQuad->_pbrDepthGBuffer._shaderResourceView;											//0

		MemoryVariant tEyePosDX = new float[4];						//gEyePosW		

		//DEFERRED_PHONG_MAXIMUM_DIRECTIONAL_COUNT�� ������ �����Ǿ�� �Ѵ�.
		MemoryVariant tDirectionalLightList = new DirectionalLightData[10]; // gLightDataList
		MemoryVariant tDirectionalLightCount = new int; //gDirLightListCount

		MemoryVariant tWorldMatDX = new DirectX::XMMATRIX;			//gWorld												
		MemoryVariant tWorldInvTransposeDX = new DirectX::XMMATRIX; //gWorldInvTranspose									
		MemoryVariant tTexTransformDX = new DirectX::XMMATRIX;		//gTexTransform											
		MemoryVariant tWorldViewProjDX = new DirectX::XMMATRIX;		//gWorldViewProj	

		MemoryVariant tPBRMaterial = new PBRMaterialData;		//gLegacyMaterial										
		MemoryVariant tAlbedoMap = baseRenderable->GetTextureStorage()->GetDiffuseTexture(0)->GetSRV(); // gDiffuseMap		
		MemoryVariant tArmMap = baseRenderable->GetTextureStorage()->GetArmTexture(0)->GetSRV();   // gNormalMap		
		MemoryVariant tNormalMap = baseRenderable->GetTextureStorage()->GetNormalTexture(0)->GetSRV();   // gNormalMap		

		////���߿� get<T>�� ���� �ε�������, ���� �Բ� Updater�� Vector�� �����Ѵ�.
		////(���ø� Ư¡ ������ ���� enum���� ġȯ�� ����ϰ�����, �ϴ��� ���� �������ش�.)
		////
		_varVariantList.push_back(std::make_pair(tAlbedo_GBufferSRV, tAlbedo_GBufferSRV.index()));		   //0
		_varVariantList.push_back(std::make_pair(tNormal_GBufferSRV, tNormal_GBufferSRV.index()));		   //1
		_varVariantList.push_back(std::make_pair(tPosition_GBufferSRV, tPosition_GBufferSRV.index()));	   //2
		_varVariantList.push_back(std::make_pair(tArm_GBufferSRV, tArm_GBufferSRV.index()));			   //3
		_varVariantList.push_back(std::make_pair(tDepth_GBufferSRV, tDepth_GBufferSRV.index()));		   //4
		_varVariantList.push_back(std::make_pair(tEyePosDX, tEyePosDX.index()));						   //5
		_varVariantList.push_back(std::make_pair(tDirectionalLightList, tDirectionalLightList.index()));   //6
		_varVariantList.push_back(std::make_pair(tDirectionalLightCount, tDirectionalLightCount.index())); //7
		_varVariantList.push_back(std::make_pair(tWorldMatDX, tWorldMatDX.index()));					   //8
		_varVariantList.push_back(std::make_pair(tWorldInvTransposeDX, tWorldInvTransposeDX.index()));	   //9
		_varVariantList.push_back(std::make_pair(tTexTransformDX, tTexTransformDX.index()));			   //10
		_varVariantList.push_back(std::make_pair(tWorldViewProjDX, tWorldViewProjDX.index()));			   //11
		_varVariantList.push_back(std::make_pair(tPBRMaterial, tPBRMaterial.index()));					   //12
		_varVariantList.push_back(std::make_pair(tAlbedoMap, tAlbedoMap.index()));						   //13
		_varVariantList.push_back(std::make_pair(tArmMap, tArmMap.index()));							   //14
		_varVariantList.push_back(std::make_pair(tNormalMap, tNormalMap.index()));						   //15
	}

	template<>
	void ShaderUpdater::UpdateVariables<DeferredPBRType>(const DirectX::XMFLOAT4X4& worldTM, double interval)
	{
		///�������! ��� �׳� Basic�� �ߵ����Ѿ� ��. (Tech) Not Lighting!)

		//�ϴ� ���� �޾Ƴ���.
		CameraMatrices* _cameraMatrix = _assignedObject->GetCameraMatrices();
		ShaderMaterial* _material = _assignedObject->GetShaderMaterial();
		BaseFormation* _formation = _assignedObject->GetBaseFormation();
		SceneConstantsUpdater* _sceneConstantsUpdater = SceneConstantsUpdater::GetInstance();
		RenderConstantData* _rcData = _sceneConstantsUpdater->GetRenderConstantData();

		//�������, Initialize�� �ڵ带 ������� Shader ���� ������Ʈ ��Ų��.
		LowLevelDX11* tLowDX = LowLevelDX11::GetInstance();

		//�̹� ���̳ο��� ��SRV ������ �ϴ�, �̸� Pixel �ϳ��� �޴°ɷ� �ٿ�����!
		ID3D11ShaderResourceView* tAlbedoGBuffer = ResourceManager::GetInstance().GetDefaultPixelTexture()->GetSRV();
		ID3D11ShaderResourceView* tNormalGBuffer = ResourceManager::GetInstance().GetDefaultPixelTexture()->GetSRV();
		ID3D11ShaderResourceView* tPositionGBuffer = ResourceManager::GetInstance().GetDefaultPixelTexture()->GetSRV();
		ID3D11ShaderResourceView* tArmGBuffer = ResourceManager::GetInstance().GetDefaultPixelTexture()->GetSRV();
		ID3D11ShaderResourceView* tDepthGBuffer = ResourceManager::GetInstance().GetDefaultPixelTexture()->GetSRV();

		DirectX::XMFLOAT3 tEyePosDX = _cameraMatrix->mPosition; 
		DirectX::XMMATRIX tWorldMatDX = DirectX::XMLoadFloat4x4(&worldTM);
		DirectX::XMMATRIX tWorldInvTransposeDX = MathHelper::InverseTranspose(tWorldMatDX);
		DirectX::XMMATRIX tWorldViewProjDX = (tWorldMatDX)*_cameraMatrix->View() * _cameraMatrix->Proj();
		DirectX::XMMATRIX tTexTransformDX = DirectX::XMMatrixIdentity();

		PBRMaterialData tPBRMaterial = _assignedObject->GetPBRMaterialData();
		ID3D11ShaderResourceView* tAlbedoSRV = _assignedObject->GetTextureStorage()->GetDiffuseTexture(0)->GetSRV();
		ID3D11ShaderResourceView* tArmSRV = _assignedObject->GetTextureStorage()->GetArmTexture(0)->GetSRV();
		ID3D11ShaderResourceView* tNormalSRV = _assignedObject->GetTextureStorage()->GetNormalTexture(0)->GetSRV();
		
		std::get<SRV_POINTER>(_varVariantList.at(0).first) = tAlbedoGBuffer;
		std::get<SRV_POINTER>(_varVariantList.at(1).first) = tNormalGBuffer;
		std::get<SRV_POINTER>(_varVariantList.at(2).first) = tPositionGBuffer;
		std::get<SRV_POINTER>(_varVariantList.at(3).first) = tArmGBuffer;
		std::get<SRV_POINTER>(_varVariantList.at(4).first) = tDepthGBuffer;

		std::get<FLOAT_POINTER>(_varVariantList.at(5).first)[0] = tEyePosDX.x;
		std::get<FLOAT_POINTER>(_varVariantList.at(5).first)[1] = tEyePosDX.y;
		std::get<FLOAT_POINTER>(_varVariantList.at(5).first)[2] = tEyePosDX.z;

		///�ѹ��� ����.
		//Light �ø������ �������� �׷��� �ѹ��� �� �޾ƿ��� + GPU ���ޱ���.
		//���� �߰��� Light. (DEFERRED_PBR_MAXIMUM_DIRECTIONAL_COUNT �� �Ѿ��)
		int tCount = min(3, _sceneConstantsUpdater->GetDirLightCount());
		for (int i = 0; i < tCount; i++)
		{
			std::get<CUSTOM_LEGACY_DIRECTIONAL_LIGHT_POINTER>(_varVariantList.at(6).first)[i] =
				_rcData->dirLightList.at(_sceneConstantsUpdater->GetDirLightBrightIndex(i));
		}
		*(std::get<INT_POINTER>(_varVariantList.at(7).first)) = _sceneConstantsUpdater->GetDirLightCount();

		*(std::get<XMMATRIX_POINTER>(_varVariantList.at(8).first)) = tWorldMatDX; //gModel�̶� World�� �����ϰ� ����!
		*(std::get<XMMATRIX_POINTER>(_varVariantList.at(9).first)) = tWorldInvTransposeDX;
		*(std::get<XMMATRIX_POINTER>(_varVariantList.at(10).first)) = tTexTransformDX;
		*(std::get<XMMATRIX_POINTER>(_varVariantList.at(11).first)) = tWorldViewProjDX;
		
		*(std::get<CUSTOM_PBR_MATERIAL_POINTER>(_varVariantList.at(12).first)) = tPBRMaterial;

		std::get<SRV_POINTER>(_varVariantList.at(13).first) = tAlbedoSRV;
		std::get<SRV_POINTER>(_varVariantList.at(14).first) = tArmSRV;
		std::get<SRV_POINTER>(_varVariantList.at(15).first) = tNormalSRV;

		///���� ���� ���̴��� ����.
		_material->GetShader()->UpdateSingleVariable("gTexAlbedoSRV_GBuffer", std::get<SRV_POINTER>(_varVariantList.at(0).first));
		_material->GetShader()->UpdateSingleVariable("gTexNormalSRV_GBuffer", std::get<SRV_POINTER>(_varVariantList.at(1).first));
		_material->GetShader()->UpdateSingleVariable("gTexPositionSRV_GBuffer", std::get<SRV_POINTER>(_varVariantList.at(2).first));
		_material->GetShader()->UpdateSingleVariable("gTexArmSRV_GBuffer", std::get<SRV_POINTER>(_varVariantList.at(3).first));
		_material->GetShader()->UpdateSingleVariable("gTexDepthSRV_GBuffer", std::get<SRV_POINTER>(_varVariantList.at(4).first));
		_material->GetShader()->UpdateSingleVariable("gEyePosW", std::get<FLOAT_POINTER>(_varVariantList.at(5).first));
		_material->GetShader()->UpdateSingleVariable("gLightDataList", std::get<CUSTOM_LEGACY_DIRECTIONAL_LIGHT_POINTER>(_varVariantList.at(6).first));
		_material->GetShader()->UpdateSingleVariable("gDirLightListCount", std::get<INT_POINTER>(_varVariantList.at(7).first));

		_material->GetShader()->UpdateSingleVariable("gWorld", std::get<XMMATRIX_POINTER>(_varVariantList.at(8).first));
		_material->GetShader()->UpdateSingleVariable("gWorldInvTranspose", std::get<XMMATRIX_POINTER>(_varVariantList.at(9).first));
		_material->GetShader()->UpdateSingleVariable("gTexTransform", std::get<XMMATRIX_POINTER>(_varVariantList.at(10).first));
		_material->GetShader()->UpdateSingleVariable("gWorldViewProj", std::get<XMMATRIX_POINTER>(_varVariantList.at(11).first));
		_material->GetShader()->UpdateSingleVariable("gPBRMaterial", std::get<CUSTOM_PBR_MATERIAL_POINTER>(_varVariantList.at(12).first));
		_material->GetShader()->UpdateSingleVariable("gAlbedoMap", std::get<SRV_POINTER>(_varVariantList.at(13).first));
		_material->GetShader()->UpdateSingleVariable("gARMMap", std::get<SRV_POINTER>(_varVariantList.at(14).first));
		_material->GetShader()->UpdateSingleVariable("gNormalMap", std::get<SRV_POINTER>(_varVariantList.at(15).first));

		//�׻� ���������δ� UpdateShader()�� ȣ���ؾ� �ݿ��ȴ�.
		_material->GetShader()->UpdateShader();
	}
	//////////////////////////////////////////////////////////////////////////
	///BasicPBRSkinned.cso
	template<>
	void ShaderUpdater::Initialize<DeferredSkinnedPBRType>(BaseRenderable* baseRenderable)
	{
		//�ʼ���, ���߿� Shader�� �޸𸮸� �����ϴµ� �ʼ����̴�.
		this->_assignedObject = baseRenderable;

		LowLevelDX11* tLowDX = LowLevelDX11::GetInstance();
		////�ܼ��� SRV�� ��ó�� �ȵ��ư� ��. �ܼ��� ���� �����ϱ� ����. (GetTextureStorage�� ��� ������Ʈ ����! ���� ���� ���� SRV �����ؾ�)
		MemoryVariant tAlbedo_GBufferSRV = tLowDX->_deferredPBRShaderQuad->_pbrAlbedoGBuffer._shaderResourceView;		 									//0
		MemoryVariant tNormal_GBufferSRV = tLowDX->_deferredPBRShaderQuad->_pbrNormalGBuffer._shaderResourceView;		 									//0
		MemoryVariant tPosition_GBufferSRV = tLowDX->_deferredPBRShaderQuad->_pbrPositionGBuffer._shaderResourceView;										//0
		MemoryVariant tArm_GBufferSRV = tLowDX->_deferredPBRShaderQuad->_pbrArmGBuffer._shaderResourceView;											//0
		MemoryVariant tDepth_GBufferSRV = tLowDX->_deferredPBRShaderQuad->_pbrDepthGBuffer._shaderResourceView;											//0

		MemoryVariant tEyePosDX = new float[4];						//gEyePosW		

		//DEFERRED_PHONG_MAXIMUM_DIRECTIONAL_COUNT�� ������ �����Ǿ�� �Ѵ�.
		MemoryVariant tDirectionalLightList = new DirectionalLightData[10]; // gLightDataList
		MemoryVariant tDirectionalLightCount = new int; //gDirLightListCount

		MemoryVariant tWorldMatDX = new DirectX::XMMATRIX;			//gWorld												
		MemoryVariant tWorldInvTransposeDX = new DirectX::XMMATRIX; //gWorldInvTranspose									
		MemoryVariant tTexTransformDX = new DirectX::XMMATRIX;		//gTexTransform											
		MemoryVariant tWorldViewProjDX = new DirectX::XMMATRIX;		//gWorldViewProj	

		MemoryVariant tPBRMaterial = new PBRMaterialData;		//gLegacyMaterial										
		MemoryVariant tAlbedoMap = baseRenderable->GetTextureStorage()->GetDiffuseTexture(0)->GetSRV(); // gDiffuseMap		
		MemoryVariant tArmMap = baseRenderable->GetTextureStorage()->GetArmTexture(0)->GetSRV();   // gNormalMap		
		MemoryVariant tNormalMap = baseRenderable->GetTextureStorage()->GetNormalTexture(0)->GetSRV();   // gNormalMap		

		MemoryVariant tBoneMat = new ByteBuffer;

		////���߿� get<T>�� ���� �ε�������, ���� �Բ� Updater�� Vector�� �����Ѵ�.
		////(���ø� Ư¡ ������ ���� enum���� ġȯ�� ����ϰ�����, �ϴ��� ���� �������ش�.)
		////
		_varVariantList.push_back(std::make_pair(tAlbedo_GBufferSRV, tAlbedo_GBufferSRV.index()));		   //0
		_varVariantList.push_back(std::make_pair(tNormal_GBufferSRV, tNormal_GBufferSRV.index()));		   //1
		_varVariantList.push_back(std::make_pair(tPosition_GBufferSRV, tPosition_GBufferSRV.index()));	   //2
		_varVariantList.push_back(std::make_pair(tArm_GBufferSRV, tArm_GBufferSRV.index()));			   //3
		_varVariantList.push_back(std::make_pair(tDepth_GBufferSRV, tDepth_GBufferSRV.index()));		   //4
		_varVariantList.push_back(std::make_pair(tEyePosDX, tEyePosDX.index()));						   //5
		_varVariantList.push_back(std::make_pair(tDirectionalLightList, tDirectionalLightList.index()));   //6
		_varVariantList.push_back(std::make_pair(tDirectionalLightCount, tDirectionalLightCount.index())); //7
		_varVariantList.push_back(std::make_pair(tWorldMatDX, tWorldMatDX.index()));					   //8
		_varVariantList.push_back(std::make_pair(tWorldInvTransposeDX, tWorldInvTransposeDX.index()));	   //9
		_varVariantList.push_back(std::make_pair(tTexTransformDX, tTexTransformDX.index()));			   //10
		_varVariantList.push_back(std::make_pair(tWorldViewProjDX, tWorldViewProjDX.index()));			   //11
		_varVariantList.push_back(std::make_pair(tPBRMaterial, tPBRMaterial.index()));					   //12
		_varVariantList.push_back(std::make_pair(tAlbedoMap, tAlbedoMap.index()));						   //13
		_varVariantList.push_back(std::make_pair(tArmMap, tArmMap.index()));							   //14
		_varVariantList.push_back(std::make_pair(tNormalMap, tNormalMap.index()));						   //15

		_varVariantList.push_back(std::make_pair(tBoneMat, tBoneMat.index())); //16 (Skinning)
	}

	template<>
	void ShaderUpdater::UpdateVariables<DeferredSkinnedPBRType>(const DirectX::XMFLOAT4X4& worldTM, double interval)
	{
		///�������! ��� �׳� Basic�� �ߵ����Ѿ� ��. (Tech) Not Lighting!)

		//�ϴ� ���� �޾Ƴ���.
		CameraMatrices* _cameraMatrix = _assignedObject->GetCameraMatrices();
		ShaderMaterial* _material = _assignedObject->GetShaderMaterial();
		BaseFormation* _formation = _assignedObject->GetBaseFormation();
		SceneConstantsUpdater* _sceneConstantsUpdater = SceneConstantsUpdater::GetInstance();
		RenderConstantData* _rcData = _sceneConstantsUpdater->GetRenderConstantData();

		//�������, Initialize�� �ڵ带 ������� Shader ���� ������Ʈ ��Ų��.
		LowLevelDX11* tLowDX = LowLevelDX11::GetInstance();

		//�̹� ���̳ο��� ��SRV ������ �ϴ�, �̸� Pixel �ϳ��� �޴°ɷ� �ٿ�����!
		ID3D11ShaderResourceView* tAlbedoGBuffer = ResourceManager::GetInstance().GetDefaultPixelTexture()->GetSRV();
		ID3D11ShaderResourceView* tNormalGBuffer = ResourceManager::GetInstance().GetDefaultPixelTexture()->GetSRV();
		ID3D11ShaderResourceView* tPositionGBuffer = ResourceManager::GetInstance().GetDefaultPixelTexture()->GetSRV();
		ID3D11ShaderResourceView* tArmGBuffer = ResourceManager::GetInstance().GetDefaultPixelTexture()->GetSRV();
		ID3D11ShaderResourceView* tDepthGBuffer = ResourceManager::GetInstance().GetDefaultPixelTexture()->GetSRV();

		DirectX::XMFLOAT3 tEyePosDX = _cameraMatrix->mPosition;
		DirectX::XMMATRIX tWorldMatDX = DirectX::XMLoadFloat4x4(&worldTM);
		DirectX::XMMATRIX tWorldInvTransposeDX = MathHelper::InverseTranspose(tWorldMatDX);
		DirectX::XMMATRIX tWorldViewProjDX = (tWorldMatDX)*_cameraMatrix->View() * _cameraMatrix->Proj();
		DirectX::XMMATRIX tTexTransformDX = DirectX::XMMatrixIdentity();

		PBRMaterialData tPBRMaterial = _assignedObject->GetPBRMaterialData();
		ID3D11ShaderResourceView* tAlbedoSRV = _assignedObject->GetTextureStorage()->GetDiffuseTexture(0)->GetSRV();
		ID3D11ShaderResourceView* tArmSRV = _assignedObject->GetTextureStorage()->GetArmTexture(0)->GetSRV();
		ID3D11ShaderResourceView* tNormalSRV = _assignedObject->GetTextureStorage()->GetNormalTexture(0)->GetSRV();

		std::get<SRV_POINTER>(_varVariantList.at(0).first) = tAlbedoGBuffer;
		std::get<SRV_POINTER>(_varVariantList.at(1).first) = tNormalGBuffer;
		std::get<SRV_POINTER>(_varVariantList.at(2).first) = tPositionGBuffer;
		std::get<SRV_POINTER>(_varVariantList.at(3).first) = tArmGBuffer;
		std::get<SRV_POINTER>(_varVariantList.at(4).first) = tDepthGBuffer;

		std::get<FLOAT_POINTER>(_varVariantList.at(5).first)[0] = tEyePosDX.x;
		std::get<FLOAT_POINTER>(_varVariantList.at(5).first)[1] = tEyePosDX.y;
		std::get<FLOAT_POINTER>(_varVariantList.at(5).first)[2] = tEyePosDX.z;

		///�ѹ��� ����.
		//Light �ø������ �������� �׷��� �ѹ��� �� �޾ƿ��� + GPU ���ޱ���.
		//���� �߰��� Light. (DEFERRED_PBR_MAXIMUM_DIRECTIONAL_COUNT �� �Ѿ��)
		int tCount = min(3, _sceneConstantsUpdater->GetDirLightCount());
		for (int i = 0; i < tCount; i++)
		{
			std::get<CUSTOM_LEGACY_DIRECTIONAL_LIGHT_POINTER>(_varVariantList.at(6).first)[i] =
				_rcData->dirLightList.at(_sceneConstantsUpdater->GetDirLightBrightIndex(i));
		}
		*(std::get<INT_POINTER>(_varVariantList.at(7).first)) = _sceneConstantsUpdater->GetDirLightCount();

		*(std::get<XMMATRIX_POINTER>(_varVariantList.at(8).first)) = tWorldMatDX; //gModel�̶� World�� �����ϰ� ����!
		*(std::get<XMMATRIX_POINTER>(_varVariantList.at(9).first)) = tWorldInvTransposeDX;
		*(std::get<XMMATRIX_POINTER>(_varVariantList.at(10).first)) = tTexTransformDX;
		*(std::get<XMMATRIX_POINTER>(_varVariantList.at(11).first)) = tWorldViewProjDX;

		*(std::get<CUSTOM_PBR_MATERIAL_POINTER>(_varVariantList.at(12).first)) = tPBRMaterial;

		std::get<SRV_POINTER>(_varVariantList.at(13).first) = tAlbedoSRV;
		std::get<SRV_POINTER>(_varVariantList.at(14).first) = tArmSRV;
		std::get<SRV_POINTER>(_varVariantList.at(15).first) = tNormalSRV;

		//������ �޸� PBR���� �߰��� ��Ű��!
		SkinnedMeshObject* _obj = static_cast<SkinnedMeshObject*>(_assignedObject);
		SkinnedMeshFormation* tForm = static_cast<SkinnedMeshFormation*>(_formation);
		ByteBuffer* tBoneMat = nullptr;
		tBoneMat = _obj->m_AnimHandler->_nowPlayingAnim->UpdateAnimationInfo(interval).first;
		std::get<BYTEBUFFER_POINTER>(_varVariantList.at(16).first) = tBoneMat;

		///���� ���� ���̴��� ����.
		_material->GetShader()->UpdateSingleVariable("gTexAlbedoSRV_GBuffer", std::get<SRV_POINTER>(_varVariantList.at(0).first));
		_material->GetShader()->UpdateSingleVariable("gTexNormalSRV_GBuffer", std::get<SRV_POINTER>(_varVariantList.at(1).first));
		_material->GetShader()->UpdateSingleVariable("gTexPositionSRV_GBuffer", std::get<SRV_POINTER>(_varVariantList.at(2).first));
		_material->GetShader()->UpdateSingleVariable("gTexArmSRV_GBuffer", std::get<SRV_POINTER>(_varVariantList.at(3).first));
		_material->GetShader()->UpdateSingleVariable("gTexDepthSRV_GBuffer", std::get<SRV_POINTER>(_varVariantList.at(4).first));
		_material->GetShader()->UpdateSingleVariable("gEyePosW", std::get<FLOAT_POINTER>(_varVariantList.at(5).first));
		_material->GetShader()->UpdateSingleVariable("gLightDataList", std::get<CUSTOM_LEGACY_DIRECTIONAL_LIGHT_POINTER>(_varVariantList.at(6).first));
		_material->GetShader()->UpdateSingleVariable("gDirLightListCount", std::get<INT_POINTER>(_varVariantList.at(7).first));

		_material->GetShader()->UpdateSingleVariable("gWorld", std::get<XMMATRIX_POINTER>(_varVariantList.at(8).first));
		_material->GetShader()->UpdateSingleVariable("gWorldInvTranspose", std::get<XMMATRIX_POINTER>(_varVariantList.at(9).first));
		_material->GetShader()->UpdateSingleVariable("gTexTransform", std::get<XMMATRIX_POINTER>(_varVariantList.at(10).first));
		_material->GetShader()->UpdateSingleVariable("gWorldViewProj", std::get<XMMATRIX_POINTER>(_varVariantList.at(11).first));
		_material->GetShader()->UpdateSingleVariable("gPBRMaterial", std::get<CUSTOM_PBR_MATERIAL_POINTER>(_varVariantList.at(12).first));
		_material->GetShader()->UpdateSingleVariable("gAlbedoMap", std::get<SRV_POINTER>(_varVariantList.at(13).first));
		_material->GetShader()->UpdateSingleVariable("gARMMap", std::get<SRV_POINTER>(_varVariantList.at(14).first));
		_material->GetShader()->UpdateSingleVariable("gNormalMap", std::get<SRV_POINTER>(_varVariantList.at(15).first));

		_material->GetShader()->UpdateSingleVariable("bones", std::get<BYTEBUFFER_POINTER>(_varVariantList.at(16).first));

		//�׻� ���������δ� UpdateShader()�� ȣ���ؾ� �ݿ��ȴ�.
		_material->GetShader()->UpdateShader();
	}



}
