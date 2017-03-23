#pragma once
#ifndef VECTOR4_H
#define VECTOR4_H

#include "Vector3.h"

namespace Engine
{
	class Vector4
	{
	private:
		float  m_x, m_y, m_z, m_w;
	public:
		Vector4() 
		{}
		Vector4(float a, float b, float c, float d) : m_x(a), m_y(b), m_z(c) , m_w(d)
		{}
		Vector4(Vector3 i_vector3, float d) : m_x(i_vector3.getx()), m_y(i_vector3.gety()), m_z(i_vector3.getz()), m_w(d)
		{}

		bool operator == (const Vector4 & rhs);
		bool operator !=(const Vector4 & rhs);
		Vector4 operator +(const Vector4 &c1);
		Vector4 operator -(const Vector4 &c1);

		void Normalize();
		Vector4 getUnitVector() const;
		
		inline float Length() const;

		static Vector4 zeroVector;

		inline float getx(void) const;
		inline float gety(void) const;
		inline float getz(void) const;
		inline float getw(void) const;

		inline void setx(const float i_x);
		inline void sety(const float i_y);
		inline void setz(const float i_z);
		inline void setw(const float i_z);
		inline Vector3 toVector3();
	};
	inline Vector4 operator /(const Vector4, const float &f);
	inline Vector4 operator *(const float &f, const Vector4);
	inline Vector4 operator *(const Vector4 ,const float &f);
}

#include "Vector4-inl.h"

#endif