#pragma once
#ifndef VECTOR3_H
#define VECTOR3_H

#include <iostream>
namespace Engine
{
	class Vector3
	{
	
		float m_x, m_y, m_z;
	public:
		Vector3() :m_x(0), m_y(0), m_z(0)
		{}

		Vector3(float a, float b, float c) : m_x(a), m_y(b), m_z(c)
		{}

		bool operator == (const Vector3 & rhs);
		bool operator !=(const Vector3 & rhs);
		Vector3 operator +(const Vector3 &c1);
		Vector3 operator -(const Vector3 &c1);
		Vector3 getUnitVector() const;
		

		inline float getx(void) const;
		inline float gety(void) const;
		inline float getz(void) const;

		inline void setx(const float i_x);
		inline void sety(const float i_y);
		inline void setz(const float i_z);
		inline void setvector(const float i_x, const float i_y, const float i_z);

	};
	Vector3 operator /(const Vector3, const float &f);
	Vector3 operator *(const float &f, const Vector3 );
}
#include "Vector3-inl.h"
#endif
