#include "RenderPipeline.h"
#include "LowLevelDX11.h"
#include "resource.h"

#include "Shader.h"
#include "Texture.h"
#include "ShaderMaterial.h"

#include "VertexStructHelper.h"
#include "ResourceManager.h"

#include "SkinnedMeshObject.h"
#include "MeshObject.h"

#include "MeshFormation.h"
#include "SkinnedMeshFormation.h"

#include "../RocketGraphicsInterface/ISketchable.h"
#include "BaseSketchable.h"
#include "TextSketchable.h"
#include "ImageSketchable.h"

#include "RenderCubeMap.h"

#include "Define3D.h"
#include "Transform.h"
#include "CameraMatrices.h"
#include "RenderAnimation.h"
#include "SceneConstantsUpdater.h"
#include "ScreenBufferPhong.h"
#include "TextureRenderTarget.h"

#include <algorithm>
#include <tuple>
#include <limits>
#include <memory>
#include <cmath>
#include <filesystem>

#include "AnimationHandler.h"

#include "../RocketGraphicsInterface/CameraData.h"
#include "../RocketGraphicsInterface/LightData.h"
#include "../RocketGraphicsInterface/ModelData.h"
#include "../RocketGraphicsInterface/RenderConstantData.h"


//ASSIMP
#include "AssetModelLoader.h"
#include "AssetModelData.h"

//�ӽ� (������, Axis, Grid)
#include "Axis.h"
#include "Grid.h"
#include "BasicPrimitives.h"
#include "BasicLine.h"
#include "PrimitiveBatch.h"
#include "Effects.h"
#include "CommonStates.h"
#include "DirectXHelpers.h"
#include <wrl.h>

RocketCore::Graphics::IRocketGraphics* RocketCore::Graphics::CreateGraphicsInstance()
{
	return new RocketCore::Graphics::RenderPipeline;
}

void RocketCore::Graphics::ReleaseGraphicsInstance(RocketCore::Graphics::IRocketGraphics* instance)
{
	delete instance;
}

namespace RocketCore::Graphics
{
	RenderPipeline::~RenderPipeline()
	{
		//Unique Pointer�� ����.
	}

	void RenderPipeline::Initialize(void* _hWnd, int screenWidth, int screenHeight, bool isEditor /*= false*/)
	{
		this->_lowDX = LowLevelDX11::GetInstance();
		//������ LowRenderAPI�� Singleton.
		_lowDX->DirectX_Init(static_cast<HWND>(_hWnd), screenWidth, screenHeight, isEditor);
		InitFindPreRequiredEffects(); //���۵� ������ ������ ���� ����, �̸� ���� ���� ���� �ش� Effect ã�Ƴ���.
		_lowDX->DirectX_CreateFullScreenQuads();
		_lowDX->DirectX_InitGraphicsBuffer();
		_lowDX->DirectX_CreateBlendStates();

		m_ModelLoader = std::make_unique<AssetModelLoader>();
		_cameraMatrices = new CameraMatrices;
		_renderConstantData = new RenderConstantData;
		_sceneConstantsUpdater = SceneConstantsUpdater::GetInstance();
		_dxtkCommonStates = std::make_unique<DirectX::CommonStates>(_lowDX->m_d3dDevice.Get());
		_camData = new CameraData;

		//ResourceManager Initialize.
		ResourceManager::GetInstance().Initialize();

		InitDefaultCubemap(); //�⺻������ ����� ť����� ����.
		InitAxisGridResources(); // Hardcoded Axis / Grid.
		InitDebugGeometryResources(); // Basic Cube / Sphere
		InitDebugLineResources(); // Basic Line
		Init2DResources(); // Sprite / Font.
		

		//Scene Constant Initialize.
		_sceneConstantsUpdater->Initialize();
	}

	void RenderPipeline::SetRenderData(RenderConstantData _rInfo)
	{
		*_renderConstantData = _rInfo;
		
		//�Բ� RenderAnimation�� DeltaTime���� �������ش�.
		RenderAnimation::_dTimeRecord = _rInfo.deltaTime;

		//���⼭ ���� �ް�, SceneConstantsUpdater�� �۵��ؾ� �Ѵ�.
		//����Ʈ Constants Load.
		_sceneConstantsUpdater->PlaceShaderConstants(_renderConstantData);
	}

	RocketCore::Graphics::IRenderable* RenderPipeline::MakeRenderable(ModelData modelData)
	{
		std::string tFBXPath = modelData.fbxPath;
		std::string tShaderPath = modelData.shaderPath;

		std::filesystem::path tModelPath(tFBXPath);
		std::string tModelName = tModelPath.filename().string();

		std::filesystem::path tFXPath(tShaderPath);
		std::string tFXName = tFXPath.filename().string();

		//3D Model ã��.
		AssetModelData* tModelData = ResourceManager::GetInstance().FindModelByFileName(tModelName);
		if (tModelData == nullptr)
		{
			m_ModelLoader->LoadMeshFile(tFBXPath);
			tModelData = ResourceManager::GetInstance().FindModelByFileName(tModelName);
			assert(tModelData != nullptr);
		}

		bool tIsFound = ResourceManager::GetInstance().FoundExistingFXFile(tFXName);
		if (!tIsFound)
		{
			///���� ����, ������� ������Ʈ�� ������ �� �־�� �Ѵ�!
			//�ȿ� Shader ���� �� Textures �������� �����.
			ResourceManager::GetInstance().AddShaderDataWithResource(tShaderPath, modelData.renderingType);
		}
		ShaderInputSet tInputSet = ResourceManager::GetInstance().FindShaderInputSetByName(tFXName);

		//�� Object���� Pointer���� �����ϴ� Texture Storage ����ü�� ��� �����ȴ�.
		TextureStorage* tTextureStorage = new TextureStorage;
		//���� Texture ��θ� ���־��!
		tTextureStorage->AssignTextures(modelData);

		//�� ��������, Formation�� �󸶳� ����Ʈ ���� ������/��ü ũ��/InputLayout�� �˱⿡, �̸� Ȱ���� �𵨰��� ������ �̸� �������� �� �ִ�.
		//���� Data Assign!
		BaseFormation* tFormation = new MeshFormation(tInputSet.second, tModelData);

		ResourceManager::GetInstance().InsertNameFormation(tFBXPath, tFormation);

		tFormation->AssignData(tModelData);

		//Culling�� ���� AABB�� �߰�. ��� ������ Mesh�� 1������ ������ ��!
		DirectX::XMFLOAT3 minBounds = tModelData->_sceneData->m_MeshList.at(0)->m_AABB.m_MinVec;
		DirectX::XMFLOAT3 maxBounds = tModelData->_sceneData->m_MeshList.at(0)->m_AABB.m_MaxVec;

		MeshObject* tObject = new MeshObject(tInputSet.first, tFormation,
			_cameraMatrices, tTextureStorage, modelData.renderingType, minBounds, maxBounds);

		///230812 ���̴��� �ϰ������� �ؽ��ĸ� ������Ű�� ������ ������!
		//MeshObject ��ü�� �����ϱ� ����, ������ �ڵ带 �۵���Ų��.
		//���� ��ü���� Initialize �ڵ带 �������ֱ⵵ ��.
		tObject->InitializeShaderFunctions(tShaderPath, modelData.renderingType);

		//�ڱ� �ڽ��� �����ϱ� ��, �ڽ��� ������ ���´�. Map��!
		tObject->_objectName = modelData.objName;
		ResourceManager::GetInstance().InsertNameRenderable(modelData.objName, tObject);

		return tObject;
	}

