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
///1. 대부분의 애니메이션은 시작-끝이 동일한 프레임으로 끝나는 편이 좋다!
///그리고 nowPlayingAnim은 항상 nullptr가 아니고, 마지막에 MakeAnim이 호출될 떄 설정됨.
///그러니, 절대 쓰러지는 모션과 같은 애를 마지막에 MakeAnim으로 하지 말 것!
///어색해질 것이다.
/// 
///2. SpriteFont의 사이즈 변경은 알아보니 불가능하다. 잘못된 값이 들어가도 깨지지는 않게 세팅을 해놨지만,
/// 나중에 spriteFontArray, Size array 변경이 있어야 할 것!

class DirectX::SpriteBatch;
class DirectX::SpriteFont;
class DirectX::BasicEffect;
class DirectX::CommonStates;

//Render API에 대응.
namespace RocketCore::Graphics
{
	class Axis;
	class Grid;

	//디버깅에 필요한 Primitive들.
	class BasicPrimitives;
	class BasicLine;

	class LowLevelDX11;
	class IRenderer;
	class AssetModelLoader;
	class MGRTScene;
	class ModelData;
	class CameraData;
	class CameraMatrices; //Camera Data를 Graphic Engine에서 받아서 사용.
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

		//라이팅 등 렌더링 영향 미치는 정보를 매개변수에. 
		virtual void SetRenderData(RenderConstantData _rInfo) override;

		///렌더 가능한 오브젝트들을 만든다!
		virtual IRenderable* MakeRenderable(ModelData modelData) override;
		virtual IRenderable* MakeSkinnedRenderable(ModelData modelData) override;

		virtual void MakeAnim(AnimDesc desc, AnimData* data) override;

		virtual ISketchable* MakeText(float fontSize, float width, float height, RocketEngine::RMFLOAT4 color) override;
		virtual ISketchable* MakeImage(const char* imagePath, float width, float height) override;

		//CubeMap Path Set : nullptr면 만들어서 넣는다!
		virtual void SetCubeMap(const char* path) override;

		///렌더되는 오브젝트들을 지운다!
		virtual void DeleteRenderable(IRenderable* instance) override;
		virtual void DeleteSketchable(ISketchable* instance) override;

		virtual void UpdateCamera(const CameraData& cameraData) override;

		///디버그용으로 사용된다!
		virtual void RenderAxisGrid() override;

		virtual void RenderCube(const RocketEngine::RMFLOAT4X4& worldTM, const PrimitiveData& primitiveData) override;
		virtual void RenderCube(const RocketEngine::RMFLOAT4X4& worldTM, float x, float y, float z, const PrimitiveData& primitiveData) override;

		virtual void RenderSphere(RocketEngine::RMFLOAT4X4 worldTM, PrimitiveData primitiveData) override;

		virtual void RenderLine(RocketEngine::RMFLOAT3 firstPos, RocketEngine::RMFLOAT3 secondPos, RocketEngine::RMFLOAT4 color) override;
		// 쏠 수 있어!!! 23.8.7.AJY. 
		virtual void RenderRayLine(float distance, RocketEngine::RMFLOAT4 color) override;
		// 볼 수 있어!!! 23.8.7 KTW.
		virtual void RenderDebugText(float x, float y, const char* contents, float size) override;
		//2D 화면을 기준, LTRB 2D Box 그리기.
		virtual void Render2DBox(RocketEngine::RMFLOAT2 LT, RocketEngine::RMFLOAT2 RB, RocketEngine::RMFLOAT4 color) override;

		// [TW] 230802, 렌더링 과정 개조.
		virtual void BeginTotalRender() override;

		virtual void BeginDeferredPhongMeshRender() override;
		virtual void EndDeferredPhongMeshRender() override;

		virtual void BeginDeferredPBRMeshRender() override;
		virtual void EndDeferredPBRMeshRender() override;

		virtual void BeginDeferredPBRSkinnedMeshRender() override;
		virtual void EndDeferredPBRSkinnedMeshRender() override;

		virtual void BeginForwardMeshRender() override; 
		virtual void EndForwardMeshRender() override;

		virtual void CombineRenderTargets() override; //Forward Rendering + Deferred Rendering 합치기.

		virtual void BeginDebugRender() override;
		virtual void EndDebugRender() override;

		virtual void BeginDebugLineRender() override;
		virtual void EndDebugLineRender() override;

		virtual void BeginUIRender() override;
		virtual void EndUIRender() override;

		virtual void EndTotalRender() override;

