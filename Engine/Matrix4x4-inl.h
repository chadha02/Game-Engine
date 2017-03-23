#pragma once

namespace Engine
{
	inline bool Matrix4x4::IsNAN(const float i_val) const
	{
		return i_val != i_val;
	}
	
	inline Vector4 operator*(const Matrix4x4 & i_mtx, const Vector4 & i_vec)
	{
		return i_mtx.MultiplyRight(i_vec);
	}

	inline Vector4 operator* (const Vector4 & i_vec, const Matrix4x4 & i_mtx)
	{
		return i_mtx.MultiplyLeft(i_vec);
	}

	inline void Matrix4x4::Invert(void)
	{
		assert(!IsNAN(m_11));

		InverseSSE(*this);
	}

	inline Matrix4x4 Matrix4x4::GetInverse(void) const
	{
		assert(!IsNAN(m_11));

		Matrix4x4	Temporary;// (*this);

		return InverseSSE(Temporary);
	}

	inline  float Matrix4x4::deg2rad(const float deg)
	{
		//return (float)(deg* (M_PI / 180));
		static const float pi_on_180 = 4.0f * atan(1.0f) / 180.0f;
		return deg * pi_on_180;
	}

	inline Matrix4x4 Matrix4x4::operator*(const Matrix4x4 & i_other) const
	{
		assert(!IsNAN(m_11));

#if defined(_WIN32)
		// if the matrix we're multiplying against is aligned to a 16 byte address
		// we can use SSE instructions.
		if ((reinterpret_cast<uintptr_t>(&i_other) & (16 - 1)) == 0)
		{
			FastMatrix4x4 Result;
			return MultiplySSE(i_other, Result);
		}
#endif

		Matrix4x4 Result;
		return Multiply(i_other, Result);
	}
}
