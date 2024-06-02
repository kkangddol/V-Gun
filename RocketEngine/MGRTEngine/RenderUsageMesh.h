#pragma once
namespace RocketCore::Graphics
{
	class RenderUsageMesh
	{
	public:
		RenderUsageMesh();
		unsigned int BaseVertex;  //!< The base vertex of this mesh in the vertices array for the entire model.
		unsigned int BaseIndex;	  //!< The base index of this mesh in the indices array for the entire model. 
		unsigned int NumIndices;  //!< Total number of mesh indices. 
	};
}


