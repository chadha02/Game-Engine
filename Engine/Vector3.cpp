#include "Vector3.h"
namespace Engine
{
	bool Vector3:: operator==(const Vector3 & rhs)
	{
		if (m_x == rhs.m_x && m_y == rhs.m_y && m_z == rhs.m_z)
			return true;
		else
			return false;
	}
	bool Vector3:: operator!=(const Vector3 & rhs)
	{
		if (m_x != rhs.m_x || m_y != rhs.m_y || m_z != rhs.m_z)
			return true;
		else
			return false;
	}

	Vector3 Vector3 :: operator +(const Vector3 &c1)
	{

		this->m_x = this->m_x + c1.m_x;
		this->m_y = this->m_y + c1.m_y;
		this->m_z = this->m_z + c1.m_z;

		return *this;
	}
	Vector3 Vector3 :: operator -(const Vector3 &c1)
	{

		m_x = m_x - c1.m_x;
		m_y = m_y - c1.m_y;
		m_z = m_z - c1.m_z;

		return *this;
	}
	Vector3 operator / (const Vector3 v,const float &f)
	{
		return Vector3(v.getx() / f, v.gety() / f, v.getz() / f);
	
	}
	Vector3 operator * (const float &f, const Vector3 v)
	{
		return Vector3(v.getx() * f, v.gety() * f, v.getz() * f);
	}

	Vector3 Vector3::getUnitVector() const
	{
		float vectorMagnitude = sqrt( pow(getx(), 2) + pow(gety(), 2) + pow(getz(), 2)) ;
		 Vector3 unitVector(getx() / vectorMagnitude, gety() / vectorMagnitude, getz() / vectorMagnitude);
		 return unitVector;
	}
}