#pragma once
#ifndef _BOUNDING_BOX_H_
#define _BOUNDING_BOX_H_

#include "Vector3.h"

namespace Engine
{
	class BoundingBox
	{
		Vector3 extents;
		Vector3 center;

	public:
		BoundingBox()
		{}
		BoundingBox(const Vector3 i_extent, const Vector3 i_center): extents(i_extent), center(i_center)
		{}
		Vector3 GetCenter() const
		{
			return center;
		}
		Vector3 GetExtent() const
		{
			return extents ;
		}
		~BoundingBox()
		{}
	};
}

#endif