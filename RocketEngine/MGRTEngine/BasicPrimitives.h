#pragma once
#include "SimpleMath.h"
#include <DirectXMath.h>
#include <GeometricPrimitive.h>
#include "SpriteBatch.h"
#include <memory>

struct ID3D11DeviceContext;

namespace RocketCore::Graphics
{
	enum class eBasicPrimitiveType
	{
		CUBE,
		SPHERE
	};

	//�׳� ���� �������� ����׿����� ȣ���ϴ� �뵵��, ������ ����� �ʿ���� �� �ߴ�.
	class BasicPrimitives
	{
	public:
		BasicPrimitives(ID3D11DeviceContext* devcon, eBasicPrimitiveType primType);
		~BasicPrimitives();

		//nullptr�� wireframe, ������ ������ �״�� ���δ�.
		void Render(const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, bool isWire, float r, float g, float b, float a);
	private:
		eBasicPrimitiveType _primType;
		std::unique_ptr<DirectX::DX11::GeometricPrimitive> _geomShape;
	};
}


