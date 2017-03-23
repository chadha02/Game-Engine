#pragma once
#ifndef VECTOR3_INL_H
# define VECTOR3_INL_H

namespace Engine
{
	inline float Vector3::getx(void) const
	{
		return m_x;
	}

	inline float Vector3::gety(void) const
	{
		return m_y;
	}

	inline float Vector3::getz(void) const
	{
		return m_z;
	}

	inline void Vector3::setx(const float i_x)
	{
		m_x = i_x;
	}
	inline void Vector3::sety(const float i_y)
	{
		m_y = i_y;
	}
	inline void Vector3::setz(const float i_z)
	{
		m_z = i_z;
	}
	inline void Vector3::setvector(const float i_x, const float i_y, const float i_z)
	{
		m_x = i_x;
		m_y = i_y;
		m_z = i_z;
	}
}
#endif