		///크기 바꾸기 (D3D DeviceContext 관련 정보 변화 필요!)
		virtual void Resize(int _width, int _height) override; //ccupdagi

		//Finalize
		virtual void Finalize() override;

		///Particle 연동을 위해서 열어놓음.
		virtual void TriggerRenderParticle(eParticleType type, RocketEngine::RMFLOAT3 worldPos, float seconds) override;

	private:
		void InitAxisGridResources();
		void InitDebugGeometryResources();
		void InitDebugLineResources();
		void Init2DResources();
		void InitDefaultCubemap();

		///eRenderType이 바뀌면, 여기서 엄청난 오류가 날 예정이다! 이는 금방 고쳐지는 문제 (enum class 값)
		void InitFindPreRequiredEffects(); //디퍼드 루프에 활용될 ShaderMaterial들 미리 찾아놓기.

		///Particle 관리를 위해.
		void UpdateParticleControlLogic();

		///D3D 연동을 위해.
		//XXX렌더러 같은 거 사용 X, 바로 여기서 발동시키는 구조.
		//현재는 Forward 방식, 나중에 Deferred로 바꿀 것.
		LowLevelDX11* _lowDX = nullptr;

		///실질적인 렌더링 로직 관련
		std::unique_ptr<AssetModelLoader> m_ModelLoader = nullptr;
		CameraMatrices* _cameraMatrices; // 

		//Used For Rendering Synchronization
		RenderConstantData* _renderConstantData = nullptr;
		CameraData* _camData = nullptr;
		RenderCubeMap* _renderCubeMap = nullptr; //현재 사용되는 CubeMap.
		SceneConstantsUpdater* _sceneConstantsUpdater = nullptr;

		///디퍼드 렌더링 : 디퍼드는 픽셀 기반으로 라이팅 등 연산 적용 코드 필요.
		//ResourceManager에서 다 만들어진 다음, 편의를 위해 Find되어 가져와짐.
		ShaderInputSet _phongDeferredShaderInputSet;

		ShaderInputSet _pbrDeferredShaderInputSet;

		ShaderInputSet _pbrSkinnedDeferredShaderInputSet;
		
		//Deferred + Forward 값을 위해 만들어진 ShaderInputSet.
		ShaderInputSet _precombinedShaderInputSet; //얘는 쓴다. 다만, CombinedQuad는 쓰지 않음.
		

		///하드코딩된 Axis & Grid를 위해.
		//Axis & Grid, For Debugging
		Axis* tAxis = nullptr;
		Grid* tGrid = nullptr;

		///Basic Primitive 렌더를 위해.
		std::unique_ptr<BasicPrimitives> _debugCube = nullptr;
		std::unique_ptr<BasicPrimitives> _debugSphere = nullptr;

		///DebugLine을 위한 요구사항.
		DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* _primitiveBatch = nullptr;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> _debugLineInputLayout;
		DirectX::BasicEffect* _basicEffect = nullptr;
		std::unique_ptr<DirectX::CommonStates> _commonEffectState = nullptr; // 디버깅 BasicEffect에 고유한 CommonState.
		std::unique_ptr<DirectX::CommonStates> _dxtkCommonStates; //2D 렌더링에 활용.
		std::unique_ptr<BasicLine> _debugLine = nullptr;

		///

		///2D 리소스들을 위한 요구사항.
		//2D 리소스 렌더링을 하기 위해서 사용해야.
		DirectX::SpriteBatch* _spriteBatch = nullptr;

		//Font 하나로 유지한 다음에 돌려쓸 것이다. 다만, 사이즈는 다르게!
		// 10 / 25 / 40 / 55 
		std::array<DirectX::SpriteFont*, SPRITEFONT_INSTANCE_COUNT> _spriteFontArray;
		std::array<unsigned int, SPRITEFONT_INSTANCE_COUNT> _fontSizeArray;

		//현재 렌더되고 있는 큐브맵 보관.
		RenderCubeMap* _defaultCubeMap = nullptr;
		//디폴트 큐브맵.
		RenderCubeMap* _currentCubeMap = nullptr;

		///파티클을 렌더할 시간/목록 관리.
		//Particle Type, World Pos, Should Close Time 등등을 측정해야 함.
		//TriggerRenderParticles에서 이 값들을 판정해서 넣기.
		std::vector<std::tuple<eParticleType, 
			DirectX::XMFLOAT3, std::chrono::steady_clock::time_point>> _particleControlTimetable;
		
	};

}


