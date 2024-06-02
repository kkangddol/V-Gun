#pragma once
#include <string>
#include <map>
#include <tuple>
#include <vector>
#include "Util.h"
#include "TypedefGraphicHelper.h"

struct ID3DX11Effect;
struct ID3DX11EffectVariable;
struct ID3DX11EffectTechnique;
class ByteStorage;
class ByteBuffer;

namespace RocketCore::Graphics
{
	class Shader
	{
	public:
		//디폴트 셰이더 생성
		Shader();

		//Custom Shader
		Shader(const std::string& _shaderName, //셰이더 이름.
			const std::string& _fileRoute, //파일 루트.
			const std::vector<std::string>& _techMap, //테크닉 이름들을 쫙 담는 벡터.
			const std::map<std::pair<std::string, eShaderVarType>, std::pair<int, int>>& _varMap); //<VariableName, std::pair<IndividualSize, IndividualCnt> 
		
		void SetShaderFileName(std::string _shaderName);
		std::string GetShaderFileName();

		//메인 테크닉을 설정할 수 있다.
		void SetMainTechnique(const std::string& _techStr);
		ID3DX11EffectTechnique* GetMainTechnique();

		//Self : 자기 자신을 업데이트시키는 함수이다.
		void UpdateShader();

		//자기 자신이 생성된 후 -> 자체적으로 EffectVariable들이 내부에 채워짐.
		//이를 기준으로, 값을 Update하는 함수!
		void UpdateSingleVariable(const std::string& _varName, void* _dataMem);

		//Deferred 한정으로 Effect를 가져와서 특정 Variable들을 발동시켜야 될 때가 있다.
		//이를 위해.
		ID3DX11Effect* GetEffect();

	private:
		//셰이더 이름
		std::string m_ShaderName;
		//FX File Path.
		std::string m_FXFilePath;
		//만들었던 Effect 저장.
		ID3DX11Effect* m_Effect = nullptr;

		//String을 받고, 이에 상응하는 타입을 받으면 작동.
		//어차피 타입 자체를 정의하기보다는, 해당 타입이 SetRawValue시 요하는 size만 받아도 문제 없을 것.
		// NameString //	 FXVariable /  EffectVarType				// Single Size(Byte) / Size 개수.	//후에 업데이트될 값의 포인터.
	  //std::map<std::string, std::tuple<ID3DX11EffectVariable*, eShaderVarType, std::pair<int, int>, ByteBufferMGRT*>> m_FXVarSizeMap;
		std::map<std::string, EffectsTuple> m_FXVarSizeMap;
		
		//		TechVarName / FXTechnique
		std::map<std::string, ID3DX11EffectTechnique*> m_FXTechMap;
		//렌더링에 메인으로 활용될 요소.
		ID3DX11EffectTechnique* m_MainTech;

		//내부함수.
		//Effect를 만들어 반환한다.
		ID3DX11Effect* CreateEffect(const std::string& _fileRoute);

		//실제로 셰이더에 들어갈 데이터가 존재하는지를 Check한다.
		//(매번 체크해주면 좋겠지만, 연속적으로 업뎃에 들어가지는 않아도 될 것 같다.
		bool CheckShaderDataExist();
	
	};
}


