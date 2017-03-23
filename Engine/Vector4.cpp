#include "Vector4.h"
namespace Engine
{
	Vector4 Vector4::zeroVector(0.0, 0.0, 0.0, 0.0); 
	
	bool Vector4:: operator==(const Vector4 & rhs)
	{
		if (m_x == rhs.m_x && m_y == rhs.m_y && m_z == rhs.m_z && m_w == rhs.m_w)
			return true;
		else
			return false;
	}
	
	bool Vector4:: operator!=(const Vector4 & rhs)
	{
		return !(*this == rhs);
	}


	Vector4 Vector4 :: operator +(const Vector4 &c1)
	{

		this->m_x = this->m_x + c1.m_x;
		this->m_y = this->m_y + c1.m_y;
		this->m_z = this->m_z + c1.m_z;
		this->m_w = this->m_w + c1.m_w;

		return *this;
	}

	Vector4 Vector4 :: operator -(const Vector4 &c1)
	{

		m_x = m_x - c1.m_x;
		m_y = m_y - c1.m_y;
		m_z = m_z - c1.m_z;
		m_w = m_w - c1.m_w;

		return *this;
	}
	
	
	float Vector4::Length() const
	{
		return sqrt((m_x * m_x) + (m_y * m_y) + (m_z * m_z) + (m_w * m_w));
	}
	
	void  Vector4::Normalize()
	{
		float length = Length();
		if (length == 0.0f)
			return;
		float unitFactor = 1.0f / length;
		*this = *this * unitFactor;
	}

	Vector4  Vector4::getUnitVector() const
	{
		float length = Length();
		if (length == 0.0f)
			return zeroVector;
		float unitFactor = 1.0f / length;
		return *this * unitFactor;
	}

	Vector4 operator / (const Vector4 v, const float &f)
	{
		return Vector4(v.getx() / f, v.gety() / f, v.getz() / f , v.getw()/f);

	}

	Vector4 operator * (const float &f, const Vector4 v)
	{
		return Vector4(v.getx() * f, v.gety() * f, v.getz() * f , v.getw() * f);
	}

	Vector4 operator * ( const Vector4 v , const float &f)
	{
		return Vector4( f *v.getx(),  f * v.gety() ,f * v.getz(), f * v.getw());
	}
}