	RocketCore::Graphics::IRenderable* RenderPipeline::MakeSkinnedRenderable(ModelData modelData)
	{
		std::string tFBXPath = modelData.fbxPath;
		std::string tShaderPath = modelData.shaderPath;

		std::filesystem::path tModelPath(tFBXPath);
		std::string tModelName = tModelPath.filename().string();

		std::filesystem::path tFXPath(tShaderPath);
		std::string tFXName = tFXPath.filename().string();

		//3D Model ã��.
		AssetModelData* tModelData = ResourceManager::GetInstance().FindModelByFileName(tModelName);
		if (tModelData == nullptr)
		{
			m_ModelLoader->LoadSkinnedMeshFile(tFBXPath);
			tModelData = ResourceManager::GetInstance().FindModelByFileName(tModelName);
			assert(tModelData != nullptr);
		}

		bool tIsFound = ResourceManager::GetInstance().FoundExistingFXFile(tFXName);
		if (!tIsFound)
		{
			//�ȿ� Shader ���� �� Textures �������� �����.
			ResourceManager::GetInstance().AddShaderDataWithResource(tShaderPath, modelData.renderingType);
		}
		ShaderInputSet tInputSet = ResourceManager::GetInstance().FindShaderInputSetByName(tFXName);

		//�� Object���� Pointer���� �����ϴ� Texture Storage ����ü�� ��� �����ȴ�.
		TextureStorage* tTextureStorage = new TextureStorage;
		//���� Texture ��θ� ���־��!
		tTextureStorage->AssignTextures(modelData);

		//�� ��������, Formation�� �󸶳� ����Ʈ ���� ������/��ü ũ��/InputLayout�� �˱⿡, �̸� Ȱ���� �𵨰��� ������ �̸� �������� �� �ִ�.
		//���� Data Assign!
		BaseFormation* tFormation = new SkinnedMeshFormation(tInputSet.second, tModelData);

		ResourceManager::GetInstance().InsertNameFormation(tFBXPath, tFormation);

		tFormation->AssignData(tModelData);

		//Culling�� ���� AABB�� �߰�. ��� ������ Mesh�� 1������ ������ ��!
		DirectX::XMFLOAT3 minBounds = tModelData->_sceneData->m_MeshList.at(0)->m_AABB.m_MinVec;
		DirectX::XMFLOAT3 maxBounds = tModelData->_sceneData->m_MeshList.at(0)->m_AABB.m_MaxVec;

		SkinnedMeshObject* tObject = new SkinnedMeshObject(tInputSet.first, tFormation,
			_cameraMatrices, tTextureStorage, modelData.renderingType, minBounds, maxBounds);

		///230812 ���̴��� �ϰ������� �ؽ��ĸ� ������Ű�� ������ ������!
		//MeshObject ��ü�� �����ϱ� ����, ������ �ڵ带 �۵���Ų��.
		//���� ��ü���� Initialize �ڵ带 �������ֱ⵵ ��.
		tObject->InitializeShaderFunctions(tShaderPath, modelData.renderingType);

		//�ڱ� �ڽ��� �����ϱ� ��, �ڽ��� ������ ���´�. Map��!
		tObject->_objectName = modelData.objName;
		ResourceManager::GetInstance().InsertNameRenderable(modelData.objName, tObject);

		return tObject;
	}

	void RenderPipeline::MakeAnim(AnimDesc desc, AnimData* data)
	{
		if (desc.name == "")
		{
			return;
		}

		//Formation�� �����ٰ� �Ҵ����ֱ�.
		std::string tOwnerPathString = desc.ownerModelPath;
		BaseFormation* tBaseForm = ResourceManager::GetInstance().FindFormationByModelPath(tOwnerPathString);
		SkinnedMeshFormation* tFormation = static_cast<SkinnedMeshFormation*>(tBaseForm);

		assert(tFormation != nullptr);

		std::string descName = desc.name;

		//1. 3D Model ã��.
		RenderAnimation* tAnimData = ResourceManager::GetInstance().FindAnimationByName(descName);

		if (tAnimData == nullptr)
		{
			this->m_ModelLoader->LoadAnimation(tFormation,desc);
			tAnimData = ResourceManager::GetInstance().FindAnimationByName(descName);
			assert(tAnimData != nullptr);
		}

		//2. AnimDesc�� ������ �������� RenderObject�� AnimationHandler�� �ֱ� / Animation�� ���� RenderUsageNode ����.
		///MakeAnim�� �� Object�� �𸣴µ�, ��� ȣ������?
		///�߰��� Insert�ؼ� Assign�ϴ� �ְ� �ʿ��ϴ�.
		
		BaseRenderable* tBaseRen = ResourceManager::GetInstance().FindRenderableByObjectName(desc.objName);
		assert(tBaseRen != nullptr);
		SkinnedMeshObject* tSkinnedMesh = static_cast<SkinnedMeshObject*>(tBaseRen);

		std::string tNameString = desc.name;
		tSkinnedMesh->m_AnimHandler->InsertAnimationWithName(tNameString, tAnimData);
		tSkinnedMesh->m_AnimHandler->_nowPlayingAnim = tAnimData;

		tAnimData->Initialize();
		
		//[TW] AnimData�� �ֱ�
		auto tRA = tSkinnedMesh->m_AnimHandler->_nowPlayingAnim;
		double durInTick = tRA->_assetAnimSceneData->m_AnimationList[0]->m_Duration;
		double tAnimTickPerSec = tRA->_assetAnimSceneData->m_AnimationList[0]->m_TicksPerSecond;
		double tSPT = (double)1.0f / (double)tAnimTickPerSec;
		data->duration = durInTick * tSPT;
		data->name = desc.name;
	}

	void RenderPipeline::DeleteRenderable(IRenderable* instance)
	{
		delete instance;
	}

	void RenderPipeline::DeleteSketchable(ISketchable* instance)
	{
		delete instance;
	}

	void RenderPipeline::UpdateCamera(const CameraData& cameraData)
	{
		*_camData = cameraData;
		_cameraMatrices->UpdateLocalAxisValues(_camData);
		_cameraMatrices->UpdateViewMatrix(_camData);
		_cameraMatrices->UpdateFrustumPlanes();
	}

	void RenderPipeline::Resize(int _width, int _height)
	{
		_lowDX->m_WindowWidth = _width;
		_lowDX->m_WindowHeight = _height;
		
		//[TW] Rendering Resize Graphic API ���� �־�� ��.
		_lowDX->DirectX_OnResize();
	}

	void RenderPipeline::Finalize()
	{
		for (UINT i = 0; i < _spriteFontArray.size(); i++)
		{
			if (_spriteFontArray[i] != nullptr)
			{
				delete _spriteFontArray[i];
			}
		}

		if (_basicEffect != nullptr)
		{
			delete _basicEffect;
		}

		if (_spriteBatch != nullptr)
		{
			delete _spriteBatch;
		}

		if (_primitiveBatch != nullptr)
		{
			delete _primitiveBatch;
		}

		if (_renderConstantData != nullptr)
		{
			delete _renderConstantData;
		}

		ReleaseCOM(_debugLineInputLayout);
	
	}

	void RenderPipeline::RenderAxisGrid()
	{
		//[TW] Temporary
		tAxis->Update(DirectX::XMMatrixIdentity(), _cameraMatrices->View(), _cameraMatrices->Proj());
		tGrid->Update(DirectX::XMMatrixIdentity(), _cameraMatrices->View(), _cameraMatrices->Proj());

		tAxis->Render();
		tGrid->Render();
	}

	void RenderPipeline::RenderCube(const RocketEngine::RMFLOAT4X4& worldTM, const PrimitiveData& primitiveData)
	{
		DirectX::XMFLOAT4X4 tXMWorld = MathHelper::RMToXMFloat4x4(worldTM);
		_debugCube->Render(tXMWorld, _cameraMatrices->View(), _cameraMatrices->Proj(), primitiveData.isWire, primitiveData.r, primitiveData.g, primitiveData.b, 1.0f);
	}

	void RenderPipeline::RenderCube(const RocketEngine::RMFLOAT4X4& worldTM, float x, float y, float z, const PrimitiveData& primitiveData)
	{
		DirectX::XMFLOAT4X4 tXMWorld = MathHelper::RMToXMFloat4x4(worldTM);
		DirectX::XMMATRIX worldMatrix = DirectX::XMLoadFloat4x4(&tXMWorld);
		DirectX::XMVECTOR s;
		DirectX::XMVECTOR r;
		DirectX::XMVECTOR t;
		DirectX::XMMatrixDecompose(&s, &r, &t, worldMatrix);

		DirectX::XMMATRIX result = DirectX::XMMatrixAffineTransformation({ x,y,z }, { 0,0,0 }, r, t);

		DirectX::XMStoreFloat4x4(&tXMWorld,result);

		_debugCube->Render(tXMWorld, _cameraMatrices->View(), _cameraMatrices->Proj(), primitiveData.isWire, primitiveData.r, primitiveData.g, primitiveData.b, 1.0f);
	}

	void RenderPipeline::RenderSphere(RocketEngine::RMFLOAT4X4 worldTM, PrimitiveData primitiveData)
	{
		DirectX::XMFLOAT4X4 tXMWorld = MathHelper::RMToXMFloat4x4(worldTM);
		_debugSphere->Render(tXMWorld, _cameraMatrices->View(), _cameraMatrices->Proj(), primitiveData.isWire, primitiveData.r, primitiveData.g, primitiveData.b, 1.0f);
	}

	void RenderPipeline::RenderLine(RocketEngine::RMFLOAT3 firstPos, RocketEngine::RMFLOAT3 secondPos, RocketEngine::RMFLOAT4 color)
	{
		DirectX::XMFLOAT3 tFirstPos = MathHelper::RMToXMFloat3(firstPos);
		DirectX::XMFLOAT3 tSecondPos = MathHelper::RMToXMFloat3(secondPos);
		DirectX::XMFLOAT4 tColor = MathHelper::RMToXMFloat4(color);

		_debugLine->Render(_primitiveBatch, _basicEffect, _commonEffectState.get(), tFirstPos, tSecondPos, tColor);
	}

