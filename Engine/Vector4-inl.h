#pragma once
#ifndef VECTOR4_INL_H
# define VECTOR4_INL_H
//#include "Vector4.h"

namespace Engine
{
	 
	
	inline float Vector4::getx(void) const
	{
		return m_x;
	}
	inline float Vector4::gety(void) const
	{
		return m_y;
	}

	inline float Vector4::getz(void) const
	{
		return m_z;
	}

	inline float Vector4::getw(void) const
	{
		return m_w;
	}

	inline void Vector4::setx(const float i_x)
	{
		m_x = i_x;
	}
	inline void Vector4::sety(const float i_y)
	{
		m_y = i_y;
	}
	inline void Vector4::setz(const float i_z)
	{
		m_z = i_z;
	}

	inline void Vector4::setw(const float i_w)
	{
		m_w = i_w;
	}
	inline Vector3 Vector4::toVector3()
	{
		return Vector3(m_x, m_y, m_z);
	}
}
#endif