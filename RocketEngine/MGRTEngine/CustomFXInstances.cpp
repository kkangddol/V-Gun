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
		///Formation을 .fx 데이터에 따라서 셋업.
		///1. 커스텀 셰이더를 셋업한다. (디폴트 저장 및 연동은 나중에)
		//일단 만들려고 하는 .fx에 따른 셰이더에 따른 이름. -> 일단 BasicTex.fx를 기본 형태로 잡았다!
		std::vector<std::string> tTechMap = { "TextureTech" }; //Techmap에 대응.

		//FX 내 변수, 형태, 크기, 개수. (VarMap)
		std::map<std::pair<std::string, eShaderVarType>, std::pair<int, int>> tVarMap;
		tVarMap.insert(std::make_pair(std::make_pair("gWorld", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1))); //물론, 크기를 하드코딩해서는 안된다. 나중에는 이를 위한 헬퍼가 있어야 하지만, 일단은.
		tVarMap.insert(std::make_pair(std::make_pair("gWorldInvTranspose", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gWorldViewProj", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));
		//내부 구현이 안 보여서, Shader, ShaderResource는 For문 안에 if를 넣어서 다르게 해줄 것이다. 일단은.
		tVarMap.insert(std::make_pair(std::make_pair("gDiffuseMap", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1))); ///사실 현 시점 코드에서 _SHADER_RESOURCE의 make_pair는 상관없다. 완전히 통일시켜줄 수 있으면 좋을 텐데.

		//이렇게 하면, 이론상 셰이더가 만들어진 것.
		Shader* tShader =
			new Shader(shaderName, fullFilePath, tTechMap, tVarMap);
		tShader->SetMainTechnique("TextureTech");

		//셰이더를 담을 Material을 만듬.
		ShaderMaterial* tMaterial = new ShaderMaterial(tShader);
		//여기서 Initialize를 하는 것이 아니라, 따로 해야 하는데..
		//함수만 배정해놓고, 호출은 나중에 하면 된다!
		
		///이제 더 이상 여기에서 ShaderUpdater를 받지 않는다. InitializeShaderFunctions에서! (BaseRenderable)

		//VertexStructHelper를 통해서 InputLayout을 만든다!
		VertexStructHelper::GetInstance().InitVertexLayout();
		VertexStructHelper::GetInstance().SetInputClassification("D3D11_INPUT_PER_VERTEX_DATA");
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("POSITION", 0, "DXGI_FORMAT_R32G32B32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("TEXCOORD", 0, "DXGI_FORMAT_R32G32_FLOAT"));
		AssetInputStruct tInputStruct = VertexStructHelper::GetInstance().FinishInputLayout(tShader->GetMainTechnique());

		return std::make_pair(tMaterial, tInputStruct);
	}

	std::pair<ShaderMaterial*, RocketCore::Graphics::AssetInputStruct> CustomFXInstances::MakeShaderInput_SkinnedMeshTex(const std::string& fullFilePath, const std::string& shaderName)
	{
		///Formation을 .fx 데이터에 따라서 셋업.
		///1. 커스텀 셰이더를 셋업한다. (디폴트 저장 및 연동은 나중에)
		//일단 만들려고 하는 .fx에 따른 셰이더에 따른 이름. -> 일단 BasicTex.fx를 기본 형태로 잡았다!
		std::vector<std::string> tTechMap = { "Light0Tex" }; //Techmap에 대응.

		//FX 내 변수, 형태, 크기, 개수. (VarMap)
		std::map<std::pair<std::string, eShaderVarType>, std::pair<int, int>> tVarMap;
		tVarMap.insert(std::make_pair(std::make_pair("gWorld", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1))); //물론, 크기를 하드코딩해서는 안된다. 나중에는 이를 위한 헬퍼가 있어야 하지만, 일단은.
		tVarMap.insert(std::make_pair(std::make_pair("gWorldInvTranspose", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gWorldViewProj", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));

		tVarMap.insert(std::make_pair(std::make_pair("gTexTransform", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));

		tVarMap.insert(std::make_pair(std::make_pair("gDiffuseMap", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1))); ///사실 현 시점 코드에서 _SHADER_RESOURCE의 make_pair는 상관없다. 완전히 통일시켜줄 수 있으면 좋을 텐데.

		tVarMap.insert(std::make_pair(std::make_pair("bones", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 100)));

		//이렇게 하면, 이론상 셰이더가 만들어진 것.
		Shader* tShader =
			new Shader(shaderName, fullFilePath, tTechMap, tVarMap);
		tShader->SetMainTechnique("Light0Tex");

		//셰이더를 담을 Material을 만듬.
		ShaderMaterial* tMaterial = new ShaderMaterial(tShader);

		///더는 여기서 Shader Updater를 호출하지 않는다.

		//VertexStructHelper를 통해서 InputLayout을 만든다!
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
		///Formation을 .fx 데이터에 따라서 셋업.
		///1. 커스텀 셰이더를 셋업한다. (디폴트 저장 및 연동은 나중에)
		std::vector<std::string> tTechMap = { "PosNormalTexTech" }; //Techmap에 대응.

		//FX 내 변수, 형태, 크기, 개수. (VarMap)
		std::map<std::pair<std::string, eShaderVarType>, std::pair<int, int>> tVarMap;
		tVarMap.insert(std::make_pair(std::make_pair("gEyePosW", eShaderVarType::_SCALAR_FLOAT),
			std::make_pair(NULL, 3)));
		tVarMap.insert(std::make_pair(std::make_pair("gWorld", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1))); //물론, 크기를 하드코딩해서는 안된다. 나중에는 이를 위한 헬퍼가 있어야 하지만, 일단은.
		tVarMap.insert(std::make_pair(std::make_pair("gWorldInvTranspose", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gWorldViewProj", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gTexTransform", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));
		//내부 구현이 안 보여서, Shader, ShaderResource는 For문 안에 if를 넣어서 다르게 해줄 것이다. 일단은.
		tVarMap.insert(std::make_pair(std::make_pair("gLegacyMaterial", eShaderVarType::_USER_DEFINED_TYPE),
			std::make_pair(sizeof(LegacyMaterialData), 1))); //커스텀 매터리얼 구조체.. => 실제로 Byte 크기가 상관 있다!
		tVarMap.insert(std::make_pair(std::make_pair("gDiffuseMap", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gNormalMap", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));

		//여기서도 Light 활용을 위해 이미 값을 만들어줘야 한다. 개수 연동은 POSNORMALTEX_MAXIMUM_LIGHT_COUNT이랑 같이!
		tVarMap.insert(std::make_pair(std::make_pair("gLightDataList", eShaderVarType::_USER_DEFINED_TYPE),
			std::make_pair(sizeof(DirectionalLightData), 10))); //커스텀 구조체 -> 실제로 Byte 크기가 상관이 았다.
		tVarMap.insert(std::make_pair(std::make_pair("gDirLightListCount", eShaderVarType::_SCALAR_INT),
			std::make_pair(NULL, 1)));

		//이렇게 하면, 이론상 셰이더가 만들어진 것.
		Shader* tShader =
			new Shader(shaderName, fullFilePath, tTechMap, tVarMap);
		tShader->SetMainTechnique("PosNormalTexTech");

		//셰이더를 담을 Material을 만듬.
		ShaderMaterial* tMaterial = new ShaderMaterial(tShader);
		//여기서 Initialize를 하는 것이 아니라, 따로 해야 하는데..
		//함수만 배정해놓고, 호출은 나중에 하면 된다!

		///더는 여기서 ShaderUpdater 호출하지 않는다.

		//VertexStructHelper를 통해서 InputLayout을 만든다!
		VertexStructHelper::GetInstance().InitVertexLayout();
		VertexStructHelper::GetInstance().SetInputClassification("D3D11_INPUT_PER_VERTEX_DATA");
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("POSITION", 0, "DXGI_FORMAT_R32G32B32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("NORMAL",   0, "DXGI_FORMAT_R32G32B32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("TEXCOORD", 0, "DXGI_FORMAT_R32G32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("TANGENT", 0, "DXGI_FORMAT_R32G32B32_FLOAT")); //이제 이걸로 NormalMapping이 가능.
		AssetInputStruct tInputStruct = VertexStructHelper::GetInstance().FinishInputLayout(tShader->GetMainTechnique());

		return std::make_pair(tMaterial, tInputStruct);
	}

	///Deferred.
	std::pair<ShaderMaterial*, RocketCore::Graphics::AssetInputStruct> CustomFXInstances::MakeShaderInput_DeferredPhongShader(const std::string& fullFilePath, const std::string& shaderName)
	{
		///Formation을 .fx 데이터에 따라서 셋업.
		///1. 커스텀 셰이더를 셋업한다. (디폴트 저장 및 연동은 나중에)
		//개조 중.
		std::vector<std::string> tTechMap = { "BasicTech", "LightingTech" }; //Techmap에 대응.

		//FX 내 변수, 형태, 크기, 개수. (VarMap)
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
		//여기서도 Light 활용을 위해 이미 값을 만들어줘야 한다. 개수 연동은 POSNORMALTEX_MAXIMUM_LIGHT_COUNT이랑 같이!
		tVarMap.insert(std::make_pair(std::make_pair("gLightDataList", eShaderVarType::_USER_DEFINED_TYPE),
			std::make_pair(sizeof(DirectionalLightData), 10))); //커스텀 구조체 -> 실제로 Byte 크기가 상관이 았다.
		tVarMap.insert(std::make_pair(std::make_pair("gDirLightListCount", eShaderVarType::_SCALAR_INT),
			std::make_pair(NULL, 1)));


		tVarMap.insert(std::make_pair(std::make_pair("gWorld", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1))); //물론, 크기를 하드코딩해서는 안된다. 나중에는 이를 위한 헬퍼가 있어야 하지만, 일단은.
		tVarMap.insert(std::make_pair(std::make_pair("gWorldInvTranspose", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gWorldViewProj", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gTexTransform", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));
		//내부 구현이 안 보여서, Shader, ShaderResource는 For문 안에 if를 넣어서 다르게 해줄 것이다. 일단은.
		tVarMap.insert(std::make_pair(std::make_pair("gLegacyMaterial", eShaderVarType::_USER_DEFINED_TYPE),
			std::make_pair(sizeof(LegacyMaterialData), 1))); //커스텀 매터리얼 구조체.. => 실제로 Byte 크기가 상관 있다!
		tVarMap.insert(std::make_pair(std::make_pair("gDiffuseMap", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gNormalMap", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));

		
		///원래는 여기서 LightData, Material을 넣는게 당연히 맞으나, 일단은 Deferred 돌아가는 것 확인하기 위해.

		//이렇게 하면, 이론상 셰이더가 만들어진 것.
		Shader* tShader =
			new Shader(shaderName, fullFilePath, tTechMap, tVarMap);
		tShader->SetMainTechnique("BasicTech");

		//셰이더를 담을 Material을 만듬.
		ShaderMaterial* tMaterial = new ShaderMaterial(tShader);
		//여기서 Initialize를 하는 것이 아니라, 따로 해야 하는데..
		//함수만 배정해놓고, 호출은 나중에 하면 된다!

		///더는 여기서 ShaderUpdater 호출하지 않는다.

		//VertexStructHelper를 통해서 InputLayout을 만든다!
		VertexStructHelper::GetInstance().InitVertexLayout();
		VertexStructHelper::GetInstance().SetInputClassification("D3D11_INPUT_PER_VERTEX_DATA");
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("POSITION", 0, "DXGI_FORMAT_R32G32B32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("NORMAL", 0, "DXGI_FORMAT_R32G32B32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("TEXCOORD", 0, "DXGI_FORMAT_R32G32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("TANGENT", 0, "DXGI_FORMAT_R32G32B32_FLOAT"));
		//일단 normal mapping은 나중에 확인, Deferred 돌아가는 것부터 보자.
		AssetInputStruct tInputStruct = VertexStructHelper::GetInstance().FinishInputLayout(tShader->GetMainTechnique());

		return std::make_pair(tMaterial, tInputStruct);
	}

	std::pair<ShaderMaterial*, RocketCore::Graphics::AssetInputStruct> CustomFXInstances::MakeShaderInput_DeferredPBRShader(const std::string& fullFilePath, const std::string& shaderName)
	{
		std::vector<std::string> tTechMap = { "BasicTech", "LightingTech" }; //Techmap에 대응.

		//FX 내 변수, 형태, 크기, 개수. (VarMap)
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
		//여기서도 Light 활용을 위해 이미 값을 만들어줘야 한다. 개수 연동은 POSNORMALTEX_MAXIMUM_LIGHT_COUNT이랑 같이!
		tVarMap.insert(std::make_pair(std::make_pair("gLightDataList", eShaderVarType::_USER_DEFINED_TYPE),
			std::make_pair(sizeof(DirectionalLightData), 3))); //커스텀 구조체 -> 실제로 Byte 크기가 상관이 았다.
		tVarMap.insert(std::make_pair(std::make_pair("gDirLightListCount", eShaderVarType::_SCALAR_INT),
			std::make_pair(NULL, 1)));

		tVarMap.insert(std::make_pair(std::make_pair("gWorld", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1))); //물론, 크기를 하드코딩해서는 안된다. 나중에는 이를 위한 헬퍼가 있어야 하지만, 일단은.
		tVarMap.insert(std::make_pair(std::make_pair("gWorldInvTranspose", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gTexTransform", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gWorldViewProj", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));
		
		tVarMap.insert(std::make_pair(std::make_pair("gPBRMaterial", eShaderVarType::_USER_DEFINED_TYPE),
			std::make_pair(sizeof(PBRMaterialData), 1))); //커스텀 매터리얼 구조체.. => 실제로 Byte 크기가 상관 있다!
		tVarMap.insert(std::make_pair(std::make_pair("gAlbedoMap", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gARMMap", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gNormalMap", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));

		//이렇게 하면, 이론상 셰이더가 만들어진 것.
		Shader* tShader =
			new Shader(shaderName, fullFilePath, tTechMap, tVarMap);
		tShader->SetMainTechnique("BasicTech");

		//셰이더를 담을 Material을 만듬.
		ShaderMaterial* tMaterial = new ShaderMaterial(tShader);
		//여기서 Initialize를 하는 것이 아니라, 따로 해야 하는데..
		//함수만 배정해놓고, 호출은 나중에 하면 된다!

		///더는 여기서 ShaderUpdater 호출하지 않는다.

		//VertexStructHelper를 통해서 InputLayout을 만든다!
		VertexStructHelper::GetInstance().InitVertexLayout();
		VertexStructHelper::GetInstance().SetInputClassification("D3D11_INPUT_PER_VERTEX_DATA");
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("POSITION", 0, "DXGI_FORMAT_R32G32B32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("NORMAL", 0, "DXGI_FORMAT_R32G32B32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("TEXCOORD", 0, "DXGI_FORMAT_R32G32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("TANGENT", 0, "DXGI_FORMAT_R32G32B32_FLOAT"));
		//일단 normal mapping은 나중에 확인, Deferred 돌아가는 것부터 보자.
		AssetInputStruct tInputStruct = VertexStructHelper::GetInstance().FinishInputLayout(tShader->GetMainTechnique());

		return std::make_pair(tMaterial, tInputStruct);
	}

	std::pair<ShaderMaterial*, RocketCore::Graphics::AssetInputStruct> CustomFXInstances::MakeShaderInput_DeferredSkinnedPBRShader(const std::string& fullFilePath, const std::string& shaderName)
	{
		std::vector<std::string> tTechMap = { "BasicTech", "LightingTech" }; //Techmap에 대응.

		//FX 내 변수, 형태, 크기, 개수. (VarMap)
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
		//여기서도 Light 활용을 위해 이미 값을 만들어줘야 한다. 개수 연동은 DEFERRED_PBR_MAXIMUM_DIRECTIONAL_COUNT이랑 같이!
		tVarMap.insert(std::make_pair(std::make_pair("gLightDataList", eShaderVarType::_USER_DEFINED_TYPE),
			std::make_pair(sizeof(DirectionalLightData), 3))); //커스텀 구조체 -> 실제로 Byte 크기가 상관이 았다.
		tVarMap.insert(std::make_pair(std::make_pair("gDirLightListCount", eShaderVarType::_SCALAR_INT),
			std::make_pair(NULL, 1)));

		tVarMap.insert(std::make_pair(std::make_pair("gWorld", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1))); //물론, 크기를 하드코딩해서는 안된다. 나중에는 이를 위한 헬퍼가 있어야 하지만, 일단은.
		tVarMap.insert(std::make_pair(std::make_pair("gWorldInvTranspose", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gTexTransform", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gWorldViewProj", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 1)));

		tVarMap.insert(std::make_pair(std::make_pair("gPBRMaterial", eShaderVarType::_USER_DEFINED_TYPE),
			std::make_pair(sizeof(PBRMaterialData), 1))); //커스텀 매터리얼 구조체.. => 실제로 Byte 크기가 상관 있다!
		tVarMap.insert(std::make_pair(std::make_pair("gAlbedoMap", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gARMMap", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("gNormalMap", eShaderVarType::_SHADER_RESOURCE),
			std::make_pair(NULL, 1)));
		tVarMap.insert(std::make_pair(std::make_pair("bones", eShaderVarType::_MATRIX),
			std::make_pair(NULL, 100)));

		//이렇게 하면, 이론상 셰이더가 만들어진 것.
		Shader* tShader =
			new Shader(shaderName, fullFilePath, tTechMap, tVarMap);
		tShader->SetMainTechnique("BasicTech");

		//셰이더를 담을 Material을 만듬.
		ShaderMaterial* tMaterial = new ShaderMaterial(tShader);
		//여기서 Initialize를 하는 것이 아니라, 따로 해야 하는데..
		//함수만 배정해놓고, 호출은 나중에 하면 된다!

		///더는 여기서 ShaderUpdater 호출하지 않는다.

		//VertexStructHelper를 통해서 InputLayout을 만든다!
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
		//일단 normal mapping은 나중에 확인, Deferred 돌아가는 것부터 보자.
		AssetInputStruct tInputStruct = VertexStructHelper::GetInstance().FinishInputLayout(tShader->GetMainTechnique());
		return std::make_pair(tMaterial, tInputStruct);
	}

	std::pair<ShaderMaterial*, RocketCore::Graphics::AssetInputStruct> CustomFXInstances::MakeShaderInput_FinalCombinationShader(const std::string& fullFilePath, const std::string& shaderName)
	{
		///Formation을 .fx 데이터에 따라서 셋업.
		///1. 커스텀 셰이더를 셋업한다. (디폴트 저장 및 연동은 나중에)
		std::vector<std::string> tTechMap = { "CombineTech" }; //Techmap에 대응.

		//FX 내 변수, 형태, 크기, 개수. (VarMap)
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


		///원래는 여기서 LightData, Material을 넣는게 당연히 맞으나, 일단은 Differed 돌아가는 것 확인하기 위해.

		//이렇게 하면, 이론상 셰이더가 만들어진 것.
		Shader* tShader =
			new Shader(shaderName, fullFilePath, tTechMap, tVarMap);
		tShader->SetMainTechnique("CombineTech");

		//셰이더를 담을 Material을 만듬.
		ShaderMaterial* tMaterial = new ShaderMaterial(tShader);
		//여기서 Initialize를 하는 것이 아니라, 따로 해야 하는데..
		//함수만 배정해놓고, 호출은 나중에 하면 된다!

		///더는 여기서 ShaderUpdater 호출하지 않는다.

		//VertexStructHelper를 통해서 InputLayout을 만든다!
		VertexStructHelper::GetInstance().InitVertexLayout();
		VertexStructHelper::GetInstance().SetInputClassification("D3D11_INPUT_PER_VERTEX_DATA");
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("POSITION", 0, "DXGI_FORMAT_R32G32B32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("NORMAL", 0, "DXGI_FORMAT_R32G32B32_FLOAT"));
		VertexStructHelper::GetInstance().InsertSingleAlign(std::make_tuple("TEXCOORD", 0, "DXGI_FORMAT_R32G32_FLOAT"));
		//일단 normal mapping은 나중에 확인, Deferred 돌아가는 것부터 보자.
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
