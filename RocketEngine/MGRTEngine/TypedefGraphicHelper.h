///편한 구현을 위해, using 해놓은 요소들을 한 곳에 묶어놓아서 쓸 수 있도록 하였다.

#pragma once
#include <map>
#include <string>
#include <array>
#include <d3d11.h>
#include "d3dx11effect.h"
#include "SimpleMath.h"
#include "Util.h"
#include "ShaderMaterial.h"
#include "ByteBuffer.hpp"
#include <tuple>
#include <list>
#include <vector>
#include <bitset>
#include "../RocketGraphicsInterface/ModelData.h"

namespace RocketCore::Graphics
{
	struct AssetInputStruct
	{
		ID3D11InputLayout* m_Layout = nullptr;
		size_t m_InputLayoutCount = 0;
		size_t m_InputLayoutSize = 0;
		uint32_t m_Offsets[D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT]{ 0 };
		uint32_t m_Strides[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT]{ 0 };
		//32개까지 Input이 가능하다고 하나, 일단은 Vector로 이름을 받을 것이다.
		std::vector<std::string> m_SemanticNameVector;
		//그 이상은 안들어온다고 한다.
	};

	enum class eShaderVertexSemantic
	{
		_POSITION,
		_NORMAL,
		_TANGENT,
		_BINORMAL,
		_TEXCOORD,
		_BLENDINDICES,
		_BLENDWEIGHT,
		_ERROR //이 값이 나오면 Error이다.
	};

	class ByteStorage
	{
	public:
		void* m_Mem;
		size_t m_SizeInBytes;
	};

	struct MatPropStruct
	{
		MatPropStruct() : m_FloatData(-1.0f), m_DoubleData(-1.0f), m_StringData("NULL Value."), m_IntData(-1)
		{}
		float m_FloatData;
		double m_DoubleData;
		std::string m_StringData;
		int m_IntData;
	};

	using InputLayoutTuple = std::tuple<std::string, int, std::string>;
	using EffectsTuple = std::tuple<ID3DX11EffectVariable*, eShaderVarType, std::pair<int, int>, void*>;
	using ShaderInputSet = std::pair<ShaderMaterial*, AssetInputStruct>;

	//Texture를 활용하기 위해 -> Material에서 경로를 빼오는 것! 미리 여기서 D3D 관련 Texture를 만들기도 한다.
	//현재 쓰이지 않는다! 자동 연동을 꿈꾼다면 해야하지만, 이는 4Q 내부는 아닐 것 같다.
	struct AssetTextureSRV
	{
		std::string type;
		std::string path;
		ID3D11ShaderResourceView* texture;
	};

	class TextureStorage
	{
		//일단은 미래에 어떻게 될지 몰라, 안전장치를 해제했다.
		//SetTextureStorageData에서 현재는 수동으로 정보를 받아오기에, 괜찮을 것.
	public:
		enum : UINT 
		{ MAX_EACH_TEXTURE_COUNT = 10 };

		bool isEmpty = true;

		std::array<Texture*, MAX_EACH_TEXTURE_COUNT> m_DiffuseMapTextures;
		std::array<Texture*, MAX_EACH_TEXTURE_COUNT> m_NormalMapTextures;
		std::array<Texture*, MAX_EACH_TEXTURE_COUNT> m_ArmMapTextures;

		void AssignTextures(ModelData model);

		Texture* GetDiffuseTexture(int index);
		Texture* GetNormalTexture(int index);
		Texture* GetArmTexture(int index);
	};
}
