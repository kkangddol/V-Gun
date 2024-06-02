#pragma once
#include "../RocketGraphicsInterface/IRenderable.h"
#include "../RocketGraphicsInterface/ModelData.h"
#include "../RocketGraphicsInterface/MaterialData.h"
#include "../RocketGraphicsInterface/RenderType.h"
#include <functional>
#include <DirectXMath.h>

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace RocketCore::Graphics
{
	class BaseFormation;
	class ShaderMaterial;
	class ShaderUpdater;
	class CameraMatrices;
	class TextureStorage;
	class SceneConstantsUpdater;

	class BaseRenderable : public IRenderable
	{
	public:
		BaseRenderable(ShaderMaterial* mat, BaseFormation* form, CameraMatrices* cameraMat, 
			TextureStorage* tStorage, eRenderType renderType, 
			DirectX::XMFLOAT3 minBounds, DirectX::XMFLOAT3 maxBounds);
		virtual ~BaseRenderable();
		//IRenderable 상속.
		virtual void Render(const RocketEngine::RMFLOAT4X4& worldTM) override; //3D

		virtual void Render(const RocketEngine::RMFLOAT4X4& worldTM, const LegacyMaterialData& m) override;
		virtual void Render(const RocketEngine::RMFLOAT4X4& worldTM, const PBRMaterialData& m)    override;

		virtual void PlayAnim(AnimData animData) override;

		//개별적인 Initialize 함수 (함수 매핑을 위해)
		void InitializeShaderFunctions(std::string filepath, eRenderType renderType);

		TextureStorage* GetTextureStorage();
		LegacyMaterialData GetLegacyMaterialData();
		PBRMaterialData GetPBRMaterialData();
		ShaderMaterial* GetShaderMaterial();
		CameraMatrices* GetCameraMatrices();
		BaseFormation* GetBaseFormation();

		//진짜 셰이더 업데이트용 코드, ShaderMaterial에 있었으면 안되었다.. ㅠㅠ..
		ShaderUpdater* GetShaderUpdater();

		//셰이더 변수 초기화 / 업데이트에 관련된 함수 세터.
		void SetShaderInitFunction(std::function<void(BaseRenderable*)> param);
		void SetShaderUpdateFunction(std::function<void(const DirectX::XMFLOAT4X4&, double)> param);

		//ShaderMaterial딴에서 Shader Variable을 업데이트하는 것은 맞지만, 매번 받는 것이 아닌,
		//선언된 std::function을 가지고 돌아가야 한다. (Initialize와  (updater의) UpdateShaderVariables 모두!)
		//CustomFX에 의해 Init/배정된 자신의 업데이트 함수를 호출한다.

		void InitializeShaderVariables(BaseRenderable* renderable);
		void UpdateShaderVariables(const DirectX::XMFLOAT4X4& worldTM, double interval);

		std::string _objectName;

		void GetOutMinMaxBounds(DirectX::XMFLOAT3& minF, DirectX::XMFLOAT3& maxF);

	protected:
		ShaderMaterial* _material = nullptr;
		BaseFormation* _formation = nullptr;
		CameraMatrices* _cameraMatrix = nullptr;
		TextureStorage* _textureStorage = nullptr;
		SceneConstantsUpdater* _sceneConstantsUpdater = nullptr;
		eRenderType _renderingType = eRenderType::FORWARD; //Enum을 저장해서 처리.

		ID3D11Device* _dev = nullptr;
		ID3D11DeviceContext* _devcon = nullptr;

		DirectX::XMFLOAT3 _minBoundingBox;
		DirectX::XMFLOAT3 _maxBoundingBox;

		//Render의 오버로드를 위해서, 별도로 Material 정보 저장.
		//따로 셰이더 변수 전달과정에서 전달될 것.
		LegacyMaterialData _legacyMaterialData;
		PBRMaterialData _pbrMaterialData;

		ShaderUpdater* _shaderUpdater = nullptr;

		//오브젝트에 붙을 즉시 실행될 함수들, CustomFX에서 함수는 등록이 되지만,
		//실제 실행은 오브젝트 딴에서 실행된다.
		std::function<void(BaseRenderable*)> _shaderInitFunction;
		std::function<void(const DirectX::XMFLOAT4X4&, double)> _shaderUpdateFunction;

		//같은 오브젝트 : 디퍼드일때만 호출되는 것인데.. 함수가 하나 되어야 한다. 
		//일단 Deferre
	};
}


