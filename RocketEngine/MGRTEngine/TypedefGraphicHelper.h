///���� ������ ����, using �س��� ��ҵ��� �� ���� ������Ƽ� �� �� �ֵ��� �Ͽ���.

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
		//32������ Input�� �����ϴٰ� �ϳ�, �ϴ��� Vector�� �̸��� ���� ���̴�.
		std::vector<std::string> m_SemanticNameVector;
		//�� �̻��� �ȵ��´ٰ� �Ѵ�.
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
		_ERROR //�� ���� ������ Error�̴�.
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

	//Texture�� Ȱ���ϱ� ���� -> Material���� ��θ� ������ ��! �̸� ���⼭ D3D ���� Texture�� ����⵵ �Ѵ�.
	//���� ������ �ʴ´�! �ڵ� ������ �޲۴ٸ� �ؾ�������, �̴� 4Q ���δ� �ƴ� �� ����.
	struct AssetTextureSRV
	{
		std::string type;
		std::string path;
		ID3D11ShaderResourceView* texture;
	};

	class TextureStorage
	{
		//�ϴ��� �̷��� ��� ���� ����, ������ġ�� �����ߴ�.
		//SetTextureStorageData���� ����� �������� ������ �޾ƿ��⿡, ������ ��.
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
