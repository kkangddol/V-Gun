#pragma once
#include "Util.h"
#include "Define3D.h"
#include <memory>
#include <array>
#include <string>
#include <vector>
#include <tuple>
#include <chrono>
#include "PrimitiveBatch.h"
#include "VertexTypes.h"
#include "TypedefGraphicHelper.h"
#include <DirectXMath.h>

#include "../RocketGraphicsInterface/IRocketGraphics.h"
#include "../RocketGraphicsInterface/ISketchable.h"
#include "../RocketGraphicsInterface/IRenderable.h"
#include "../RocketGraphicsInterface/ParticleType.h"

///NOTE TO PROGRAMMERS :
///1. ��κ��� �ִϸ��̼��� ����-���� ������ ���������� ������ ���� ����!
///�׸��� nowPlayingAnim�� �׻� nullptr�� �ƴϰ�, �������� MakeAnim�� ȣ��� �� ������.
///�׷���, ���� �������� ��ǰ� ���� �ָ� �������� MakeAnim���� ���� �� ��!
///������� ���̴�.
/// 
///2. SpriteFont�� ������ ������ �˾ƺ��� �Ұ����ϴ�. �߸��� ���� ���� �������� �ʰ� ������ �س�����,
/// ���߿� spriteFontArray, Size array ������ �־�� �� ��!

class DirectX::SpriteBatch;
class DirectX::SpriteFont;
class DirectX::BasicEffect;
class DirectX::CommonStates;

//Render API�� ����.
namespace RocketCore::Graphics
{
	class Axis;
	class Grid;

	//����뿡 �ʿ��� Primitive��.
	class BasicPrimitives;
	class BasicLine;

	class LowLevelDX11;
	class IRenderer;
	class AssetModelLoader;
	class MGRTScene;
	class ModelData;
	class CameraData;
	class CameraMatrices; //Camera Data�� Graphic Engine���� �޾Ƽ� ���.
	class RenderCubeMap; 
	class SceneConstantsUpdater;
	class ShaderMaterial;

	//RenderSystem != Renderer
	class RenderPipeline : public RocketCore::Graphics::IRocketGraphics
	{
	private:
		enum { SPRITEFONT_INSTANCE_COUNT = 4 };
	public:
		RenderPipeline() = default;
		virtual ~RenderPipeline();

		virtual void Initialize(void* _hWnd, int screenWidth, int screenHeight, bool isEditor) override;

		//������ �� ������ ���� ��ġ�� ������ �Ű�������. 
		virtual void SetRenderData(RenderConstantData _rInfo) override;

		///���� ������ ������Ʈ���� �����!
		virtual IRenderable* MakeRenderable(ModelData modelData) override;
		virtual IRenderable* MakeSkinnedRenderable(ModelData modelData) override;

		virtual void MakeAnim(AnimDesc desc, AnimData* data) override;

		virtual ISketchable* MakeText(float fontSize, float width, float height, RocketEngine::RMFLOAT4 color) override;
		virtual ISketchable* MakeImage(const char* imagePath, float width, float height) override;

		//CubeMap Path Set : nullptr�� ���� �ִ´�!
		virtual void SetCubeMap(const char* path) override;

		///�����Ǵ� ������Ʈ���� �����!
		virtual void DeleteRenderable(IRenderable* instance) override;
		virtual void DeleteSketchable(ISketchable* instance) override;

		virtual void UpdateCamera(const CameraData& cameraData) override;

		///����׿����� ���ȴ�!
		virtual void RenderAxisGrid() override;

		virtual void RenderCube(const RocketEngine::RMFLOAT4X4& worldTM, const PrimitiveData& primitiveData) override;
		virtual void RenderCube(const RocketEngine::RMFLOAT4X4& worldTM, float x, float y, float z, const PrimitiveData& primitiveData) override;

		virtual void RenderSphere(RocketEngine::RMFLOAT4X4 worldTM, PrimitiveData primitiveData) override;

		virtual void RenderLine(RocketEngine::RMFLOAT3 firstPos, RocketEngine::RMFLOAT3 secondPos, RocketEngine::RMFLOAT4 color) override;
		// �� �� �־�!!! 23.8.7.AJY. 
		virtual void RenderRayLine(float distance, RocketEngine::RMFLOAT4 color) override;
		// �� �� �־�!!! 23.8.7 KTW.
		virtual void RenderDebugText(float x, float y, const char* contents, float size) override;
		//2D ȭ���� ����, LTRB 2D Box �׸���.
		virtual void Render2DBox(RocketEngine::RMFLOAT2 LT, RocketEngine::RMFLOAT2 RB, RocketEngine::RMFLOAT4 color) override;

		// [TW] 230802, ������ ���� ����.
		virtual void BeginTotalRender() override;

		virtual void BeginDeferredPhongMeshRender() override;
		virtual void EndDeferredPhongMeshRender() override;

		virtual void BeginDeferredPBRMeshRender() override;
		virtual void EndDeferredPBRMeshRender() override;

		virtual void BeginDeferredPBRSkinnedMeshRender() override;
		virtual void EndDeferredPBRSkinnedMeshRender() override;

