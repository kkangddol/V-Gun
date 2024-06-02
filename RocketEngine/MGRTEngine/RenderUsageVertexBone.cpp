#include "RenderUsageVertexBone.h"
#include <memory.h>
#include <cassert>
#include <limits>
namespace RocketCore::Graphics
{
	RenderUsageVertexBone::RenderUsageVertexBone()
	{
		//아무리 Bone의 영향을 받지 않더라도, 멈추지는 않게 되어 있다.
		memset(IDs, 0, 4 * sizeof(IDs[0]));
		memset(Weights, 0, 4 * sizeof(Weights[0]));
	}

	void RenderUsageVertexBone::AddBoneData(unsigned int BoneID, float Weight)
	{
		for (unsigned int i = 0; i < 4; i++) {

			// Check to see if there are any empty weight values. 
			if (Weights[i] <= std::numeric_limits<float>::epsilon()) {
				// Add ID of bone. 
				IDs[i] = BoneID;

				// Set the vertex weight influence for this bone ID. 
				Weights[i] = Weight;
				return;
			}

		}
		// should never get here - more bones than we have space for
		assert(0);
	}
}