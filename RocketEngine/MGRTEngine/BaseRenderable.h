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
		//IRenderable ���.
		virtual void Render(const RocketEngine::RMFLOAT4X4& worldTM) override; //3D

		virtual void Render(const RocketEngine::RMFLOAT4X4& worldTM, const LegacyMaterialData& m) override;
		virtual void Render(const RocketEngine::RMFLOAT4X4& worldTM, const PBRMaterialData& m)    override;

		virtual void PlayAnim(AnimData animData) override;

		//�������� Initialize �Լ� (�Լ� ������ ����)
		void InitializeShaderFunctions(std::string filepath, eRenderType renderType);

		TextureStorage* GetTextureStorage();
		LegacyMaterialData GetLegacyMaterialData();
		PBRMaterialData GetPBRMaterialData();
		ShaderMaterial* GetShaderMaterial();
		CameraMatrices* GetCameraMatrices();
		BaseFormation* GetBaseFormation();

		//��¥ ���̴� ������Ʈ�� �ڵ�, ShaderMaterial�� �־����� �ȵǾ���.. �Ф�..
		ShaderUpdater* GetShaderUpdater();

		//���̴� ���� �ʱ�ȭ / ������Ʈ�� ���õ� �Լ� ����.
		void SetShaderInitFunction(std::function<void(BaseRenderable*)> param);
		void SetShaderUpdateFunction(std::function<void(const DirectX::XMFLOAT4X4&, double)> param);

		//ShaderMaterial������ Shader Variable�� ������Ʈ�ϴ� ���� ������, �Ź� �޴� ���� �ƴ�,
		//����� std::function�� ������ ���ư��� �Ѵ�. (Initialize��  (updater��) UpdateShaderVariables ���!)
		//CustomFX�� ���� Init/������ �ڽ��� ������Ʈ �Լ��� ȣ���Ѵ�.

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
		eRenderType _renderingType = eRenderType::FORWARD; //Enum�� �����ؼ� ó��.

		ID3D11Device* _dev = nullptr;
		ID3D11DeviceContext* _devcon = nullptr;

		DirectX::XMFLOAT3 _minBoundingBox;
		DirectX::XMFLOAT3 _maxBoundingBox;

		//Render�� �����ε带 ���ؼ�, ������ Material ���� ����.
		//���� ���̴� ���� ���ް������� ���޵� ��.
		LegacyMaterialData _legacyMaterialData;
		PBRMaterialData _pbrMaterialData;

		ShaderUpdater* _shaderUpdater = nullptr;

		//������Ʈ�� ���� ��� ����� �Լ���, CustomFX���� �Լ��� ����� ������,
		//���� ������ ������Ʈ ������ ����ȴ�.
		std::function<void(BaseRenderable*)> _shaderInitFunction;
		std::function<void(const DirectX::XMFLOAT4X4&, double)> _shaderUpdateFunction;

		//���� ������Ʈ : ���۵��϶��� ȣ��Ǵ� ���ε�.. �Լ��� �ϳ� �Ǿ�� �Ѵ�. 
		//�ϴ� Deferre
	};
}