		virtual void BeginForwardMeshRender() override; 
		virtual void EndForwardMeshRender() override;

		virtual void CombineRenderTargets() override; //Forward Rendering + Deferred Rendering ��ġ��.

		virtual void BeginDebugRender() override;
		virtual void EndDebugRender() override;

		virtual void BeginDebugLineRender() override;
		virtual void EndDebugLineRender() override;

		virtual void BeginUIRender() override;
		virtual void EndUIRender() override;

		virtual void EndTotalRender() override;

		///ũ�� �ٲٱ� (D3D DeviceContext ���� ���� ��ȭ �ʿ�!)
		virtual void Resize(int _width, int _height) override; //ccupdagi

		//Finalize
		virtual void Finalize() override;

		///Particle ������ ���ؼ� �������.
		virtual void TriggerRenderParticle(eParticleType type, RocketEngine::RMFLOAT3 worldPos, float seconds) override;

	private:
		void InitAxisGridResources();
		void InitDebugGeometryResources();
		void InitDebugLineResources();
		void Init2DResources();
		void InitDefaultCubemap();

		///eRenderType�� �ٲ��, ���⼭ ��û�� ������ �� �����̴�! �̴� �ݹ� �������� ���� (enum class ��)
		void InitFindPreRequiredEffects(); //���۵� ������ Ȱ��� ShaderMaterial�� �̸� ã�Ƴ���.

		///Particle ������ ����.
		void UpdateParticleControlLogic();

		///D3D ������ ����.
		//XXX������ ���� �� ��� X, �ٷ� ���⼭ �ߵ���Ű�� ����.
		//����� Forward ���, ���߿� Deferred�� �ٲ� ��.
		LowLevelDX11* _lowDX = nullptr;

		///�������� ������ ���� ����
		std::unique_ptr<AssetModelLoader> m_ModelLoader = nullptr;
		CameraMatrices* _cameraMatrices; // 

		//Used For Rendering Synchronization
		RenderConstantData* _renderConstantData = nullptr;
		CameraData* _camData = nullptr;
		RenderCubeMap* _renderCubeMap = nullptr; //���� ���Ǵ� CubeMap.
		SceneConstantsUpdater* _sceneConstantsUpdater = nullptr;

		///���۵� ������ : ���۵�� �ȼ� ������� ������ �� ���� ���� �ڵ� �ʿ�.
		//ResourceManager���� �� ������� ����, ���Ǹ� ���� Find�Ǿ� ��������.
		ShaderInputSet _phongDeferredShaderInputSet;

		ShaderInputSet _pbrDeferredShaderInputSet;

		ShaderInputSet _pbrSkinnedDeferredShaderInputSet;
		
		//Deferred + Forward ���� ���� ������� ShaderInputSet.
		ShaderInputSet _precombinedShaderInputSet; //��� ����. �ٸ�, CombinedQuad�� ���� ����.
		

		///�ϵ��ڵ��� Axis & Grid�� ����.
		//Axis & Grid, For Debugging
		Axis* tAxis = nullptr;
		Grid* tGrid = nullptr;

		///Basic Primitive ������ ����.
		std::unique_ptr<BasicPrimitives> _debugCube = nullptr;
		std::unique_ptr<BasicPrimitives> _debugSphere = nullptr;

		///DebugLine�� ���� �䱸����.
		DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* _primitiveBatch = nullptr;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> _debugLineInputLayout;
		DirectX::BasicEffect* _basicEffect = nullptr;
		std::unique_ptr<DirectX::CommonStates> _commonEffectState = nullptr; // ����� BasicEffect�� ������ CommonState.
		std::unique_ptr<DirectX::CommonStates> _dxtkCommonStates; //2D �������� Ȱ��.
		std::unique_ptr<BasicLine> _debugLine = nullptr;

		///

		///2D ���ҽ����� ���� �䱸����.
		//2D ���ҽ� �������� �ϱ� ���ؼ� ����ؾ�.
		DirectX::SpriteBatch* _spriteBatch = nullptr;

		//Font �ϳ��� ������ ������ ������ ���̴�. �ٸ�, ������� �ٸ���!
		// 10 / 25 / 40 / 55 
		std::array<DirectX::SpriteFont*, SPRITEFONT_INSTANCE_COUNT> _spriteFontArray;
		std::array<unsigned int, SPRITEFONT_INSTANCE_COUNT> _fontSizeArray;

		//���� �����ǰ� �ִ� ť��� ����.
		RenderCubeMap* _defaultCubeMap = nullptr;
		//����Ʈ ť���.
		RenderCubeMap* _currentCubeMap = nullptr;

		///��ƼŬ�� ������ �ð�/��� ����.
		//Particle Type, World Pos, Should Close Time ����� �����ؾ� ��.
		//TriggerRenderParticles���� �� ������ �����ؼ� �ֱ�.
		std::vector<std::tuple<eParticleType, 
			DirectX::XMFLOAT3, std::chrono::steady_clock::time_point>> _particleControlTimetable;
		
	};

}


