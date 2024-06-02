///[TW] 디버그를 위한 수정을 거침.

//***************************************************************************************
// GeometryGenerator.h by Frank Luna (C) 2011 All Rights Reserved.
//   
// Defines a static class for procedurally generating the geometry of 
// common mathematical objects.
//
// All triangles are generated "outward" facing.  If you want "inward" 
// facing triangles (for example, if you want to place the camera inside
// a sphere to simulate a sky), you will need to:
//   1. Change the Direct3D cull mode or manually reverse the winding order.
//   2. Invert the normal.
//   3. Update the texture coordinates and tangent vectors.
//***************************************************************************************

#ifndef GEOMETRYGENERATOR_H
#define GEOMETRYGENERATOR_H

#include "Define3D.h"
namespace RocketCore::Graphics
{
	class GeometryGenerator
	{
	public:
		
		struct GeomVertex_PosColor
		{
			GeomVertex_PosColor() {}
			GeomVertex_PosColor(XMFLOAT3 pos, XMFLOAT4 col)
				: _position(pos), _color(col) {}
			XMFLOAT3 _position;
			XMFLOAT4 _color;
		};

		struct GeomVertex_PosNormalTex
		{
			GeomVertex_PosNormalTex() {}
			GeomVertex_PosNormalTex(XMFLOAT3 pos, XMFLOAT3 norm, XMFLOAT2 tex )
				: _position(pos), _normal(norm), _tex(tex) {}
			XMFLOAT3 _position;
			XMFLOAT3 _normal;
			XMFLOAT2 _tex;
		};

		struct MeshData_PosColor
		{
			std::vector<GeomVertex_PosColor> Vertices;
			std::vector<UINT> Indices;
		};

		//ScreenQuad를 위해 개조.
		struct MeshData_PosNormalTex
		{
			std::vector<GeomVertex_PosNormalTex> Vertices;
			std::vector<UINT> Indices;
		};

		///<summary>
		/// Creates a box centered at the origin with the given dimensions.
		///</summary>
		void CreateBox(float width, float height, float depth, MeshData_PosColor& meshData);

		///<summary>
		/// Creates a sphere centered at the origin with the given radius.  The
		/// slices and stacks parameters control the degree of tessellation.
		///</summary>
		void CreateSphere(float radius, UINT sliceCount, UINT stackCount, MeshData_PosColor& meshData);

		///<summary>
		/// Creates a cylinder parallel to the y-axis, and centered about the origin.  
		/// The bottom and top radius can vary to form various cone shapes rather than true
		// cylinders.  The slices and stacks parameters control the degree of tessellation.
		///</summary>
		void CreateCylinder(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, MeshData_PosColor& meshData);

		///<summary>
		/// Creates an mxn grid in the xz-plane with m rows and n columns, centered
		/// at the origin with the specified width and depth.
		///</summary>
		void CreateGrid(float width, float depth, UINT m, UINT n, MeshData_PosColor& meshData);

		///<summary>
		/// Creates a quad covering the screen in NDC coordinates.  This is useful for
		/// postprocessing effects.
		///</summary>
		void GenerateFullscreenQuad(MeshData_PosNormalTex& a_meshData); //[TW] Deferred Rendering 위해 개조.

	private:
		void BuildCylinderTopCap(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, MeshData_PosColor& meshData);
		void BuildCylinderBottomCap(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, MeshData_PosColor& meshData);
		
	};
}


#endif // GEOMETRYGENERATOR_H