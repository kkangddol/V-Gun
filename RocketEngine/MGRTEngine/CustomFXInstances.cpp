#include "CustomFXInstances.h"
#include "Shader.h"
#include "ShaderUpdater.h"
#include "ShaderMaterial.h"
#include "VertexStructHelper.h"
#include <functional>
#include "../RocketGraphicsInterface/LightData.h"
#include "../RocketGraphicsInterface/MaterialData.h"

namespace RocketCore::Graphics
{
	std::pair<ShaderMaterial*, RocketCore::Graphics::AssetInputStruct> CustomFXInstances::MakeShaderInput_BasicTexFX(const std::string& fullFilePath, const std::string& shaderName)
	{
		///Formation�� .fx �����Ϳ� ���� �¾�.
		///1. Ŀ���� ���̴��� �¾��Ѵ�. (����Ʈ ���� �� ������ ���߿�)
		//�ϴ� ������� �ϴ� .fx�� ���� ���̴��� ���� �̸�. -> �ϴ� BasicTex.fx�� �⺻ ���·� ��Ҵ�!
		std::vector<std::string> tTechMap = { "TextureTech" }; //Techmap�� ����.

		//FX �� ����, ����, ũ��, ����. (VarMap)
		std::map<std::pair<std::string, eShaderVarType>, std::pair<int, int>> tVarMap;
		tVarMap.insert(std::make_pair(std::make_pair("gWorld", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1))); //����, ũ�⸦ �ϵ��ڵ��ؼ��� �ȵȴ�. ���߿��� �̸� ���� ���۰� �־�� ������, �ϴ���.
		tVarMap.insert(std::make_pair(std::make_pair("gWorldInvTranspose", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gWorldViewProj", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));
		//���� ������ �� ������, Shader, ShaderResource�� For�� �ȿ� if�� �־ �ٸ��� ���� ���̴�. �ϴ���.
		tVarMap.insert(std::make_pair(std::make_pair("gDiffuseMap", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1))); ///��� �� ���� �ڵ忡�� _SHADER_RESOURCE�� make_pair�� �������. ������ ���Ͻ����� �� ������ ���� �ٵ�.

		//�̷��� �ϸ�, �̷л� ���̴��� ������� ��.
		Shader* tShader =
			new Shader(shaderName, fullFilePath, tTechMap, tVarMap);
		tShader->SetMainTechnique("TextureTech");

		//���̴��� ���� Material�� ����.
		ShaderMaterial* tMaterial = new ShaderMaterial(tShader);
		//���⼭ Initialize�� �ϴ� ���� �ƴ϶�, ���� �ؾ� �ϴµ�..
		//�Լ��� �����س���, ȣ���� ���߿� �ϸ� �ȴ�!
		
		///���� �� �̻� ���⿡�� ShaderUpdater�� ���� �ʴ´�. InitializeShaderFunctions����! (BaseRenderable)

		//VertexStructHelper�� ���ؼ� InputLayout�� �����!
		VertexStructHelper::GetInstance().InitVertexLayout();
		VertexStructHelper::GetInstance().SetInputClassification("D3D11_INPUT_PER_VERTEX_DATA");
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("POSITION", 0, "DXGI_FORMAT_R32G32B32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("TEXCOORD", 0, "DXGI_FORMAT_R32G32_FLOAT"));
		AssetInputStruct tInputStruct = VertexStructHelper::GetInstance().FinishInputLayout(tShader->GetMainTechnique());

		return std::make_pair(tMaterial, tInputStruct);
	}

	std::pair<ShaderMaterial*, RocketCore::Graphics::AssetInputStruct> CustomFXInstances::MakeShaderInput_SkinnedMeshTex(const std::string& fullFilePath, const std::string& shaderName)
	{
		///Formation�� .fx �����Ϳ� ���� �¾�.
		///1. Ŀ���� ���̴��� �¾��Ѵ�. (����Ʈ ���� �� ������ ���߿�)
		//�ϴ� ������� �ϴ� .fx�� ���� ���̴��� ���� �̸�. -> �ϴ� BasicTex.fx�� �⺻ ���·� ��Ҵ�!
		std::vector<std::string> tTechMap = { "Light0Tex" }; //Techmap�� ����.

		//FX �� ����, ����, ũ��, ����. (VarMap)
		std::map<std::pair<std::string, eShaderVarType>, std::pair<int, int>> tVarMap;
		tVarMap.insert(std::make_pair(std::make_pair("gWorld", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1))); //����, ũ�⸦ �ϵ��ڵ��ؼ��� �ȵȴ�. ���߿��� �̸� ���� ���۰� �־�� ������, �ϴ���.
		tVarMap.insert(std::make_pair(std::make_pair("gWorldInvTranspose", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gWorldViewProj", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));

		tVarMap.insert(std::make_pair(std::make_pair("gTexTransform", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));

		tVarMap.insert(std::make_pair(std::make_pair("gDiffuseMap", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1))); ///��� �� ���� �ڵ忡�� _SHADER_RESOURCE�� make_pair�� �������. ������ ���Ͻ����� �� ������ ���� �ٵ�.

		tVarMap.insert(std::make_pair(std::make_pair("bones", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 100)));

		//�̷��� �ϸ�, �̷л� ���̴��� ������� ��.
		Shader* tShader =
			new Shader(shaderName, fullFilePath, tTechMap, tVarMap);
		tShader->SetMainTechnique("Light0Tex");

		//���̴��� ���� Material�� ����.
		ShaderMaterial* tMaterial = new ShaderMaterial(tShader);

		///���� ���⼭ Shader Updater�� ȣ������ �ʴ´�.

		//VertexStructHelper�� ���ؼ� InputLayout�� �����!
		VertexStructHelper::GetInstance().InitVertexLayout();
		VertexStructHelper::GetInstance().SetInputClassification("D3D11_INPUT_PER_VERTEX_DATA");
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("POSITION", 0, "DXGI_FORMAT_R32G32B32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("NORMAL", 0, "DXGI_FORMAT_R32G32B32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("TEXCOORD", 0, "DXGI_FORMAT_R32G32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("BLENDINDICES", 0, "DXGI_FORMAT_R32_UINT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("BLENDINDICES", 1, "DXGI_FORMAT_R32_UINT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("BLENDINDICES", 2, "DXGI_FORMAT_R32_UINT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("BLENDINDICES", 3, "DXGI_FORMAT_R32_UINT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("BLENDWEIGHT", 0, "DXGI_FORMAT_R32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("BLENDWEIGHT", 1, "DXGI_FORMAT_R32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("BLENDWEIGHT", 2, "DXGI_FORMAT_R32_FLOAT"));
		AssetInputStruct tInputStruct = VertexStructHelper::GetInstance().FinishInputLayout(tShader->GetMainTechnique());

		return std::make_pair(tMaterial, tInputStruct);
	}

	std::pair<ShaderMaterial*, RocketCore::Graphics::AssetInputStruct> CustomFXInstances::MakeShaderInput_PosNormalTex(const std::string& fullFilePath, const std::string& shaderName)
	{
		///Formation�� .fx �����Ϳ� ���� �¾�.
		///1. Ŀ���� ���̴��� �¾��Ѵ�. (����Ʈ ���� �� ������ ���߿�)
		std::vector<std::string> tTechMap = { "PosNormalTexTech" }; //Techmap�� ����.

		//FX �� ����, ����, ũ��, ����. (VarMap)
		std::map<std::pair<std::string, eShaderVarType>, std::pair<int, int>> tVarMap;
		tVarMap.insert(std::make_pair(std::make_pair("gEyePosW", eShaderVarType::_SCALAR_FLOAT),
			std::make_pair(NULL, 3)));
		tVarMap.insert(std::make_pair(std::make_pair("gWorld", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1))); //����, ũ�⸦ �ϵ��ڵ��ؼ��� �ȵȴ�. ���߿��� �̸� ���� ���۰� �־�� ������, �ϴ���.
		tVarMap.insert(std::make_pair(std::make_pair("gWorldInvTranspose", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gWorldViewProj", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gTexTransform", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));
		//���� ������ �� ������, Shader, ShaderResource�� For�� �ȿ� if�� �־ �ٸ��� ���� ���̴�. �ϴ���.
		tVarMap.insert(std::make_pair(std::make_pair("gLegacyMaterial", eShaderVarType::_USER_DEFINED_TYPE),
			std::make_pair(sizeof(LegacyMaterialData), 1))); //Ŀ���� ���͸��� ����ü.. => ������ Byte ũ�Ⱑ ��� �ִ�!
		tVarMap.insert(std::make_pair(std::make_pair("gDiffuseMap", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gNormalMap", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));

		//���⼭�� Light Ȱ���� ���� �̹� ���� �������� �Ѵ�. ���� ������ POSNORMALTEX_MAXIMUM_LIGHT_COUNT�̶� ����!
		tVarMap.insert(std::make_pair(std::make_pair("gLightDataList", eShaderVarType::_USER_DEFINED_TYPE),
			std::make_pair(sizeof(DirectionalLightData), 10))); //Ŀ���� ����ü -> ������ Byte ũ�Ⱑ ����� �Ҵ�.
		tVarMap.insert(std::make_pair(std::make_pair("gDirLightListCount", eShaderVarType::_SCALAR_INT),
			std::make_pair(NULL, 1)));

		//�̷��� �ϸ�, �̷л� ���̴��� ������� ��.
		Shader* tShader =
			new Shader(shaderName, fullFilePath, tTechMap, tVarMap);
		tShader->SetMainTechnique("PosNormalTexTech");

		//���̴��� ���� Material�� ����.
		ShaderMaterial* tMaterial = new ShaderMaterial(tShader);
		//���⼭ Initialize�� �ϴ� ���� �ƴ϶�, ���� �ؾ� �ϴµ�..
		//�Լ��� �����س���, ȣ���� ���߿� �ϸ� �ȴ�!

		///���� ���⼭ ShaderUpdater ȣ������ �ʴ´�.

		//VertexStructHelper�� ���ؼ� InputLayout�� �����!
		VertexStructHelper::GetInstance().InitVertexLayout();
		VertexStructHelper::GetInstance().SetInputClassification("D3D11_INPUT_PER_VERTEX_DATA");
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("POSITION", 0, "DXGI_FORMAT_R32G32B32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("NORMAL",   0, "DXGI_FORMAT_R32G32B32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("TEXCOORD", 0, "DXGI_FORMAT_R32G32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("TANGENT", 0, "DXGI_FORMAT_R32G32B32_FLOAT")); //���� �̰ɷ� NormalMapping�� ����.
		AssetInputStruct tInputStruct = VertexStructHelper::GetInstance().FinishInputLayout(tShader->GetMainTechnique());

		return std::make_pair(tMaterial, tInputStruct);
	}

	///Deferred.
	std::pair<ShaderMaterial*, RocketCore::Graphics::AssetInputStruct> CustomFXInstances::MakeShaderInput_DeferredPhongShader(const std::string& fullFilePath, const std::string& shaderName)
	{
		///Formation�� .fx �����Ϳ� ���� �¾�.
		///1. Ŀ���� ���̴��� �¾��Ѵ�. (����Ʈ ���� �� ������ ���߿�)
		//���� ��.
		std::vector<std::string> tTechMap = { "BasicTech", "LightingTech" }; //Techmap�� ����.

		//FX �� ����, ����, ũ��, ����. (VarMap)
		std::map<std::pair<std::string, eShaderVarType>, std::pair<int, int>> tVarMap;
		//G-Buffers
		tVarMap.insert(std::make_pair(std::make_pair("gTexDiffuseSRV_GBuffer", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gTexNormalSRV_GBuffer", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gTexPositionSRV_GBuffer", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gTexDepthSRV_GBuffer", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));

		//Other Variables
		tVarMap.insert(std::make_pair(std::make_pair("gEyePosW", eShaderVarType::_SCALAR_FLOAT),
			std::make_pair(NULL, 3)));
		//���⼭�� Light Ȱ���� ���� �̹� ���� �������� �Ѵ�. ���� ������ POSNORMALTEX_MAXIMUM_LIGHT_COUNT�̶� ����!
		tVarMap.insert(std::make_pair(std::make_pair("gLightDataList", eShaderVarType::_USER_DEFINED_TYPE),
			std::make_pair(sizeof(DirectionalLightData), 10))); //Ŀ���� ����ü -> ������ Byte ũ�Ⱑ ����� �Ҵ�.
		tVarMap.insert(std::make_pair(std::make_pair("gDirLightListCount", eShaderVarType::_SCALAR_INT),
			std::make_pair(NULL, 1)));


		tVarMap.insert(std::make_pair(std::make_pair("gWorld", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1))); //����, ũ�⸦ �ϵ��ڵ��ؼ��� �ȵȴ�. ���߿��� �̸� ���� ���۰� �־�� ������, �ϴ���.
		tVarMap.insert(std::make_pair(std::make_pair("gWorldInvTranspose", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gWorldViewProj", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gTexTransform", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));
		//���� ������ �� ������, Shader, ShaderResource�� For�� �ȿ� if�� �־ �ٸ��� ���� ���̴�. �ϴ���.
		tVarMap.insert(std::make_pair(std::make_pair("gLegacyMaterial", eShaderVarType::_USER_DEFINED_TYPE),
			std::make_pair(sizeof(LegacyMaterialData), 1))); //Ŀ���� ���͸��� ����ü.. => ������ Byte ũ�Ⱑ ��� �ִ�!
		tVarMap.insert(std::make_pair(std::make_pair("gDiffuseMap", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gNormalMap", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));

		
		///������ ���⼭ LightData, Material�� �ִ°� �翬�� ������, �ϴ��� Deferred ���ư��� �� Ȯ���ϱ� ����.

		//�̷��� �ϸ�, �̷л� ���̴��� ������� ��.
		Shader* tShader =
			new Shader(shaderName, fullFilePath, tTechMap, tVarMap);
		tShader->SetMainTechnique("BasicTech");

		//���̴��� ���� Material�� ����.
		ShaderMaterial* tMaterial = new ShaderMaterial(tShader);
		//���⼭ Initialize�� �ϴ� ���� �ƴ϶�, ���� �ؾ� �ϴµ�..
		//�Լ��� �����س���, ȣ���� ���߿� �ϸ� �ȴ�!

		///���� ���⼭ ShaderUpdater ȣ������ �ʴ´�.

		//VertexStructHelper�� ���ؼ� InputLayout�� �����!
		VertexStructHelper::GetInstance().InitVertexLayout();
		VertexStructHelper::GetInstance().SetInputClassification("D3D11_INPUT_PER_VERTEX_DATA");
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("POSITION", 0, "DXGI_FORMAT_R32G32B32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("NORMAL", 0, "DXGI_FORMAT_R32G32B32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("TEXCOORD", 0, "DXGI_FORMAT_R32G32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("TANGENT", 0, "DXGI_FORMAT_R32G32B32_FLOAT"));
		//�ϴ� normal mapping�� ���߿� Ȯ��, Deferred ���ư��� �ͺ��� ����.
		AssetInputStruct tInputStruct = VertexStructHelper::GetInstance().FinishInputLayout(tShader->GetMainTechnique());

		return std::make_pair(tMaterial, tInputStruct);
	}

	std::pair<ShaderMaterial*, RocketCore::Graphics::AssetInputStruct> CustomFXInstances::MakeShaderInput_DeferredPBRShader(const std::string& fullFilePath, const std::string& shaderName)
	{
		std::vector<std::string> tTechMap = { "BasicTech", "LightingTech" }; //Techmap�� ����.

		//FX �� ����, ����, ũ��, ����. (VarMap)
		std::map<std::pair<std::string, eShaderVarType>, std::pair<int, int>> tVarMap;
		//G-Buffers
		tVarMap.insert(std::make_pair(std::make_pair("gTexAlbedoSRV_GBuffer", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gTexNormalSRV_GBuffer", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gTexPositionSRV_GBuffer", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gTexArmSRV_GBuffer", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gTexDepthSRV_GBuffer", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));

		//Other Variables
		tVarMap.insert(std::make_pair(std::make_pair("gEyePosW", eShaderVarType::_SCALAR_FLOAT),
			std::make_pair(NULL, 3)));
		//���⼭�� Light Ȱ���� ���� �̹� ���� �������� �Ѵ�. ���� ������ POSNORMALTEX_MAXIMUM_LIGHT_COUNT�̶� ����!
		tVarMap.insert(std::make_pair(std::make_pair("gLightDataList", eShaderVarType::_USER_DEFINED_TYPE),
			std::make_pair(sizeof(DirectionalLightData), 3))); //Ŀ���� ����ü -> ������ Byte ũ�Ⱑ ����� �Ҵ�.
		tVarMap.insert(std::make_pair(std::make_pair("gDirLightListCount", eShaderVarType::_SCALAR_INT),
			std::make_pair(NULL, 1)));

		tVarMap.insert(std::make_pair(std::make_pair("gWorld", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1))); //����, ũ�⸦ �ϵ��ڵ��ؼ��� �ȵȴ�. ���߿��� �̸� ���� ���۰� �־�� ������, �ϴ���.
		tVarMap.insert(std::make_pair(std::make_pair("gWorldInvTranspose", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gTexTransform", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gWorldViewProj", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));
		
		tVarMap.insert(std::make_pair(std::make_pair("gPBRMaterial", eShaderVarType::_USER_DEFINED_TYPE),
			std::make_pair(sizeof(PBRMaterialData), 1))); //Ŀ���� ���͸��� ����ü.. => ������ Byte ũ�Ⱑ ��� �ִ�!
		tVarMap.insert(std::make_pair(std::make_pair("gAlbedoMap", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gARMMap", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gNormalMap", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));

		//�̷��� �ϸ�, �̷л� ���̴��� ������� ��.
		Shader* tShader =
			new Shader(shaderName, fullFilePath, tTechMap, tVarMap);
		tShader->SetMainTechnique("BasicTech");

		//���̴��� ���� Material�� ����.
		ShaderMaterial* tMaterial = new ShaderMaterial(tShader);
		//���⼭ Initialize�� �ϴ� ���� �ƴ϶�, ���� �ؾ� �ϴµ�..
		//�Լ��� �����س���, ȣ���� ���߿� �ϸ� �ȴ�!

		///���� ���⼭ ShaderUpdater ȣ������ �ʴ´�.

		//VertexStructHelper�� ���ؼ� InputLayout�� �����!
		VertexStructHelper::GetInstance().InitVertexLayout();
		VertexStructHelper::GetInstance().SetInputClassification("D3D11_INPUT_PER_VERTEX_DATA");
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("POSITION", 0, "DXGI_FORMAT_R32G32B32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("NORMAL", 0, "DXGI_FORMAT_R32G32B32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("TEXCOORD", 0, "DXGI_FORMAT_R32G32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("TANGENT", 0, "DXGI_FORMAT_R32G32B32_FLOAT"));
		//�ϴ� normal mapping�� ���߿� Ȯ��, Deferred ���ư��� �ͺ��� ����.
		AssetInputStruct tInputStruct = VertexStructHelper::GetInstance().FinishInputLayout(tShader->GetMainTechnique());

		return std::make_pair(tMaterial, tInputStruct);
	}

	std::pair<ShaderMaterial*, RocketCore::Graphics::AssetInputStruct> CustomFXInstances::MakeShaderInput_DeferredSkinnedPBRShader(const std::string& fullFilePath, const std::string& shaderName)
	{
		std::vector<std::string> tTechMap = { "BasicTech", "LightingTech" }; //Techmap�� ����.

		//FX �� ����, ����, ũ��, ����. (VarMap)
		std::map<std::pair<std::string, eShaderVarType>, std::pair<int, int>> tVarMap;
		//G-Buffers
		tVarMap.insert(std::make_pair(std::make_pair("gTexAlbedoSRV_GBuffer", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gTexNormalSRV_GBuffer", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gTexPositionSRV_GBuffer", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gTexArmSRV_GBuffer", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gTexDepthSRV_GBuffer", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));

		//Other Variables
		tVarMap.insert(std::make_pair(std::make_pair("gEyePosW", eShaderVarType::_SCALAR_FLOAT),
			std::make_pair(NULL, 3)));
		//���⼭�� Light Ȱ���� ���� �̹� ���� �������� �Ѵ�. ���� ������ DEFERRED_PBR_MAXIMUM_DIRECTIONAL_COUNT�̶� ����!
		tVarMap.insert(std::make_pair(std::make_pair("gLightDataList", eShaderVarType::_USER_DEFINED_TYPE),
			std::make_pair(sizeof(DirectionalLightData), 3))); //Ŀ���� ����ü -> ������ Byte ũ�Ⱑ ����� �Ҵ�.
		tVarMap.insert(std::make_pair(std::make_pair("gDirLightListCount", eShaderVarType::_SCALAR_INT),
			std::make_pair(NULL, 1)));

		tVarMap.insert(std::make_pair(std::make_pair("gWorld", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1))); //����, ũ�⸦ �ϵ��ڵ��ؼ��� �ȵȴ�. ���߿��� �̸� ���� ���۰� �־�� ������, �ϴ���.
		tVarMap.insert(std::make_pair(std::make_pair("gWorldInvTranspose", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gTexTransform", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gWorldViewProj", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));

		tVarMap.insert(std::make_pair(std::make_pair("gPBRMaterial", eShaderVarType::_USER_DEFINED_TYPE),
			std::make_pair(sizeof(PBRMaterialData), 1))); //Ŀ���� ���͸��� ����ü.. => ������ Byte ũ�Ⱑ ��� �ִ�!
		tVarMap.insert(std::make_pair(std::make_pair("gAlbedoMap", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gARMMap", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gNormalMap", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("bones", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 100)));

		//�̷��� �ϸ�, �̷л� ���̴��� ������� ��.
		Shader* tShader =
			new Shader(shaderName, fullFilePath, tTechMap, tVarMap);
		tShader->SetMainTechnique("BasicTech");

		//���̴��� ���� Material�� ����.
		ShaderMaterial* tMaterial = new ShaderMaterial(tShader);
		//���⼭ Initialize�� �ϴ� ���� �ƴ϶�, ���� �ؾ� �ϴµ�..
		//�Լ��� �����س���, ȣ���� ���߿� �ϸ� �ȴ�!

		///���� ���⼭ ShaderUpdater ȣ������ �ʴ´�.

		//VertexStructHelper�� ���ؼ� InputLayout�� �����!
		VertexStructHelper::GetInstance().InitVertexLayout();
		VertexStructHelper::GetInstance().SetInputClassification("D3D11_INPUT_PER_VERTEX_DATA");
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("POSITION", 0, "DXGI_FORMAT_R32G32B32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("NORMAL", 0, "DXGI_FORMAT_R32G32B32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("TEXCOORD", 0, "DXGI_FORMAT_R32G32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("TANGENT", 0, "DXGI_FORMAT_R32G32B32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("BLENDINDICES", 0, "DXGI_FORMAT_R32_UINT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("BLENDINDICES", 1, "DXGI_FORMAT_R32_UINT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("BLENDINDICES", 2, "DXGI_FORMAT_R32_UINT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("BLENDINDICES", 3, "DXGI_FORMAT_R32_UINT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("BLENDWEIGHT", 0, "DXGI_FORMAT_R32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("BLENDWEIGHT", 1, "DXGI_FORMAT_R32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("BLENDWEIGHT", 2, "DXGI_FORMAT_R32_FLOAT"));
		//�ϴ� normal mapping�� ���߿� Ȯ��, Deferred ���ư��� �ͺ��� ����.
		AssetInputStruct tInputStruct = VertexStructHelper::GetInstance().FinishInputLayout(tShader->GetMainTechnique());
		return std::make_pair(tMaterial, tInputStruct);
	}

	std::pair<ShaderMaterial*, RocketCore::Graphics::AssetInputStruct> CustomFXInstances::MakeShaderInput_FinalCombinationShader(const std::string& fullFilePath, const std::string& shaderName)
	{
		///Formation�� .fx �����Ϳ� ���� �¾�.
		///1. Ŀ���� ���̴��� �¾��Ѵ�. (����Ʈ ���� �� ������ ���߿�)
		std::vector<std::string> tTechMap = { "CombineTech" }; //Techmap�� ����.

		//FX �� ����, ����, ũ��, ����. (VarMap)
		std::map<std::pair<std::string, eShaderVarType>, std::pair<int, int>> tVarMap;
		tVarMap.insert(std::make_pair(std::make_pair("gForward_QuadBuffer", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gForward_DepthBuffer", eShaderVarType::_DEPTH_STENCIL_VIEW),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gDeferredPhong_QuadBuffer", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gDeferredPhong_DepthBuffer", eShaderVarType::_RENDER_TARGET_VIEW),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gDeferredPBR_QuadBuffer", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gDeferredPBR_DepthBuffer", eShaderVarType::_RENDER_TARGET_VIEW),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gDeferredPBR_Skinned_QuadBuffer", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gDeferredPBR_Skinned_DepthBuffer", eShaderVarType::_RENDER_TARGET_VIEW),
			std::make_pair(NULL, 1)));


		///������ ���⼭ LightData, Material�� �ִ°� �翬�� ������, �ϴ��� Differed ���ư��� �� Ȯ���ϱ� ����.

		//�̷��� �ϸ�, �̷л� ���̴��� ������� ��.
		Shader* tShader =
			new Shader(shaderName, fullFilePath, tTechMap, tVarMap);
		tShader->SetMainTechnique("CombineTech");

		//���̴��� ���� Material�� ����.
		ShaderMaterial* tMaterial = new ShaderMaterial(tShader);
		//���⼭ Initialize�� �ϴ� ���� �ƴ϶�, ���� �ؾ� �ϴµ�..
		//�Լ��� �����س���, ȣ���� ���߿� �ϸ� �ȴ�!

		///���� ���⼭ ShaderUpdater ȣ������ �ʴ´�.

		//VertexStructHelper�� ���ؼ� InputLayout�� �����!
		VertexStructHelper::GetInstance().InitVertexLayout();
		VertexStructHelper::GetInstance().SetInputClassification("D3D11_INPUT_PER_VERTEX_DATA");
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("POSITION", 0, "DXGI_FORMAT_R32G32B32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("NORMAL", 0, "DXGI_FORMAT_R32G32B32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("TEXCOORD", 0, "DXGI_FORMAT_R32G32_FLOAT"));
		//�ϴ� normal mapping�� ���߿� Ȯ��, Deferred ���ư��� �ͺ��� ����.
		AssetInputStruct tInputStruct = VertexStructHelper::GetInstance().FinishInputLayout(tShader->GetMainTechnique());

		return std::make_pair(tMaterial, tInputStruct);
	}

	
	

	//std::pair<ShaderMaterial*, RocketCore::Graphics::AssetInputStruct> CustomFXInstances::MakeShaderInput_PosNormalTex(const std::string& fullFilePath, const std::string& shaderName)
	//{
	//	///HERE.
	//	
	//
	//}

}