	void RenderPipeline::RenderRayLine(float distance, RocketEngine::RMFLOAT4 color)
	{
		RocketEngine::RMFLOAT3 cameraPos = RocketEngine::RMFLOAT3();
		cameraPos.x = _camData->position.x;
		cameraPos.y = _camData->position.y;
		cameraPos.z = _camData->position.z;
		DirectX::XMFLOAT3 tFirstPos = { cameraPos.x, cameraPos.y, cameraPos.z };

		//XMFLOAT3 direction = { _cameraMatrices->viewMat._31,_cameraMatrices->viewMat._32,_cameraMatrices->viewMat._33 };
		XMFLOAT3 direction = XMFLOAT3(_camData->rotation.x, _camData->rotation.y, _camData->rotation.z);
		RocketEngine::RMFLOAT3 secondPos = cameraPos;
		secondPos.x += (direction.x * distance);
		secondPos.y += (direction.y * distance);
		secondPos.z += (direction.z * distance);
		DirectX::XMFLOAT3 tSecondPos = { secondPos.x, secondPos.y,secondPos.z };

		DirectX::XMFLOAT4 tColor = MathHelper::RMToXMFloat4(color);

		_debugLine->Render(_primitiveBatch, _basicEffect, _commonEffectState.get(), tFirstPos, tSecondPos, tColor);
	}

	void RenderPipeline::Render2DBox(RocketEngine::RMFLOAT2 LT, RocketEngine::RMFLOAT2 RB, RocketEngine::RMFLOAT4 color)
	{
		DirectX::XMFLOAT2 tLT = MathHelper::RMToXMFloat2(LT);
		DirectX::XMFLOAT2 tRB = MathHelper::RMToXMFloat2(RB);
		DirectX::XMFLOAT4 tColor = MathHelper::RMToXMFloat4(color);
		DirectX::XMVECTOR tColorVec = DirectX::XMLoadFloat4(&tColor);

		//�밡�� ����.. Pixel Bitmap �׸����.. ����.. ��¥ �ƴ� �� ����...
		DirectX::XMFLOAT2 tTopLeft = XMFLOAT2(tLT.x, tLT.y);
		DirectX::XMFLOAT2 tTopRight = XMFLOAT2(tRB.x, tLT.y);
		DirectX::XMFLOAT2 tBottomLeft = XMFLOAT2(tLT.x, tRB.y);
		DirectX::XMFLOAT2 tBottomRight = XMFLOAT2(tRB.x, tRB.y);
		
		_spriteBatch->Draw(ResourceManager::GetInstance().GetDefaultPixelTexture()->GetSRV(), tTopLeft, nullptr, tColorVec, 0, DirectX::XMFLOAT2(0, 0), 5); // TopLeft
		_spriteBatch->Draw(ResourceManager::GetInstance().GetDefaultPixelTexture()->GetSRV(), MathHelper::Float2Lerp(tTopLeft, tTopRight, 0.25f), nullptr, tColorVec, 0, DirectX::XMFLOAT2(0, 0), 5);
		_spriteBatch->Draw(ResourceManager::GetInstance().GetDefaultPixelTexture()->GetSRV(), MathHelper::Float2Lerp(tTopLeft, tTopRight, 0.5f), nullptr, tColorVec, 0, DirectX::XMFLOAT2(0, 0), 5);
		_spriteBatch->Draw(ResourceManager::GetInstance().GetDefaultPixelTexture()->GetSRV(), MathHelper::Float2Lerp(tTopLeft, tTopRight, 0.75f), nullptr, tColorVec, 0, DirectX::XMFLOAT2(0, 0), 5);
		_spriteBatch->Draw(ResourceManager::GetInstance().GetDefaultPixelTexture()->GetSRV(), tTopRight, nullptr, tColorVec, 0, DirectX::XMFLOAT2(0, 0), 5); // TopRight
		_spriteBatch->Draw(ResourceManager::GetInstance().GetDefaultPixelTexture()->GetSRV(), MathHelper::Float2Lerp(tTopRight, tBottomRight, 0.25f), nullptr, tColorVec, 0, DirectX::XMFLOAT2(0, 0), 5);
		_spriteBatch->Draw(ResourceManager::GetInstance().GetDefaultPixelTexture()->GetSRV(), MathHelper::Float2Lerp(tTopRight, tBottomRight, 0.5f), nullptr, tColorVec, 0, DirectX::XMFLOAT2(0, 0), 5);
		_spriteBatch->Draw(ResourceManager::GetInstance().GetDefaultPixelTexture()->GetSRV(), MathHelper::Float2Lerp(tTopRight, tBottomRight, 0.75f), nullptr, tColorVec, 0, DirectX::XMFLOAT2(0, 0), 5);
		_spriteBatch->Draw(ResourceManager::GetInstance().GetDefaultPixelTexture()->GetSRV(), tBottomRight, nullptr, tColorVec, 0, DirectX::XMFLOAT2(0, 0), 5); // Bottom Right
		_spriteBatch->Draw(ResourceManager::GetInstance().GetDefaultPixelTexture()->GetSRV(), MathHelper::Float2Lerp(tBottomLeft, tBottomRight, 0.25f), nullptr, tColorVec, 0, DirectX::XMFLOAT2(0, 0), 5);
		_spriteBatch->Draw(ResourceManager::GetInstance().GetDefaultPixelTexture()->GetSRV(), MathHelper::Float2Lerp(tBottomLeft, tBottomRight, 0.5f), nullptr, tColorVec, 0, DirectX::XMFLOAT2(0, 0), 5);
		_spriteBatch->Draw(ResourceManager::GetInstance().GetDefaultPixelTexture()->GetSRV(), MathHelper::Float2Lerp(tBottomLeft, tBottomRight, 0.75f), nullptr, tColorVec, 0, DirectX::XMFLOAT2(0, 0), 5);
		_spriteBatch->Draw(ResourceManager::GetInstance().GetDefaultPixelTexture()->GetSRV(), tBottomLeft, nullptr, tColorVec, 0, DirectX::XMFLOAT2(0, 0), 5); // BottomLeft
		_spriteBatch->Draw(ResourceManager::GetInstance().GetDefaultPixelTexture()->GetSRV(), MathHelper::Float2Lerp(tBottomLeft, tTopLeft, 0.25f), nullptr, tColorVec, 0, DirectX::XMFLOAT2(0, 0), 5);
		_spriteBatch->Draw(ResourceManager::GetInstance().GetDefaultPixelTexture()->GetSRV(), MathHelper::Float2Lerp(tBottomLeft, tTopLeft, 0.5f), nullptr, tColorVec, 0, DirectX::XMFLOAT2(0, 0), 5);
		_spriteBatch->Draw(ResourceManager::GetInstance().GetDefaultPixelTexture()->GetSRV(), MathHelper::Float2Lerp(tBottomLeft, tTopLeft, 0.75f), nullptr, tColorVec, 0, DirectX::XMFLOAT2(0, 0), 5);
	}

	void RenderPipeline::RenderDebugText(float x, float y, const char* contents, float size)
	{
		///�ϴ�, ������ ũ�Ⱑ ��������! 
		//�ϴ� Font ����� �ް�, ���� ����� ���� ����.
		std::array<int, SPRITEFONT_INSTANCE_COUNT> tDetermineIndex;
		for (int i = 0; i < SPRITEFONT_INSTANCE_COUNT; i++)
		{
			tDetermineIndex[i] = abs(size - _fontSizeArray[i]);
		}
		std::array<int, SPRITEFONT_INSTANCE_COUNT>::iterator tMinResult = std::min_element(tDetermineIndex.begin(), tDetermineIndex.end());
		int tAllocateSizeIndex = std::distance(tDetermineIndex.begin(), tMinResult);

		TCHAR _wBuffer[TextSketchable::MAXIMUM_TEXTBUFFER_COUNT];

		for (int i = 0; i < TextSketchable::MAXIMUM_TEXTBUFFER_COUNT; i++)
		{
			_wBuffer[i] = '\0';
		}

		std::string tStr = contents;
		int _tStringSize = std::min(tStr.size(), (size_t)TextSketchable::MAXIMUM_TEXTBUFFER_COUNT);
		MultiByteToWideChar(CP_ACP, 0, contents, strlen(contents) + 1, _wBuffer, _tStringSize);

		_spriteFontArray[tAllocateSizeIndex]->DrawString(_spriteBatch, _wBuffer, DirectX::XMFLOAT2(x, y), DirectX::Colors::White);
	}

	RocketCore::Graphics::ISketchable* RenderPipeline::MakeText(float fontSize, float width, float height, RocketEngine::RMFLOAT4 color)
	{
		///�ϴ�, ������ ũ�Ⱑ ��������! 
		//�ϴ� Font ����� �ް�, ���� ����� ���� ����.
		std::array<int, SPRITEFONT_INSTANCE_COUNT> tDetermineIndex;
		for (int i = 0; i < SPRITEFONT_INSTANCE_COUNT; i++)
		{
			tDetermineIndex[i] = abs(fontSize - _fontSizeArray[i]);
		}
		std::array<int, SPRITEFONT_INSTANCE_COUNT>::iterator tMinResult = std::min_element(tDetermineIndex.begin(), tDetermineIndex.end());
		int tAllocateSizeIndex = std::distance(tDetermineIndex.begin(), tMinResult);
		TextSketchable* tNewText = new TextSketchable(_spriteBatch, _spriteFontArray[tAllocateSizeIndex], width, height, MathHelper::RMToXMFloat4(color));

		return tNewText;
	}

