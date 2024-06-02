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

	//그냥 렌더 엔진에서 디버그용으로 호출하는 용도라, 별도의 헤더가 필요없을 듯 했다.
	class BasicPrimitives
	{
	public:
		BasicPrimitives(ID3D11DeviceContext* devcon, eBasicPrimitiveType primType);
		~BasicPrimitives();

		//nullptr면 wireframe, 색깔을 넣으면 그대로 보인다.
		void Render(const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, bool isWire, float r, float g, float b, float a);
	private:
		eBasicPrimitiveType _primType;
		std::unique_ptr<DirectX::DX11::GeometricPrimitive> _geomShape;
	};
}


