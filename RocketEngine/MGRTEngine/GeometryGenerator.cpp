//***************************************************************************************
// GeometryGenerator.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************


#include "GeometryGenerator.h"
#include "MathHelper.h"

using namespace DirectX;
using namespace DirectX::PackedVector;

namespace RocketCore::Graphics
{

	void GeometryGenerator::CreateBox(float width, float height, float depth, MeshData_PosColor& meshData)
	{
		//
		// Create the vertices.
		//

		GeomVertex_PosColor v[24];

		float w2 = 0.5f * width;
		float h2 = 0.5f * height;
		float d2 = 0.5f * depth;

		// Fill in the front face vertex data.
		v[0] = GeomVertex_PosColor(XMFLOAT3(- w2, -h2, -d2), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
		v[1] = GeomVertex_PosColor(XMFLOAT3(-w2, +h2, -d2), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
		v[2] = GeomVertex_PosColor(XMFLOAT3(+w2, +h2, -d2), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
		v[3] = GeomVertex_PosColor(XMFLOAT3(+w2, -h2, -d2), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));

		// Fill in the back face vertex data.
		v[4] = GeomVertex_PosColor(XMFLOAT3(-w2, -h2, +d2),  XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
		v[5] = GeomVertex_PosColor(XMFLOAT3(+w2, -h2, +d2),  XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
		v[6] = GeomVertex_PosColor(XMFLOAT3(+w2, +h2, +d2),  XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
		v[7] = GeomVertex_PosColor(XMFLOAT3(-w2, +h2, +d2),  XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));

		// Fill in the top face vertex data.
		v[8] = GeomVertex_PosColor(XMFLOAT3(-w2, +h2, -d2),  XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
		v[9] = GeomVertex_PosColor(XMFLOAT3(-w2, +h2, +d2),  XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
		v[10] = GeomVertex_PosColor(XMFLOAT3(+w2, +h2, +d2), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
		v[11] = GeomVertex_PosColor(XMFLOAT3(+w2, +h2, -d2), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));

		// Fill in the bottom face vertex data.
		v[12] = GeomVertex_PosColor(XMFLOAT3(-w2, -h2, -d2),  XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
		v[13] = GeomVertex_PosColor(XMFLOAT3(+w2, -h2, -d2),  XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
		v[14] = GeomVertex_PosColor(XMFLOAT3(+w2, -h2, +d2),  XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
		v[15] = GeomVertex_PosColor(XMFLOAT3(-w2, -h2, +d2),  XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));

		// Fill in the left face vertex data.
		v[16] = GeomVertex_PosColor(XMFLOAT3(-w2, -h2, +d2),  XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
		v[17] = GeomVertex_PosColor(XMFLOAT3(-w2, +h2, +d2),  XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
		v[18] = GeomVertex_PosColor(XMFLOAT3(-w2, +h2, -d2),  XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
		v[19] = GeomVertex_PosColor(XMFLOAT3(-w2, -h2, -d2),  XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));

		// Fill in the right face vertex data.
		v[20] = GeomVertex_PosColor(XMFLOAT3(+w2, -h2, -d2),  XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
		v[21] = GeomVertex_PosColor(XMFLOAT3(+w2, +h2, -d2),  XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
		v[22] = GeomVertex_PosColor(XMFLOAT3(+w2, +h2, +d2),  XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
		v[23] = GeomVertex_PosColor(XMFLOAT3(+w2, -h2, +d2),  XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));

		meshData.Vertices.assign(&v[0], &v[24]);

		//
		// Create the indices.
		//

		UINT i[36];

		// Fill in the front face index data
		i[0] = 0; i[1] = 1; i[2] = 2;
		i[3] = 0; i[4] = 2; i[5] = 3;

		// Fill in the back face index data
		i[6] = 4; i[7] = 5; i[8] = 6;
		i[9] = 4; i[10] = 6; i[11] = 7;

		// Fill in the top face index data
		i[12] = 8; i[13] = 9; i[14] = 10;
		i[15] = 8; i[16] = 10; i[17] = 11;

		// Fill in the bottom face index data
		i[18] = 12; i[19] = 13; i[20] = 14;
		i[21] = 12; i[22] = 14; i[23] = 15;

		// Fill in the left face index data
		i[24] = 16; i[25] = 17; i[26] = 18;
		i[27] = 16; i[28] = 18; i[29] = 19;

		// Fill in the right face index data
		i[30] = 20; i[31] = 21; i[32] = 22;
		i[33] = 20; i[34] = 22; i[35] = 23;

		meshData.Indices.assign(&i[0], &i[36]);
	}

	void GeometryGenerator::CreateSphere(float radius, UINT sliceCount, UINT stackCount, MeshData_PosColor& meshData)
	{
		meshData.Vertices.clear();
		meshData.Indices.clear();

		//
		// Compute the vertices stating at the top pole and moving down the stacks.
		//

		// Poles: note that there will be texture coordinate distortion as there is
		// not a unique point on the texture map to assign to the pole when mapping
		// a rectangular texture onto a sphere.
		GeomVertex_PosColor topVertex(XMFLOAT3(0.0f, +radius, 0.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
		GeomVertex_PosColor bottomVertex(XMFLOAT3(0.0f, -radius, 0.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));

		meshData.Vertices.push_back(topVertex);

		float phiStep = XM_PI / stackCount;
		float thetaStep = 2.0f * XM_PI / sliceCount;

		// Compute vertices for each stack ring (do not count the poles as rings).
		for (UINT i = 1; i <= stackCount - 1; ++i)
		{
			float phi = i * phiStep;

			// Vertices of ring.
			for (UINT j = 0; j <= sliceCount; ++j)
			{
				float theta = j * thetaStep;

				GeomVertex_PosColor v;

				// spherical to cartesian
				v._position.x = radius * sinf(phi) * cosf(theta);
				v._position.y = radius * cosf(phi);
				v._position.z = radius * sinf(phi) * sinf(theta);

				v._color = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);

				meshData.Vertices.push_back(v);
			}
		}

		meshData.Vertices.push_back(bottomVertex);

		//
		// Compute indices for top stack.  The top stack was written first to the vertex buffer
		// and connects the top pole to the first ring.
		//

		for (UINT i = 1; i <= sliceCount; ++i)
		{
			meshData.Indices.push_back(0);
			meshData.Indices.push_back(i + 1);
			meshData.Indices.push_back(i);
		}

		//
		// Compute indices for inner stacks (not connected to poles).
		//

		// Offset the indices to the index of the first vertex in the first ring.
		// This is just skipping the top pole vertex.
		UINT baseIndex = 1;
		UINT ringVertexCount = sliceCount + 1;
		for (UINT i = 0; i < stackCount - 2; ++i)
		{
			for (UINT j = 0; j < sliceCount; ++j)
			{
				meshData.Indices.push_back(baseIndex + i * ringVertexCount + j);
				meshData.Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
				meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

				meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
				meshData.Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
				meshData.Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
			}
		}

		//
		// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
		// and connects the bottom pole to the bottom ring.
		//

		// South pole vertex was added last.
		UINT southPoleIndex = (UINT)meshData.Vertices.size() - 1;

		// Offset the indices to the index of the first vertex in the last ring.
		baseIndex = southPoleIndex - ringVertexCount;

		for (UINT i = 0; i < sliceCount; ++i)
		{
			meshData.Indices.push_back(southPoleIndex);
			meshData.Indices.push_back(baseIndex + i);
			meshData.Indices.push_back(baseIndex + i + 1);
		}
	}

	void GeometryGenerator::CreateCylinder(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, MeshData_PosColor& meshData)
	{
		meshData.Vertices.clear();
		meshData.Indices.clear();

		//
		// Build Stacks.
		// 

		float stackHeight = height / stackCount;

		// Amount to increment radius as we move up each stack level from bottom to top.
		float radiusStep = (topRadius - bottomRadius) / stackCount;

		UINT ringCount = stackCount + 1;

		// Compute vertices for each stack ring starting at the bottom and moving up.
		for (UINT i = 0; i < ringCount; ++i)
		{
			float y = -0.5f * height + i * stackHeight;
			float r = bottomRadius + i * radiusStep;

			// vertices of ring
			float dTheta = 2.0f * XM_PI / sliceCount;
			for (UINT j = 0; j <= sliceCount; ++j)
			{
				GeomVertex_PosColor vertex;

				float c = cosf(j * dTheta);
				float s = sinf(j * dTheta);

				vertex._position = XMFLOAT3(r * c, y, r * s);

				// Cylinder can be parameterized as follows, where we introduce v
				// parameter that goes in the same direction as the v tex-coord
				// so that the bitangent goes in the same direction as the v tex-coord.
				//   Let r0 be the bottom radius and let r1 be the top radius.
				//   y(v) = h - hv for v in [0,1].
				//   r(v) = r1 + (r0-r1)v
				//
				//   x(t, v) = r(v)*cos(t)
				//   y(t, v) = h - hv
				//   z(t, v) = r(v)*sin(t)
				// 
				//  dx/dt = -r(v)*sin(t)
				//  dy/dt = 0
				//  dz/dt = +r(v)*cos(t)
				//
				//  dx/dv = (r0-r1)*cos(t)
				//  dy/dv = -h
				//  dz/dv = (r0-r1)*sin(t)

				// This is unit length.
				
				vertex._color = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
				meshData.Vertices.push_back(vertex);
			}
		}

		// Add one because we duplicate the first and last vertex per ring
		// since the texture coordinates are different.
		UINT ringVertexCount = sliceCount + 1;

		// Compute indices for each stack.
		for (UINT i = 0; i < stackCount; ++i)
		{
			for (UINT j = 0; j < sliceCount; ++j)
			{
				meshData.Indices.push_back(i * ringVertexCount + j);
				meshData.Indices.push_back((i + 1) * ringVertexCount + j);
				meshData.Indices.push_back((i + 1) * ringVertexCount + j + 1);

				meshData.Indices.push_back(i * ringVertexCount + j);
				meshData.Indices.push_back((i + 1) * ringVertexCount + j + 1);
				meshData.Indices.push_back(i * ringVertexCount + j + 1);
			}
		}

		BuildCylinderTopCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData);
		BuildCylinderBottomCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData);
	}

	void GeometryGenerator::BuildCylinderTopCap(float bottomRadius, float topRadius, float height,
		UINT sliceCount, UINT stackCount, MeshData_PosColor& meshData)
	{
		UINT baseIndex = (UINT)meshData.Vertices.size();

		float y = 0.5f * height;
		float dTheta = 2.0f * XM_PI / sliceCount;

		// Duplicate cap ring vertices because the texture coordinates and normals differ.
		for (UINT i = 0; i <= sliceCount; ++i)
		{
			float x = topRadius * cosf(i * dTheta);
			float z = topRadius * sinf(i * dTheta);

			// Scale down by the height to try and make top cap texture coord area
			// proportional to base.
			float u = x / height + 0.5f;
			float v = z / height + 0.5f;

			meshData.Vertices.push_back(GeomVertex_PosColor(XMFLOAT3(x, y, z), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f)));
		}

		// Cap center vertex.
		meshData.Vertices.push_back(GeomVertex_PosColor(XMFLOAT3(0.0f, y, 0.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f)));

		// Index of center vertex.
		UINT centerIndex = (UINT)meshData.Vertices.size() - 1;

		for (UINT i = 0; i < sliceCount; ++i)
		{
			meshData.Indices.push_back(centerIndex);
			meshData.Indices.push_back(baseIndex + i + 1);
			meshData.Indices.push_back(baseIndex + i);
		}
	}

	void GeometryGenerator::BuildCylinderBottomCap(float bottomRadius, float topRadius, float height,
		UINT sliceCount, UINT stackCount, MeshData_PosColor& meshData)
	{
		// 
		// Build bottom cap.
		//

		UINT baseIndex = (UINT)meshData.Vertices.size();
		float y = -0.5f * height;

		// vertices of ring
		float dTheta = 2.0f * XM_PI / sliceCount;
		for (UINT i = 0; i <= sliceCount; ++i)
		{
			float x = bottomRadius * cosf(i * dTheta);
			float z = bottomRadius * sinf(i * dTheta);

			// Scale down by the height to try and make top cap texture coord area
			// proportional to base.
			float u = x / height + 0.5f;
			float v = z / height + 0.5f;

			meshData.Vertices.push_back(GeomVertex_PosColor(XMFLOAT3(x, y, z), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f)));
		}

		// Cap center vertex.
		meshData.Vertices.push_back(GeomVertex_PosColor(XMFLOAT3(0.0f, y, 0.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f)));

		// Cache the index of center vertex.
		UINT centerIndex = (UINT)meshData.Vertices.size() - 1;

		for (UINT i = 0; i < sliceCount; ++i)
		{
			meshData.Indices.push_back(centerIndex);
			meshData.Indices.push_back(baseIndex + i);
			meshData.Indices.push_back(baseIndex + i + 1);
		}
	}

	void GeometryGenerator::CreateGrid(float width, float depth, UINT m, UINT n, MeshData_PosColor& meshData)
	{
		UINT vertexCount = m * n;
		UINT faceCount = (m - 1) * (n - 1) * 2;

		//
		// Create the vertices.
		//

		float halfWidth = 0.5f * width;
		float halfDepth = 0.5f * depth;

		float dx = width / (n - 1);
		float dz = depth / (m - 1);

		float du = 1.0f / (n - 1);
		float dv = 1.0f / (m - 1);

		meshData.Vertices.resize(vertexCount);
		for (UINT i = 0; i < m; ++i)
		{
			float z = halfDepth - i * dz;
			for (UINT j = 0; j < n; ++j)
			{
				float x = -halfWidth + j * dx;

				meshData.Vertices[i * n + j]._position = XMFLOAT3(x, 0.0f, z);
				meshData.Vertices[i * n + j]._color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}

		//
		// Create the indices.
		//

		meshData.Indices.resize(faceCount * 3); // 3 indices per face

		// Iterate over each quad and compute indices.
		UINT k = 0;
		for (UINT i = 0; i < m - 1; ++i)
		{
			for (UINT j = 0; j < n - 1; ++j)
			{
				meshData.Indices[k] = i * n + j;
				meshData.Indices[k + 1] = i * n + j + 1;
				meshData.Indices[k + 2] = (i + 1) * n + j;

				meshData.Indices[k + 3] = (i + 1) * n + j;
				meshData.Indices[k + 4] = i * n + j + 1;
				meshData.Indices[k + 5] = (i + 1) * n + j + 1;

				k += 6; // next quad
			}
		}
	}

	void GeometryGenerator::GenerateFullscreenQuad(MeshData_PosNormalTex& a_meshData)
	{
		a_meshData.Vertices.resize(4);
		a_meshData.Indices.resize(6);

		// Position coordinates specified in NDC space.
		//a_meshData.Vertices[0] = { { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f } };
		//a_meshData.Vertices[1] = { { -1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } };
		//a_meshData.Vertices[2] = { {  1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f } };
		//a_meshData.Vertices[3] = { {  1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f } };

		a_meshData.Vertices[0] = { { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } };
		a_meshData.Vertices[1] = { { -1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } };
		a_meshData.Vertices[2] = { {  1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } };
		a_meshData.Vertices[3] = { {  1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } };

		a_meshData.Indices[0] = 0;
		a_meshData.Indices[1] = 1;
		a_meshData.Indices[2] = 2;

		a_meshData.Indices[3] = 0;
		a_meshData.Indices[4] = 2;
		a_meshData.Indices[5] = 3;
	}

	
	

	

}

