///InputLayout을 위한 Vertex Struct 등을 만들기 위해, 만든 Helper 클래스이다.

#pragma once
#include <DirectXMath.h>
#include <unordered_map>
#include <map>
#include <string>
#include <dxgiformat.h>
#include <tuple>
#include "TypedefGraphicHelper.h"
#include <d3d11.h>

////Input Layout을 배출해야.
//class ID3D11InputLayout;
//class D3D11_INPUT_ELEMENT_DESC;
struct ID3DX11EffectTechnique;

namespace RocketCore::Graphics
{
	class VertexStructHelper
	{
	public:
		static VertexStructHelper& GetInstance()
		{
			static VertexStructHelper m_Instance;
			return m_Instance;
		}

		//D3D에서 Vertex를 어떻게 받을지, 모드를 세팅해준다.
		void SetInputClassification(const std::string& _ic);

		//처음 버텍스를 만들 때, 리셋 확신을 위해서.
		void InitVertexLayout();
		//한개의 Align된 칸을 넣기. (순서대로 SemanticName, Semantic Index, DXGIformat string)
		void InsertSingleAlign(const InputLayoutTuple& _ilTuple);
		//이제 필요한 요소들이 다 들어온 상태, 내보내라!
		AssetInputStruct FinishInputLayout(ID3DX11EffectTechnique* _tech);

		//Vertex 시맨틱 String을 enum으로.
		RocketCore::Graphics::eShaderVertexSemantic VertexSemanticStringToEnum(const std::string& _semNameStr);

	private:
		//D3D Input Classification.
		unsigned short m_ICMode = 0;
		//일단 Vertex Struct를 받기 위해, 나중에 한번에 리턴.
		std::vector<D3D11_INPUT_ELEMENT_DESC> m_IEDescVec;
		//Semantic Name을 제대로 받기 위해, 따로 저장해서 나중에 합쳐질 예정.
		std::vector<std::string> m_SemanticNameVec;

		//함수 내부 계산에 쓰이는, 현재까지의 ByteOffset.
		unsigned int m_CurrentByteOffset = 0;

		struct InputClassification : public std::map<std::string, int>
		{
			InputClassification()
			{
				//헤더 추가하지 않기 위해서, API에서 각각 Enum의 할당 숫자에 대응된다.
				this->operator[]("D3D11_INPUT_PER_VERTEX_DATA") = 0;
				this->operator[]("D3D11_INPUT_PER_INSTANCE_DATA") = 1;
			}

			~InputClassification()
			{}
		};