	RocketCore::Graphics::ISketchable* RenderPipeline::MakeImage(const char* imagePath, float width, float height)
	{
		ImageSketchable* tNewImage = new ImageSketchable(_spriteBatch, width, height, imagePath);
		return tNewImage;
	}
	
	void RenderPipeline::BeginTotalRender()
	{
		///Particle Logic ����.
		UpdateParticleControlLogic();
		///
		
		///���� ������ ���� ����.
		//�ϴ� Forward Renderer�� ���·� ����.
		_lowDX->m_d3dMainContext->ClearRenderTargetView(
			LowLevelDX11::GetInstance()->m_MainRenderTargetView.Get(),
			DirectX::Colors::DarkSlateGray
		);

		_lowDX->m_d3dMainContext->ClearDepthStencilView(
			_lowDX->m_pDepthStencilView.Get(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
			1.0f,
			0);
	
	}

	void RenderPipeline::BeginDeferredPhongMeshRender()
	{
		//DeferredPhong�� ���� ������ ������Ʈ�� ������.
		_lowDX->m_d3dMainContext.Get()->IASetInputLayout(_phongDeferredShaderInputSet.second.m_Layout);
		//�̹� ���۵� ���̴� ����� ���� Rasterize�� ����Ʈ �����´�.
		
		//auto transparent = XMFLOAT4(0.888f, 1.0f, 1.0f, 0.0f);'
		///���⿡ �ִ°� Stencil�� �ɷ��� ������ �ʴ´�..?
		auto transparent = XMFLOAT4(0.0f, 1.0f, 1.0f, 0.0f); ///�� ���� ������ �߿��ϴ�. Depth�� ����Ϸ���!
		//�������� G-Buffer���� RenderTarget���� Clear�Ѵ�.
		_lowDX->m_d3dMainContext.Get()->ClearRenderTargetView(_lowDX->_deferredPhongShaderQuad->_phongDiffuseGBuffer._renderTargetView, reinterpret_cast<const float*>(&transparent));
		_lowDX->m_d3dMainContext.Get()->ClearRenderTargetView(_lowDX->_deferredPhongShaderQuad->_phongNormalGBuffer._renderTargetView, reinterpret_cast<const float*>(&transparent));
		_lowDX->m_d3dMainContext.Get()->ClearRenderTargetView(_lowDX->_deferredPhongShaderQuad->_phongPositionGBuffer._renderTargetView, reinterpret_cast<const float*>(&transparent));
		_lowDX->m_d3dMainContext.Get()->ClearRenderTargetView(_lowDX->_deferredPhongShaderQuad->_phongDepthGBuffer._renderTargetView, reinterpret_cast<const float*>(&transparent));

		//������ ��ü ó�� ���� �޾Ƴ��� RenderTarget���� ����.
		ID3D11RenderTargetView* renderTargets[] = {
		_lowDX->_deferredPhongShaderQuad->_phongDiffuseGBuffer._renderTargetView,
		_lowDX->_deferredPhongShaderQuad->_phongNormalGBuffer._renderTargetView,
		_lowDX->_deferredPhongShaderQuad->_phongPositionGBuffer._renderTargetView,
		_lowDX->_deferredPhongShaderQuad->_phongDepthGBuffer._renderTargetView
		};

		//��� ���ձ⿡�� ���� RenderTarget���� ��������� DX�� ����.
		_lowDX->m_d3dMainContext.Get()->OMSetRenderTargets(ScreenBuffer::DEFERRED_PHONG_BUFFER_COUNT, renderTargets, _lowDX->m_pDepthStencilView.Get());
		
		//DepthStencil ���� Ŭ����!
		//_lowDX->m_d3dMainContext.Get()->ClearDepthStencilView(_lowDX->m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

		//�⺻������ Basic Technique�� ���ý��Ѿ� �Ѵ�.
		_phongDeferredShaderInputSet.first->GetShader()->SetMainTechnique("BasicTech");
		_lowDX->m_d3dMainContext.Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		auto selectedTech = _phongDeferredShaderInputSet.first->GetShader()->GetMainTechnique();
		selectedTech->GetPassByIndex(0)->Apply(0, _lowDX->m_d3dMainContext.Get());

		
	}

	void RenderPipeline::EndDeferredPhongMeshRender()
	{
		//���� RenderTarget���� ����.
		ID3D11RenderTargetView* resetRenderTargets[] = { nullptr, nullptr, nullptr, nullptr };
		_lowDX->m_d3dMainContext.Get()->OMSetRenderTargets(ScreenBuffer::DEFERRED_PHONG_BUFFER_COUNT, resetRenderTargets, _lowDX->m_pDepthStencilView.Get());
		//���⼭ "�����ִ�" �۾��� �����ؾ� �Ѵ�.
		//�켱, InputLayout�� �ٽ� Quad�� �°� �ٲ��ش�.
		_lowDX->_deferredPhongShaderQuad->UseQuadLayout();

		ID3D11RenderTargetView*& tDeferred = _lowDX->_deferredPhongFullScreenQuad->_defTextureRenderTarget._renderTargetView;
		_lowDX->m_d3dMainContext.Get()->OMSetRenderTargets(1, &tDeferred, nullptr);

		auto transparent = XMFLOAT4(0.0f, 1.0f, 1.0f, 0.0f); ///�� ���� ������ �߿��ϴ�. Depth�� ����Ϸ���!
		_lowDX->m_d3dMainContext.Get()->ClearRenderTargetView(tDeferred, reinterpret_cast<const float*>(&transparent));
		_lowDX->m_d3dMainContext.Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//�̹� �ȿ����� SRV�� �� ���� ���̴�. �̰� �ѹ� ��ŵ!
		//���ø� ����! ����, ���⼭�� ModelMatrix�� ���� ����� �ִ´�.
		auto& quad = _lowDX->_deferredPhongShaderQuad;
		quad->_diffuseGBufferEffectVar->SetResource(quad->_phongDiffuseGBuffer._shaderResourceView);
		quad->_normalGBufferEffectVar->SetResource(quad->_phongNormalGBuffer._shaderResourceView);
		quad->_positionGBufferEffectVar->SetResource(quad->_phongPositionGBuffer._shaderResourceView);
		quad->_depthGBufferEffectVar->SetResource(quad->_phongDepthGBuffer._shaderResourceView);
		///�� �������� Begin�� clear�ߴ� 0.888f�� Depth�� �νĵȴ�. �̸� ���ľ� �Ѵ�!

		//���� ������ ���ؼ� Deferred Technique�� �������� �����ߴ�.
		_phongDeferredShaderInputSet.first->GetShader()->SetMainTechnique("LightingTech");
		_phongDeferredShaderInputSet.first->GetShader()->
			GetMainTechnique()->GetPassByIndex(0)->Apply(0, _lowDX->m_d3dMainContext.Get());

		//Buffer�� Second�� ���ε�.
		static const UINT stride = sizeof(GeometryGenerator::GeomVertex_PosNormalTex);
		static const UINT offset = 0;

		 _lowDX->m_d3dMainContext.Get()->IASetVertexBuffers(0, 1, &quad->_vertexBuffer, &stride, &offset);
		 _lowDX->m_d3dMainContext.Get()->IASetIndexBuffer(quad->_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		 _lowDX->m_d3dMainContext.Get()->DrawIndexed(quad->_indexCount, 0, 0);


		 ////���� ������ ���ҽ� ����.
		 //quad->_diffuseGBufferEffectVar->SetResource(nullptr);
		 //quad->_normalGBufferEffectVar->SetResource(nullptr);
		 //quad->_positionGBufferEffectVar->SetResource(nullptr);
		 //quad->_depthGBufferEffectVar->SetResource(nullptr);
	}

	void RenderPipeline::BeginDeferredPBRMeshRender()
	{
		_lowDX->m_d3dMainContext->OMSetBlendState(_lowDX->m_NoBlend.Get(), nullptr, 0xFFFFFFFF);

		//DeferredPhong�� ���� ������ ������Ʈ�� ������.
		_lowDX->m_d3dMainContext.Get()->IASetInputLayout(_phongDeferredShaderInputSet.second.m_Layout);
		//�̹� ���۵� ���̴� ����� ���� Rasterize�� ����Ʈ �����´�.

		auto transparent = XMFLOAT4(0.0f, 1.0f, 1.0f, 0.0f); ///�� ���� ������ �߿��ϴ�. Depth�� ����Ϸ���!
		//�������� G-Buffer���� RenderTarget���� Clear�Ѵ�.
		_lowDX->m_d3dMainContext.Get()->ClearRenderTargetView(_lowDX->_deferredPBRShaderQuad->_pbrAlbedoGBuffer._renderTargetView, reinterpret_cast<const float*>(&transparent));
		_lowDX->m_d3dMainContext.Get()->ClearRenderTargetView(_lowDX->_deferredPBRShaderQuad->_pbrNormalGBuffer._renderTargetView, reinterpret_cast<const float*>(&transparent));
		_lowDX->m_d3dMainContext.Get()->ClearRenderTargetView(_lowDX->_deferredPBRShaderQuad->_pbrPositionGBuffer._renderTargetView, reinterpret_cast<const float*>(&transparent));
		_lowDX->m_d3dMainContext.Get()->ClearRenderTargetView(_lowDX->_deferredPBRShaderQuad->_pbrArmGBuffer._renderTargetView, reinterpret_cast<const float*>(&transparent));
		_lowDX->m_d3dMainContext.Get()->ClearRenderTargetView(_lowDX->_deferredPBRShaderQuad->_pbrDepthGBuffer._renderTargetView, reinterpret_cast<const float*>(&transparent));


		//������ ��ü ó�� ���� �޾Ƴ��� RenderTarget���� ����.
		ID3D11RenderTargetView* renderTargets[] = {
		_lowDX->_deferredPBRShaderQuad->_pbrAlbedoGBuffer._renderTargetView,
		_lowDX->_deferredPBRShaderQuad->_pbrNormalGBuffer._renderTargetView,
		_lowDX->_deferredPBRShaderQuad->_pbrPositionGBuffer._renderTargetView,
		_lowDX->_deferredPBRShaderQuad->_pbrArmGBuffer._renderTargetView,
		_lowDX->_deferredPBRShaderQuad->_pbrDepthGBuffer._renderTargetView
		};

		//��� ���ձ⿡�� ���� RenderTarget���� ��������� DX�� ����.
		_lowDX->m_d3dMainContext.Get()->OMSetRenderTargets(ScreenBuffer::DEFERRED_PBR_BUFFER_COUNT, renderTargets, _lowDX->m_pDepthStencilView.Get());
		
		
		//DepthStencil ���� Ŭ����!
		//_lowDX->m_d3dMainContext.Get()->ClearDepthStencilView(_lowDX->m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

		//�⺻������ Basic Technique�� ���ý��Ѿ� �Ѵ�.
		_pbrDeferredShaderInputSet.first->GetShader()->SetMainTechnique("BasicTech");
		_lowDX->m_d3dMainContext.Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		auto selectedTech = _pbrDeferredShaderInputSet.first->GetShader()->GetMainTechnique();
		selectedTech->GetPassByIndex(0)->Apply(0, _lowDX->m_d3dMainContext.Get());
	}

	void RenderPipeline::EndDeferredPBRMeshRender()
	{
		//���� RenderTarget���� ����.
		ID3D11RenderTargetView* resetRenderTargets[] = { nullptr, nullptr, nullptr, nullptr, nullptr };
		_lowDX->m_d3dMainContext.Get()->OMSetRenderTargets(ScreenBuffer::DEFERRED_PBR_BUFFER_COUNT, resetRenderTargets, _lowDX->m_pDepthStencilView.Get());
		//���⼭ "�����ִ�" �۾��� �����ؾ� �Ѵ�.
		//�켱, InputLayout�� �ٽ� Quad�� �°� �ٲ��ش�.
		_lowDX->_deferredPBRShaderQuad->UseQuadLayout();

		ID3D11RenderTargetView*& tDeferred = _lowDX->_deferredPBRFullScreenQuad->_defTextureRenderTarget._renderTargetView;
		_lowDX->m_d3dMainContext.Get()->OMSetRenderTargets(1, &tDeferred, nullptr);

		auto transparent = XMFLOAT4(0.0f, 1.0f, 1.0f, 0.0f); ///�� ���� ������ �߿��ϴ�. Depth�� ����Ϸ���!
		_lowDX->m_d3dMainContext.Get()->ClearRenderTargetView(tDeferred, reinterpret_cast<const float*>(&transparent));
		_lowDX->m_d3dMainContext.Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//�̹� �ȿ����� SRV�� �� ���� ���̴�. �̰� �ѹ� ��ŵ!
		//���ø� ����! ����, ���⼭�� ModelMatrix�� ���� ����� �ִ´�.
		auto& quad = _lowDX->_deferredPBRShaderQuad;
		quad->_albedoGBufferEffectVar->SetResource(quad->_pbrAlbedoGBuffer._shaderResourceView);
		quad->_normalGBufferEffectVar->SetResource(quad->_pbrNormalGBuffer._shaderResourceView);
		quad->_positionGBufferEffectVar->SetResource(quad->_pbrPositionGBuffer._shaderResourceView);
		quad->_armGBufferEffectVar->SetResource(quad->_pbrArmGBuffer._shaderResourceView);
		quad->_depthGBufferEffectVar->SetResource(quad->_pbrDepthGBuffer._shaderResourceView);

		//���� ������ ���ؼ� Deferred Technique�� �������� �����ߴ�.
		_pbrDeferredShaderInputSet.first->GetShader()->SetMainTechnique("LightingTech");
		_pbrDeferredShaderInputSet.first->GetShader()->
			GetMainTechnique()->GetPassByIndex(0)->Apply(0, _lowDX->m_d3dMainContext.Get());

		//Buffer�� Second�� ���ε�.
		static const UINT stride = sizeof(GeometryGenerator::GeomVertex_PosNormalTex);
		static const UINT offset = 0;

		_lowDX->m_d3dMainContext.Get()->IASetVertexBuffers(0, 1, &quad->_vertexBuffer, &stride, &offset);
		_lowDX->m_d3dMainContext.Get()->IASetIndexBuffer(quad->_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		_lowDX->m_d3dMainContext.Get()->DrawIndexed(quad->_indexCount, 0, 0);
	}

	void RenderPipeline::BeginDeferredPBRSkinnedMeshRender()
	{
		//DeferredPhong�� ���� ������ ������Ʈ�� ������.
		_lowDX->m_d3dMainContext.Get()->IASetInputLayout(_pbrSkinnedDeferredShaderInputSet.second.m_Layout);
		//�̹� ���۵� ���̴� ����� ���� Rasterize�� ����Ʈ �����´�.

		auto transparent = XMFLOAT4(0.0f, 1.0f, 1.0f, 0.0f); ///�� ���� ������ �߿��ϴ�. Depth�� ����Ϸ���!
		//�������� G-Buffer���� RenderTarget���� Clear�Ѵ�.
		_lowDX->m_d3dMainContext.Get()->ClearRenderTargetView(_lowDX->_deferredPBRSkinnedShaderQuad->_pbrAlbedoGBuffer._renderTargetView, reinterpret_cast<const float*>(&transparent));
		_lowDX->m_d3dMainContext.Get()->ClearRenderTargetView(_lowDX->_deferredPBRSkinnedShaderQuad->_pbrNormalGBuffer._renderTargetView, reinterpret_cast<const float*>(&transparent));
		_lowDX->m_d3dMainContext.Get()->ClearRenderTargetView(_lowDX->_deferredPBRSkinnedShaderQuad->_pbrPositionGBuffer._renderTargetView, reinterpret_cast<const float*>(&transparent));
		_lowDX->m_d3dMainContext.Get()->ClearRenderTargetView(_lowDX->_deferredPBRSkinnedShaderQuad->_pbrArmGBuffer._renderTargetView, reinterpret_cast<const float*>(&transparent));
		_lowDX->m_d3dMainContext.Get()->ClearRenderTargetView(_lowDX->_deferredPBRSkinnedShaderQuad->_pbrDepthGBuffer._renderTargetView, reinterpret_cast<const float*>(&transparent));

		//������ ��ü ó�� ���� �޾Ƴ��� RenderTarget���� ����.
		ID3D11RenderTargetView* renderTargets[] = {
		_lowDX->_deferredPBRSkinnedShaderQuad->_pbrAlbedoGBuffer._renderTargetView,
		_lowDX->_deferredPBRSkinnedShaderQuad->_pbrNormalGBuffer._renderTargetView,
		_lowDX->_deferredPBRSkinnedShaderQuad->_pbrPositionGBuffer._renderTargetView,
		_lowDX->_deferredPBRSkinnedShaderQuad->_pbrArmGBuffer._renderTargetView,
		_lowDX->_deferredPBRSkinnedShaderQuad->_pbrDepthGBuffer._renderTargetView
		};

		//��� ���ձ⿡�� ���� RenderTarget���� ��������� DX�� ����.
		_lowDX->m_d3dMainContext.Get()->OMSetRenderTargets(ScreenBuffer::DEFERRED_PBR_BUFFER_COUNT, renderTargets, _lowDX->m_pDepthStencilView.Get());

		
		//DepthStencil ���� Ŭ����!
		//_lowDX->m_d3dMainContext.Get()->ClearDepthStencilView(_lowDX->m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

		//�⺻������ Basic Technique�� ���ý��Ѿ� �Ѵ�.
		_pbrSkinnedDeferredShaderInputSet.first->GetShader()->SetMainTechnique("BasicTech");
		_lowDX->m_d3dMainContext.Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		auto selectedTech = _pbrSkinnedDeferredShaderInputSet.first->GetShader()->GetMainTechnique();
		selectedTech->GetPassByIndex(0)->Apply(0, _lowDX->m_d3dMainContext.Get());
	}

	void RenderPipeline::EndDeferredPBRSkinnedMeshRender()
	{
		//���� RenderTarget���� ����.
		ID3D11RenderTargetView* resetRenderTargets[] = { nullptr, nullptr, nullptr, nullptr, nullptr };
		_lowDX->m_d3dMainContext.Get()->OMSetRenderTargets(ScreenBuffer::DEFERRED_PBR_BUFFER_COUNT, resetRenderTargets, _lowDX->m_pDepthStencilView.Get());
		//���⼭ "�����ִ�" �۾��� �����ؾ� �Ѵ�.
		//�켱, InputLayout�� �ٽ� Quad�� �°� �ٲ��ش�.
		_lowDX->_deferredPBRSkinnedShaderQuad->UseQuadLayout();

		ID3D11RenderTargetView*& tDeferred = _lowDX->_deferredPBRSkinnedFullScreenQuad->_defTextureRenderTarget._renderTargetView;
		_lowDX->m_d3dMainContext.Get()->OMSetRenderTargets(1, &tDeferred, nullptr);

		auto transparent = XMFLOAT4(0.0f, 1.0f, 1.0f, 0.0f); ///�� ���� ������ �߿��ϴ�. Depth�� ����Ϸ���!

		_lowDX->m_d3dMainContext.Get()->ClearRenderTargetView(tDeferred, reinterpret_cast<const float*>(&transparent));
		_lowDX->m_d3dMainContext.Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//�̹� �ȿ����� SRV�� �� ���� ���̴�. �̰� �ѹ� ��ŵ!
		//���ø� ����! ����, ���⼭�� ModelMatrix�� ���� ����� �ִ´�.
		auto& quad = _lowDX->_deferredPBRSkinnedShaderQuad;
		quad->_albedoGBufferEffectVar->SetResource(quad->_pbrAlbedoGBuffer._shaderResourceView);
		quad->_normalGBufferEffectVar->SetResource(quad->_pbrNormalGBuffer._shaderResourceView);
		quad->_positionGBufferEffectVar->SetResource(quad->_pbrPositionGBuffer._shaderResourceView);
		quad->_armGBufferEffectVar->SetResource(quad->_pbrArmGBuffer._shaderResourceView);
		quad->_depthGBufferEffectVar->SetResource(quad->_pbrDepthGBuffer._shaderResourceView);

		//���� ������ ���ؼ� Deferred Technique�� �������� �����ߴ�.
		_pbrSkinnedDeferredShaderInputSet.first->GetShader()->SetMainTechnique("LightingTech");
		_pbrSkinnedDeferredShaderInputSet.first->GetShader()->
			GetMainTechnique()->GetPassByIndex(0)->Apply(0, _lowDX->m_d3dMainContext.Get());

		//Buffer�� Second�� ���ε�.
		static const UINT stride = sizeof(GeometryGenerator::GeomVertex_PosNormalTex);
		static const UINT offset = 0;

		_lowDX->m_d3dMainContext.Get()->IASetVertexBuffers(0, 1, &quad->_vertexBuffer, &stride, &offset);
		_lowDX->m_d3dMainContext.Get()->IASetIndexBuffer(quad->_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		_lowDX->m_d3dMainContext.Get()->DrawIndexed(quad->_indexCount, 0, 0);
	}

	void RenderPipeline::BeginForwardMeshRender()
	{
		//_lowDX->m_d3dMainContext.Get()->RSSetViewports(1, &(_lowDX->m_QuadViewport));

		//_lowDX->m_d3dMainContext->RSSetScissorRects(
		//	1,
		//	&(_lowDX->d3dScissorsRect)
		//);

		ID3D11RenderTargetView*& tForward = _lowDX->_forwardFullScreenQuad->_defTextureRenderTarget._renderTargetView;
		//ID3D11RenderTargetView* tMainP = _lowDX->m_MainRenderTargetView.Get();
		//ID3D11RenderTargetView** tMainPP = _lowDX->m_MainRenderTargetView.GetAddressOf();

		auto opaque = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		_lowDX->m_d3dMainContext.Get()->ClearRenderTargetView(tForward, reinterpret_cast<const float*>(&opaque));
		//_lowDX->m_d3dMainContext.Get()->ClearRenderTargetView(tMainP, reinterpret_cast<const float*>(&opaque));
		//���� Forward�� ������ ���̴�. ��� ���ձ����� MainRenderTargetView�� �ٷ� ����� ���̶� �˷�����!
		_lowDX->m_d3dMainContext->
			OMSetRenderTargets(1, &tForward, _lowDX->m_pDepthStencilView.Get());

		_lowDX->m_d3dMainContext.Get()->ClearDepthStencilView(_lowDX->m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

		//���⼭ OMSetDepthStencilState�� ���� DepthWriting�� ����ؾ� �Ѵ�.
		//�ٵ� Default�� MSDN�� ������ �̹� �Ǿ� �־.. ���� �ٲ� �ʿ�� ���� ��!
	
		_lowDX->m_d3dMainContext->OMSetDepthStencilState(_lowDX->m_NormalDSS.Get(), 0);
	}

	void RenderPipeline::EndForwardMeshRender()
	{
		//���⼭ CubeMap�� ���� ������Ʈ�� ��, �����Ѵ�!
		//Initialize�� ������ ���ÿ� �Ǿ� �ִ�. �����غ���!
		_currentCubeMap->Render();
	}

	void RenderPipeline::CombineRenderTargets()
	{
		///THIS: 
		///_lowDX->m_d3dMainContext.Get()->RSSetViewports(1, &(_lowDX->m_FullClientViewport));
		
		//DeferredPhong�� ���� ������ ������Ʈ�� ������.
		_lowDX->m_d3dMainContext.Get()->IASetInputLayout(_precombinedShaderInputSet.second.m_Layout);

		_lowDX->m_d3dMainContext.Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
		//���⼭ "�����ִ�" �۾��� �����ؾ� �Ѵ�.
		ID3D11RenderTargetView*& tCombined = _lowDX->_combinedFullScreenQuad->_defTextureRenderTarget._renderTargetView;
		ID3D11RenderTargetView* tMainP = _lowDX->m_MainRenderTargetView.Get();
		ID3D11RenderTargetView** tMainPP = _lowDX->m_MainRenderTargetView.GetAddressOf();
		//_lowDX->m_d3dMainContext.Get()->OMSetRenderTargets(1, &tCombined, nullptr);
		_lowDX->m_d3dMainContext.Get()->OMSetRenderTargets(1, tMainPP, nullptr);

		///BlendState ����
		//_lowDX->m_d3dMainContext.Get()->OMSetBlendState(_lowDX->m_AdditiveBlend.Get(), nullptr, 0xFFFFFFFF);

		auto transparent = XMFLOAT4(0.0f, 1.0f, 1.0f, 0.0f); ///�� ���� ������ �߿��ϴ�. Depth�� ����Ϸ���!

		//_lowDX->m_d3dMainContext.Get()->ClearRenderTargetView(tCombined, reinterpret_cast<const float*>(&transparent));
		_lowDX->m_d3dMainContext.Get()->ClearRenderTargetView(tMainP, reinterpret_cast<const float*>(&transparent));
		//_lowDX->m_d3dMainContext.Get()->ClearDepthStencilView(_lowDX->m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

		//DeferredPhong, Forward�� SRV�� �����´�.
		ID3D11ShaderResourceView*& tForwardQuadSRV = _lowDX->_forwardFullScreenQuad->_defTextureRenderTarget._shaderResourceView;
		ID3D11ShaderResourceView*& tForwardDepthSRV = _lowDX->_dsv_sync_srv;

		ID3D11ShaderResourceView*& tDeferredPhongQuadSRV = _lowDX->_deferredPhongFullScreenQuad->_defTextureRenderTarget._shaderResourceView;
		//RTV ���� ������ �Ұ�. -> SRV�� �ٲپ��.
		ID3D11ShaderResourceView*& tDeferredPhongDepthSRV = _lowDX->_deferredPhongShaderQuad->_phongDepthGBuffer._shaderResourceView;

		ID3D11ShaderResourceView*& tDeferredPBRQuadSRV = _lowDX->_deferredPBRFullScreenQuad->_defTextureRenderTarget._shaderResourceView;
		//RTV ���� ������ �Ұ�. -> SRV�� �ٲپ��.
		ID3D11ShaderResourceView*& tDeferredPBRDepthSRV = _lowDX->_deferredPBRShaderQuad->_pbrDepthGBuffer._shaderResourceView;

		ID3D11ShaderResourceView*& tDeferredPBRSkinnedQuadSRV = _lowDX->_deferredPBRSkinnedFullScreenQuad->_defTextureRenderTarget._shaderResourceView;
		//RTV ���� ������ �Ұ�. -> SRV�� �ٲپ��.
		ID3D11ShaderResourceView*& tDeferredPBRSkinnedDepthSRV = _lowDX->_deferredPBRSkinnedShaderQuad->_pbrDepthGBuffer._shaderResourceView;


		auto& quad = _lowDX->_combinedFullScreenQuad;
		quad->_gNearClipEffectVar->SetFloat(_camData->nearZ);
		quad->_gFarClipEffectVar->SetFloat(_camData->farZ);

		quad->_gForward_QuadBufferEffectVar->SetResource(tForwardQuadSRV);
		quad->_gForward_DepthBufferEffectVar->SetResource(tForwardDepthSRV);
		quad->_gDeferredPhong_QuadBufferEffectVar->SetResource(tDeferredPhongQuadSRV);
		quad->_gDeferredPhong_DepthBufferEffectVar->SetResource(tDeferredPhongDepthSRV);
		quad->_gDeferredPBR_QuadBufferEffectVar->SetResource(tDeferredPBRQuadSRV);
		quad->_gDeferredPBR_DepthBufferEffectVar->SetResource(tDeferredPBRDepthSRV);
		quad->_gDeferredPBR_Skinned_QuadBufferEffectVar->SetResource(tDeferredPBRSkinnedQuadSRV);
		quad->_gDeferredPBR_Skinned_DepthBufferEffectVar->SetResource(tDeferredPBRSkinnedDepthSRV);

		//���� ������ �ݿ��ϴ� �ð��̴�.
		_precombinedShaderInputSet.first->GetShader()->SetMainTechnique("CombineTech");
		_precombinedShaderInputSet.first->GetShader()->
			GetMainTechnique()->GetPassByIndex(0)->Apply(0, _lowDX->m_d3dMainContext.Get());

		static const UINT stride = sizeof(GeometryGenerator::GeomVertex_PosNormalTex);
		static const UINT offset = 0;

		_lowDX->m_d3dMainContext.Get()->IASetVertexBuffers(0, 1, &quad->_vertexBuffer, &stride, &offset);
		_lowDX->m_d3dMainContext.Get()->IASetIndexBuffer(quad->_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		_lowDX->m_d3dMainContext.Get()->DrawIndexed(quad->_indexCount, 0, 0);

		///BlendState ���� ����
		//_lowDX->m_d3dMainContext.Get()->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);

		//���ҽ� �Ҵ� ����.
		quad->_gNearClipEffectVar->SetFloat(0.0f);
		quad->_gFarClipEffectVar->SetFloat(0.0f);

		quad->_gForward_QuadBufferEffectVar->SetResource(nullptr);
		quad->_gForward_DepthBufferEffectVar->SetResource(nullptr);
		quad->_gDeferredPhong_QuadBufferEffectVar->SetResource(nullptr);
		quad->_gDeferredPhong_DepthBufferEffectVar->SetResource(nullptr);
		quad->_gDeferredPBR_QuadBufferEffectVar->SetResource(nullptr);
		quad->_gDeferredPBR_DepthBufferEffectVar->SetResource(nullptr);
		quad->_gDeferredPBR_Skinned_QuadBufferEffectVar->SetResource(nullptr);
		quad->_gDeferredPBR_Skinned_DepthBufferEffectVar->SetResource(nullptr);
	}

	

	void RenderPipeline::BeginDebugRender()
	{
		//_lowDX->m_d3dMainContext.Get()->RSSetViewports(1, &(_lowDX->m_FullClientViewport));
		_lowDX->m_d3dMainContext.Get()->OMSetRenderTargets(1, _lowDX->m_MainRenderTargetView.GetAddressOf(), nullptr);
		_lowDX->m_d3dMainContext.Get()->ClearDepthStencilView(_lowDX->m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void RenderPipeline::EndDebugRender()
	{

	}

	void RenderPipeline::BeginDebugLineRender()
	{
		_lowDX->m_d3dMainContext->OMSetBlendState(_commonEffectState->Opaque(), nullptr, 0xFFFFFFFF);
		_lowDX->m_d3dMainContext->OMSetDepthStencilState(_commonEffectState->DepthNone(), 0);
		_lowDX->m_d3dMainContext->RSSetState(_commonEffectState->CullNone());

		//�Ϻη� 2DBasicEffect�� �ߵ�X.

		_basicEffect->SetWorld(DirectX::XMMatrixIdentity());
		_basicEffect->SetView(_cameraMatrices->View());
		_basicEffect->SetProjection(_cameraMatrices->Proj());

		_basicEffect->Apply(LowLevelDX11::GetInstance()->m_d3dMainContext.Get());

		_lowDX->m_d3dMainContext->IASetInputLayout(_debugLineInputLayout.Get());
		_primitiveBatch->Begin();
	}

	void RenderPipeline::EndDebugLineRender()
	{
		_primitiveBatch->End();
	}

	void RenderPipeline::BeginUIRender()
	{
		//2D��.
		//_spriteBatch->Begin(SpriteSortMode_Deferred, _dxtkCommonStates->AlphaBlend());
		_spriteBatch->Begin(SpriteSortMode_Deferred, _dxtkCommonStates->NonPremultiplied());
		//_spriteBatch->Begin();
	}

	void RenderPipeline::EndUIRender()
	{
		_spriteBatch->End();
	}

	void RenderPipeline::EndTotalRender()
	{
		//����� Forward ���.
		HRESULT hr = S_OK;
		hr = _lowDX->m_swapChain->Present1(0, 0, &(_lowDX->m_dxgiParam));
		assert(hr == S_OK);

		//Added
		_lowDX->m_d3dMainContext.Get()->RSSetState(0);
		ID3D11RenderTargetView* resetRenderTargets[] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
		_lowDX->m_d3dMainContext.Get()->OMSetRenderTargets(_countof(resetRenderTargets), resetRenderTargets, nullptr);
		_lowDX->m_d3dMainContext.Get()->OMSetDepthStencilState(0, 0);
		ID3D11ShaderResourceView* nullSRV[16] = { 0 };
		_lowDX->m_d3dMainContext.Get()->PSSetShaderResources(0, 16, nullSRV);
	}

	void RenderPipeline::InitAxisGridResources()
	{
		tAxis = new Axis(_lowDX->m_d3dDevice.Get(), _lowDX->m_d3dMainContext.Get(), _lowDX->m_SolidRS.Get());
		tGrid = new Grid(_lowDX->m_d3dDevice.Get(), _lowDX->m_d3dMainContext.Get(), _lowDX->m_WireFrameRS.Get());

		tAxis->Initialize();
		tGrid->Initialize();
	}

	void RenderPipeline::InitDebugGeometryResources()
	{
		_debugCube = std::make_unique<BasicPrimitives>(_lowDX->m_d3dMainContext.Get(), eBasicPrimitiveType::CUBE);
		_debugSphere = std::make_unique<BasicPrimitives>(_lowDX->m_d3dMainContext.Get(), eBasicPrimitiveType::SPHERE);
	}

	void RenderPipeline::InitDebugLineResources()
	{
		//DrawLine�� Ȱ��!
		_primitiveBatch = new DirectX::PrimitiveBatch<DirectX::VertexPositionColor>(LowLevelDX11::GetInstance()->m_d3dMainContext.Get());

		_basicEffect = new DirectX::BasicEffect(_lowDX->m_d3dDevice.Get());

		_basicEffect->SetVertexColorEnabled(true);

		void const* shaderByteCode;
		size_t byteCodeLength;

		_basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

		HR(_lowDX->m_d3dDevice.Get()->CreateInputLayout(VertexPositionColor::InputElements,
			VertexPositionColor::InputElementCount,
			shaderByteCode,
			byteCodeLength,
			_debugLineInputLayout.ReleaseAndGetAddressOf()));

		_commonEffectState = std::make_unique<DirectX::CommonStates>(_lowDX->m_d3dDevice.Get());
		_debugLine = std::make_unique<BasicLine>(_cameraMatrices);
	}

	void RenderPipeline::Init2DResources()
	{
		//[TW] ������! 
		//SpriteBatch/Font
		_spriteBatch = new DirectX::SpriteBatch(_lowDX->m_d3dMainContext.Get());

		//2D ���ҽ� ����ϱ� ����! SPRITEFONT_INSTANCE_COUNT��ŭ �ʱ�ȭ�Ǿ� �־��!
		_fontSizeArray = { 10,25,40,55 };

		DirectX::SpriteFont* tSF10 = new DirectX::SpriteFont(_lowDX->m_d3dDevice.Get(), L"../GraphicsResources/SpriteFonts/LineSeedSansKR_KoreanCompatible_10.spritefont");
		DirectX::SpriteFont* tSF25 = new DirectX::SpriteFont(_lowDX->m_d3dDevice.Get(), L"../GraphicsResources/SpriteFonts/LineSeedSansKR_KoreanCompatible_25.spritefont");
		DirectX::SpriteFont* tSF40 = new DirectX::SpriteFont(_lowDX->m_d3dDevice.Get(), L"../GraphicsResources/SpriteFonts/LineSeedSansKR_KoreanCompatible_40.spritefont");
		DirectX::SpriteFont* tSF55 = new DirectX::SpriteFont(_lowDX->m_d3dDevice.Get(), L"../GraphicsResources/SpriteFonts/LineSeedSansKR_KoreanCompatible_55.spritefont");
		//�̻��� �ϵ���� �Ѱ�! (Font ũ��)
		tSF25->SetDefaultCharacter(L'_');
		tSF25->SetLineSpacing(20);

		_spriteFontArray = { tSF10, tSF25, tSF40, tSF55 };
	}

	void RenderPipeline::SetCubeMap(const char* path)
	{
		//���� path�� nullptr�� ���, ����Ʈ ť����� ����Ѵ�.
		if (path == nullptr)
		{
			this->_currentCubeMap = this->_defaultCubeMap;
			return;
		}

		std::string tPathStr = path;

		RenderCubeMap* tFoundCubeMap = ResourceManager::GetInstance().FindCubeMapByFilePath(tPathStr);
		if (tFoundCubeMap == nullptr)
		{
			tFoundCubeMap = ResourceManager::GetInstance().MakeCubeMapFromFilePath(tPathStr,_cameraMatrices);
		}
		assert(tFoundCubeMap != nullptr);

		this->_currentCubeMap = tFoundCubeMap;
	}

	void RenderPipeline::InitDefaultCubemap()
	{
		//std::string tPathStr = "../GraphicsResources/Cubemaps/FixedDefaultSkyboxCubemap230810.dds";
		std::string tPathStr = "../GraphicsResources/Cubemaps/FinalCubemap230819.dds";

		RenderCubeMap* tFoundCubeMap = ResourceManager::GetInstance().FindCubeMapByFilePath(tPathStr);
		if (tFoundCubeMap == nullptr)
		{
			tFoundCubeMap = ResourceManager::GetInstance().MakeCubeMapFromFilePath(tPathStr, _cameraMatrices);
		}
		assert(tFoundCubeMap != nullptr);

		//SetCubemap�� �ߵ��Ǳ� ������, �� ť����� ���� ����Ǵ� Cubemap���� ���� ��.
		this->_defaultCubeMap = tFoundCubeMap;
		this->_currentCubeMap = this->_defaultCubeMap;
	}

	void RenderPipeline::InitFindPreRequiredEffects()
	{
		///eRenderType �ٲ�� ���⵵ enum class ���� �� �ٲ��� �Ѵ�.

		///Deferred Phong.
		//�ϴ� �����ϴ� Deferred Shader �����Ϳ� �ִ��� üũ.
		std::string tPhongDeferredShaderName = "DeferredPhongShader.cso";
		std::string tPhongDeferredShaderPath = "../FX/Default/DeferredPhongShader.cso";

		bool tIsFound = ResourceManager::GetInstance().FoundExistingFXFile(tPhongDeferredShaderName);
		if (!tIsFound)
		{
			//�ȿ� Shader ���� �� Textures �������� �����.
			ResourceManager::GetInstance().AddShaderDataWithResource(tPhongDeferredShaderPath, eRenderType::DEFERRED_PHONG);
		}
		//Shader File �̸����� ã��!
		_phongDeferredShaderInputSet = ResourceManager::GetInstance().FindShaderInputSetByName(tPhongDeferredShaderName);

		///Deferred PBR
		//�ϴ� �����ϴ� Deferred Shader �����Ϳ� �ִ��� üũ.
		std::string tPBRDeferredShaderName = "BasicPBR.cso";
		std::string tPBRDeferredShaderPath = "../FX/Default/BasicPBR.cso";

		bool tIsFoundPBR = ResourceManager::GetInstance().FoundExistingFXFile(tPBRDeferredShaderName);
		if (!tIsFoundPBR)
		{
			//�ȿ� Shader ���� �� Textures �������� �����.
			ResourceManager::GetInstance().AddShaderDataWithResource(tPBRDeferredShaderPath, eRenderType::DEFERRED_PBR);
		}
		//Shader File �̸����� ã��!
		_pbrDeferredShaderInputSet = ResourceManager::GetInstance().FindShaderInputSetByName(tPBRDeferredShaderName);

		///Deferred PBR Skinned
		//�ϴ� �����ϴ� Deferred Shader �����Ϳ� �ִ��� üũ.
		std::string tPBRSkinnedDeferredShaderName = "BasicPBRSkinned.cso";
		std::string tPBRSkinnedDeferredShaderPath = "../FX/Default/BasicPBRSkinned.cso";

		bool tIsFoundPBRSkinned = ResourceManager::GetInstance().FoundExistingFXFile(tPBRSkinnedDeferredShaderName);
		if (!tIsFoundPBRSkinned)
		{
			//�ȿ� Shader ���� �� Textures �������� �����.
			ResourceManager::GetInstance().AddShaderDataWithResource(tPBRSkinnedDeferredShaderPath, eRenderType::DEFERRED_PBR_SKINNED);
		}
		//Shader File �̸����� ã��!
		_pbrSkinnedDeferredShaderInputSet = ResourceManager::GetInstance().FindShaderInputSetByName(tPBRSkinnedDeferredShaderName);

		///Precombined.
		std::string tPrecombinedShaderName = "FinalCombination.cso";
		std::string tPrecombinedShaderPath = "../FX/FinalCombination/FinalCombination.cso";

		bool tIsFoundPC = ResourceManager::GetInstance().FoundExistingFXFile(tPrecombinedShaderName);
		if (!tIsFoundPC)
		{
			//�ȿ� Shader ���� �� Textures �������� �����.
			///����� Phong�� �ƴ�����, ���� �� ���� �����ӿ�ũ�� �����Ű�� ���� ���.
			ResourceManager::GetInstance().AddShaderDataWithResource(tPrecombinedShaderPath, eRenderType::DEFERRED_PHONG);
		}
		//Shader File �̸����� ã��!
		_precombinedShaderInputSet = ResourceManager::GetInstance().FindShaderInputSetByName(tPrecombinedShaderName);
	}

	void RenderPipeline::TriggerRenderParticle(eParticleType type, RocketEngine::RMFLOAT3 worldPos, float seconds)
	{
		//���⼭�� _particleControlTimetable�� ���� �� �ʱ� ��ϸ�, 
		//RenderPipeline���� �� ������ �� �ý����� ������Ʈ�ϴ� ������ �Լ��� ���� ��.

		//���� �ð����� N�� ���ؼ� ���� �ð��� ���Ѵ�.

		////���� �ð� �ޱ�.
		std::chrono::steady_clock::time_point tNow = std::chrono::steady_clock::now();
		////N�� ����.
		int tIntSec = static_cast<int>(1000 * seconds);
		std::chrono::milliseconds tMilliSeconds(tIntSec);
		//Time Point�� N�� �߰�.
		//�׸��� �� ������ �̸� �������ִ� �Լ��� �Ϸ��ϰ�, ������ ���������� �Ѿ��!
		std::chrono::steady_clock::time_point tAfterSecondsTime = tNow + tMilliSeconds;

		DirectX::XMFLOAT3 tWorldPos = MathHelper::RMToXMFloat3(worldPos);
		
		_particleControlTimetable.push_back(std::make_tuple(type, tWorldPos, tAfterSecondsTime));
	}

	void RenderPipeline::UpdateParticleControlLogic()
	{
		auto tNowTime = std::chrono::steady_clock::now();

		//_particleControlTimetable���� �ð� ���� ����, ���߿� ������ Rendering Layer���� ó���� ����.
		using tParticleTuple = std::tuple<eParticleType,
			DirectX::XMFLOAT3, std::chrono::steady_clock::time_point>;

		_particleControlTimetable.erase(
			std::remove_if(_particleControlTimetable.begin(), _particleControlTimetable.end(),
				[tNowTime](tParticleTuple& it) {return std::get<2>(it) <= tNowTime; }), 
			_particleControlTimetable.end());

		//�� �����Ӹ��� �ߵ�, ������ �ִ� �̷� �ð����� ������ �ð��� �� ������ �� Remove.
	}

	

	/*RocketCore::Graphics::eRenderType RenderPipeline::TEMP_DetermineRenderType(const std::string& _fxName)
	{
		///�ϴ� "�ӽ÷�" ���۵����� ������Ʈ�� �����ؾ� �Ѵ�. 
		///�ý��� �󿡼� ���޵� �ٵ�, �� ������ �̷���.
	}*/
	
	


}