		//중첩 클래스 : C#의 ToEnum() 기능을 채우기 위해!
		//std::pair<DXGI_FORMAT, size_t(바이트 사이즈)> e = VertexEnumMap()[str]; //처럼 사용.
		struct VertexEnumMap : public std::unordered_map<std::string, std::pair<DXGI_FORMAT, size_t>>
		{
			VertexEnumMap()
			{
				this->operator[]("DXGI_FORMAT_UNKNOWN") = std::make_pair(DXGI_FORMAT_UNKNOWN, 0);
				this->operator[]("DXGI_FORMAT_R32G32B32A32_TYPELESS") = std::make_pair(DXGI_FORMAT_R32G32B32A32_TYPELESS, 16);
				this->operator[]("DXGI_FORMAT_R32G32B32A32_FLOAT") = std::make_pair(DXGI_FORMAT_R32G32B32A32_FLOAT, 16);
				this->operator[]("DXGI_FORMAT_R32G32B32A32_UINT") = std::make_pair(DXGI_FORMAT_R32G32B32A32_UINT, 16);
				this->operator[]("DXGI_FORMAT_R32G32B32A32_SINT") = std::make_pair(DXGI_FORMAT_R32G32B32A32_SINT, 16);
				this->operator[]("DXGI_FORMAT_R32G32B32_TYPELESS") = std::make_pair(DXGI_FORMAT_R32G32B32_TYPELESS, 12);
				this->operator[]("DXGI_FORMAT_R32G32B32_FLOAT") = std::make_pair(DXGI_FORMAT_R32G32B32_FLOAT, 12);
				this->operator[]("DXGI_FORMAT_R32G32B32_UINT") = std::make_pair(DXGI_FORMAT_R32G32B32_UINT, 12);
				this->operator[]("DXGI_FORMAT_R32G32B32_SINT") = std::make_pair(DXGI_FORMAT_R32G32B32_SINT, 12);
				this->operator[]("DXGI_FORMAT_R16G16B16A16_TYPELESS") = std::make_pair(DXGI_FORMAT_R16G16B16A16_TYPELESS, 8);
				this->operator[]("DXGI_FORMAT_R16G16B16A16_FLOAT") = std::make_pair(DXGI_FORMAT_R16G16B16A16_FLOAT, 8);
				this->operator[]("DXGI_FORMAT_R16G16B16A16_UNORM") = std::make_pair(DXGI_FORMAT_R16G16B16A16_UNORM, 8);
				this->operator[]("DXGI_FORMAT_R16G16B16A16_UINT") = std::make_pair(DXGI_FORMAT_R16G16B16A16_UINT, 8);
				this->operator[]("DXGI_FORMAT_R16G16B16A16_SNORM") = std::make_pair(DXGI_FORMAT_R16G16B16A16_SNORM, 8);
				this->operator[]("DXGI_FORMAT_R16G16B16A16_SINT") = std::make_pair(DXGI_FORMAT_R16G16B16A16_SINT, 8);
				this->operator[]("DXGI_FORMAT_R32G32_TYPELESS") = std::make_pair(DXGI_FORMAT_R32G32_TYPELESS, 8);
				this->operator[]("DXGI_FORMAT_R32G32_FLOAT") = std::make_pair(DXGI_FORMAT_R32G32_FLOAT, 8);
				this->operator[]("DXGI_FORMAT_R32G32_UINT") = std::make_pair(DXGI_FORMAT_R32G32_UINT, 8);
				this->operator[]("DXGI_FORMAT_R32G32_SINT") = std::make_pair(DXGI_FORMAT_R32G32_SINT, 8);
				this->operator[]("DXGI_FORMAT_R32G8X24_TYPELESS") = std::make_pair(DXGI_FORMAT_R32G8X24_TYPELESS, 8);
				this->operator[]("DXGI_FORMAT_D32_FLOAT_S8X24_UINT") = std::make_pair(DXGI_FORMAT_D32_FLOAT_S8X24_UINT, 8);
				this->operator[]("DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS") = std::make_pair(DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS, 8);
				this->operator[]("DXGI_FORMAT_X32_TYPELESS_G8X24_UINT") = std::make_pair(DXGI_FORMAT_X32_TYPELESS_G8X24_UINT, 8);
				this->operator[]("DXGI_FORMAT_R10G10B10A2_TYPELESS") = std::make_pair(DXGI_FORMAT_R10G10B10A2_TYPELESS, 4);
				this->operator[]("DXGI_FORMAT_R10G10B10A2_UNORM") = std::make_pair(DXGI_FORMAT_R10G10B10A2_UNORM, 4);
				this->operator[]("DXGI_FORMAT_R10G10B10A2_UINT") = std::make_pair(DXGI_FORMAT_R10G10B10A2_UINT, 4);
				this->operator[]("DXGI_FORMAT_R11G11B10_FLOAT") = std::make_pair(DXGI_FORMAT_R11G11B10_FLOAT, 4);
				this->operator[]("DXGI_FORMAT_R8G8B8A8_TYPELESS") = std::make_pair(DXGI_FORMAT_R8G8B8A8_TYPELESS, 4);
				this->operator[]("DXGI_FORMAT_R8G8B8A8_UNORM") = std::make_pair(DXGI_FORMAT_R8G8B8A8_UNORM, 4);
				this->operator[]("DXGI_FORMAT_R8G8B8A8_UNORM_SRGB") = std::make_pair(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 4);
				this->operator[]("DXGI_FORMAT_R8G8B8A8_UINT") = std::make_pair(DXGI_FORMAT_R8G8B8A8_UINT, 4);
				this->operator[]("DXGI_FORMAT_R8G8B8A8_SNORM") = std::make_pair(DXGI_FORMAT_R8G8B8A8_SNORM, 4);
				this->operator[]("DXGI_FORMAT_R8G8B8A8_SINT") = std::make_pair(DXGI_FORMAT_R8G8B8A8_SINT, 4);
				this->operator[]("DXGI_FORMAT_R16G16_TYPELESS") = std::make_pair(DXGI_FORMAT_R16G16_TYPELESS, 4);
				this->operator[]("DXGI_FORMAT_R16G16_FLOAT") = std::make_pair(DXGI_FORMAT_R16G16_FLOAT, 4);
				this->operator[]("DXGI_FORMAT_R16G16_UNORM") = std::make_pair(DXGI_FORMAT_R16G16_UNORM, 4);
				this->operator[]("DXGI_FORMAT_R16G16_UINT") = std::make_pair(DXGI_FORMAT_R16G16_UINT, 4);
				this->operator[]("DXGI_FORMAT_R16G16_SNORM") = std::make_pair(DXGI_FORMAT_R16G16_SNORM, 4);
				this->operator[]("DXGI_FORMAT_R16G16_SINT") = std::make_pair(DXGI_FORMAT_R16G16_SINT, 4);
				this->operator[]("DXGI_FORMAT_R32_TYPELESS") = std::make_pair(DXGI_FORMAT_R32_TYPELESS, 4);
				this->operator[]("DXGI_FORMAT_D32_FLOAT") = std::make_pair(DXGI_FORMAT_D32_FLOAT, 4);
				this->operator[]("DXGI_FORMAT_R32_FLOAT") = std::make_pair(DXGI_FORMAT_R32_FLOAT, 4);
				this->operator[]("DXGI_FORMAT_R32_UINT") = std::make_pair(DXGI_FORMAT_R32_UINT, 4);
				this->operator[]("DXGI_FORMAT_R32_SINT") = std::make_pair(DXGI_FORMAT_R32_SINT, 4);
				this->operator[]("DXGI_FORMAT_R24G8_TYPELESS") = std::make_pair(DXGI_FORMAT_R24G8_TYPELESS, 4);
				this->operator[]("DXGI_FORMAT_D24_UNORM_S8_UINT") = std::make_pair(DXGI_FORMAT_D24_UNORM_S8_UINT, 4);
				this->operator[]("DXGI_FORMAT_R24_UNORM_X8_TYPELESS") = std::make_pair(DXGI_FORMAT_R24_UNORM_X8_TYPELESS, 4);
				this->operator[]("DXGI_FORMAT_X24_TYPELESS_G8_UINT ") = std::make_pair(DXGI_FORMAT_X24_TYPELESS_G8_UINT, 4);
				this->operator[]("DXGI_FORMAT_R8G8_TYPELESS") = std::make_pair(DXGI_FORMAT_R8G8_TYPELESS, 2);
				this->operator[]("DXGI_FORMAT_R8G8_UNORM") = std::make_pair(DXGI_FORMAT_R8G8_UNORM, 2);
				this->operator[]("DXGI_FORMAT_R8G8_UINT") = std::make_pair(DXGI_FORMAT_R8G8_UINT, 2);
				this->operator[]("DXGI_FORMAT_R8G8_SNORM") = std::make_pair(DXGI_FORMAT_R8G8_SNORM, 2);
				this->operator[]("DXGI_FORMAT_R8G8_SINT") = std::make_pair(DXGI_FORMAT_R8G8_SINT, 2);
				this->operator[]("DXGI_FORMAT_R16_TYPELESS") = std::make_pair(DXGI_FORMAT_R16_TYPELESS, 2);
				this->operator[]("DXGI_FORMAT_R16_FLOAT") = std::make_pair(DXGI_FORMAT_R16_FLOAT, 2);
				this->operator[]("DXGI_FORMAT_D16_UNORM") = std::make_pair(DXGI_FORMAT_D16_UNORM, 2);
				this->operator[]("DXGI_FORMAT_R16_UNORM") = std::make_pair(DXGI_FORMAT_R16_UNORM, 2);
				this->operator[]("DXGI_FORMAT_R16_UINT") = std::make_pair(DXGI_FORMAT_R16_UINT, 2);
				this->operator[]("DXGI_FORMAT_R16_SNORM") = std::make_pair(DXGI_FORMAT_R16_SNORM, 2);
				this->operator[]("DXGI_FORMAT_R16_SINT") = std::make_pair(DXGI_FORMAT_R16_SINT, 2);
				this->operator[]("DXGI_FORMAT_R8_TYPELESS") = std::make_pair(DXGI_FORMAT_R8_TYPELESS, 1);
				this->operator[]("DXGI_FORMAT_R8_UNORM") = std::make_pair(DXGI_FORMAT_R8_UNORM, 1);
				this->operator[]("DXGI_FORMAT_R8_UINT") = std::make_pair(DXGI_FORMAT_R8_UINT, 1);
				this->operator[]("DXGI_FORMAT_R8_SNORM") = std::make_pair(DXGI_FORMAT_R8_SNORM, 1);
				this->operator[]("DXGI_FORMAT_R8_SINT") = std::make_pair(DXGI_FORMAT_R8_SINT, 1);
				this->operator[]("DXGI_FORMAT_A8_UNORM") = std::make_pair(DXGI_FORMAT_A8_UNORM, 1);
				//this->operator[]("DXGI_FORMAT_R1_UNORM") = std::make_pair(DXGI_FORMAT_R1_UNORM, NULL); //호환되지 않음.
				this->operator[]("DXGI_FORMAT_R9G9B9E5_SHAREDEXP") = std::make_pair(DXGI_FORMAT_R9G9B9E5_SHAREDEXP, 4);
				this->operator[]("DXGI_FORMAT_R8G8_B8G8_UNORM") = std::make_pair(DXGI_FORMAT_R8G8_B8G8_UNORM, 4);
				this->operator[]("DXGI_FORMAT_G8R8_G8B8_UNORM") = std::make_pair(DXGI_FORMAT_G8R8_G8B8_UNORM, 4);

				////텍스쳐 압축 형식 : 추가적인 정리 필요. -> Vertex Struct에 사용되지 않음.
				//this->operator[]("DXGI_FORMAT_BC1_TYPELESS") = std::make_pair(DXGI_FORMAT_BC1_TYPELESS, 0);
				//this->operator[]("DXGI_FORMAT_BC1_UNORM") = std::make_pair(DXGI_FORMAT_BC1_UNORM, 0);
				//this->operator[]("DXGI_FORMAT_BC1_UNORM_SRGB") = std::make_pair(DXGI_FORMAT_BC1_UNORM_SRGB, 0);
				//this->operator[]("DXGI_FORMAT_BC2_TYPELESS") = std::make_pair(DXGI_FORMAT_BC2_TYPELESS, 0);
				//this->operator[]("DXGI_FORMAT_BC2_UNORM") = std::make_pair(DXGI_FORMAT_BC2_UNORM, 0);
				//this->operator[]("DXGI_FORMAT_BC2_UNORM_SRGB") = std::make_pair(DXGI_FORMAT_BC2_UNORM_SRGB, 0);
				//this->operator[]("DXGI_FORMAT_BC3_TYPELESS") = std::make_pair(DXGI_FORMAT_BC3_TYPELESS, 0);
				//this->operator[]("DXGI_FORMAT_BC3_UNORM") = std::make_pair(DXGI_FORMAT_BC3_UNORM, 0);
				//this->operator[]("DXGI_FORMAT_BC3_UNORM_SRGB") = std::make_pair(DXGI_FORMAT_BC3_UNORM_SRGB, 0);
				//this->operator[]("DXGI_FORMAT_BC4_TYPELESS") = std::make_pair(DXGI_FORMAT_BC4_TYPELESS, 0);
				//this->operator[]("DXGI_FORMAT_BC4_UNORM") = std::make_pair(DXGI_FORMAT_BC4_UNORM, 0);
				//this->operator[]("DXGI_FORMAT_BC4_SNORM") = std::make_pair(DXGI_FORMAT_BC4_SNORM, 0);
				//this->operator[]("DXGI_FORMAT_BC5_TYPELESS") = std::make_pair(DXGI_FORMAT_BC5_TYPELESS, 0);
				//this->operator[]("DXGI_FORMAT_BC5_UNORM") = std::make_pair(DXGI_FORMAT_BC5_UNORM, 0);
				//this->operator[]("DXGI_FORMAT_BC5_SNORM") = std::make_pair(DXGI_FORMAT_BC5_SNORM, 0);
				//// >//

				this->operator[]("DXGI_FORMAT_B5G6R5_UNORM") = std::make_pair(DXGI_FORMAT_B5G6R5_UNORM, 2);
				this->operator[]("DXGI_FORMAT_B5G5R5A1_UNORM") = std::make_pair(DXGI_FORMAT_B5G5R5A1_UNORM, 2);
				this->operator[]("DXGI_FORMAT_B8G8R8A8_UNORM") = std::make_pair(DXGI_FORMAT_B8G8R8A8_UNORM, 4);
				this->operator[]("DXGI_FORMAT_B8G8R8X8_UNORM") = std::make_pair(DXGI_FORMAT_B8G8R8X8_UNORM, 4);
				this->operator[]("DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM") = std::make_pair(DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM, 4);;
				this->operator[]("DXGI_FORMAT_B8G8R8A8_TYPELESS") = std::make_pair(DXGI_FORMAT_B8G8R8A8_TYPELESS, 4);;
				this->operator[]("DXGI_FORMAT_B8G8R8A8_UNORM_SRGB") = std::make_pair(DXGI_FORMAT_B8G8R8A8_UNORM_SRGB, 4);;
				this->operator[]("DXGI_FORMAT_B8G8R8X8_TYPELESS") = std::make_pair(DXGI_FORMAT_B8G8R8X8_TYPELESS, 4);;
				this->operator[]("DXGI_FORMAT_B8G8R8X8_UNORM_SRGB") = std::make_pair(DXGI_FORMAT_B8G8R8X8_UNORM_SRGB, 4);;

				////텍스쳐 압축 형식 : 추가적인 정리 필요. -> Vertex Struct에 사용되지 않음.
				//this->operator[]("DXGI_FORMAT_BC6H_TYPELESS") = std::make_pair(DXGI_FORMAT_BC6H_TYPELESS, 0);;
				//this->operator[]("DXGI_FORMAT_BC6H_UF16") = std::make_pair(DXGI_FORMAT_BC6H_UF16, 0);;
				//this->operator[]("DXGI_FORMAT_BC6H_SF16") = std::make_pair(DXGI_FORMAT_BC6H_SF16, 0);;
				//this->operator[]("DXGI_FORMAT_BC7_TYPELESS") = std::make_pair(DXGI_FORMAT_BC7_TYPELESS, 0);;
				//this->operator[]("DXGI_FORMAT_BC7_UNORM") = std::make_pair(DXGI_FORMAT_BC7_UNORM, 0);;
				//this->operator[]("DXGI_FORMAT_BC7_UNORM_SRGB") = std::make_pair(DXGI_FORMAT_BC7_UNORM_SRGB, 0);;
				//// >//

				////비디오 렌더링 관련 YUV 형식: -> Vertex에 사용되지 X.
				//this->operator[]("DXGI_FORMAT_AYUV") = std::make_pair(DXGI_FORMAT_AYUV, 0);
				//this->operator[]("DXGI_FORMAT_Y410") = std::make_pair(DXGI_FORMAT_Y410, 0);
				//this->operator[]("DXGI_FORMAT_Y416") = std::make_pair(DXGI_FORMAT_Y416, 0);
				//this->operator[]("DXGI_FORMAT_NV12") = std::make_pair(DXGI_FORMAT_NV12, 0);
				//this->operator[]("DXGI_FORMAT_P010") = std::make_pair(DXGI_FORMAT_P010, 0);
				//this->operator[]("DXGI_FORMAT_P016") = std::make_pair(DXGI_FORMAT_P016, 0);
				//this->operator[]("DXGI_FORMAT_420_OPAQUE") = std::make_pair(DXGI_FORMAT_420_OPAQUE, 0);
				//this->operator[]("DXGI_FORMAT_YUY2") = std::make_pair(DXGI_FORMAT_YUY2, 0);
				//this->operator[]("DXGI_FORMAT_Y210") = std::make_pair(DXGI_FORMAT_Y210, 0);
				//this->operator[]("DXGI_FORMAT_Y216") = std::make_pair(DXGI_FORMAT_Y216, 0);
				//this->operator[]("DXGI_FORMAT_NV11") = std::make_pair(DXGI_FORMAT_NV11, 0);
				//this->operator[]("DXGI_FORMAT_AI44") = std::make_pair(DXGI_FORMAT_AI44, 0);
				//this->operator[]("DXGI_FORMAT_IA44") = std::make_pair(DXGI_FORMAT_IA44, 0);
				//this->operator[]("DXGI_FORMAT_P8") = std::make_pair(DXGI_FORMAT_P8, 0);
				//this->operator[]("DXGI_FORMAT_A8P8") = std::make_pair(DXGI_FORMAT_A8P8, 0);
				//// >//

				this->operator[]("DXGI_FORMAT_B4G4R4A4_UNORM") = std::make_pair(DXGI_FORMAT_B4G4R4A4_UNORM, 2);
				this->operator[]("DXGI_FORMAT_P208") = std::make_pair(DXGI_FORMAT_P208, 1);
				this->operator[]("DXGI_FORMAT_V208") = std::make_pair(DXGI_FORMAT_V208, 1);
				this->operator[]("DXGI_FORMAT_V408") = std::make_pair(DXGI_FORMAT_V408, 1);

				//
				//this->operator[]("DXGI_FORMAT_SAMPLER_FEEDBACK_MIN_MIP_OPAQUE") = std::make_pair(DXGI_FORMAT_SAMPLER_FEEDBACK_MIN_MIP_OPAQUE, 0);
				//this->operator[]("DXGI_FORMAT_SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE") = std::make_pair(DXGI_FORMAT_SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE, 0);
				//DXGI_FORMAT_FORCE_UINT도 사용되지 않으므로, 무시.
			};
			~VertexEnumMap() {}
		};

		//싱글턴
		VertexStructHelper() {}
		VertexStructHelper(const VertexStructHelper& ref) {}
		VertexStructHelper& operator=(const VertexStructHelper& ref) {}
		~VertexStructHelper() {}
	};
}


