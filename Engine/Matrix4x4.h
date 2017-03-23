#pragma once
#ifndef MATRIX4X4_H
#define MATRIX4X4_H
#define _USE_MATH_DEFINES
#include <cmath>
#include "Vector4.h"
#include <assert.h>


namespace Engine
{
	class Matrix4x4;
	// create an alias for cache alignment
	typedef __declspec(align(64)) Matrix4x4 FastMatrix4x4;

	class Matrix4x4
	{
		// This is a very coordinate transform centric Matrix class.
	public:
		// constructors
		// Default constructor - Does Nothing. Initializing 16 floating point numbers
		// 	can be time consuming so we have a default constructor that does nothing.
		//	Most of the time we will be initializing via one of the create functions
		//	below (i.e. CreateTranslation( )
		Matrix4x4();
		//  Value based constructor - This is good for things like creating const
		//	matrix (i.e. Identity matrix
		Matrix4x4(
			float i_11, float i_12, float i_13, float i_14,
			float i_21, float i_22, float i_23, float i_24,
			float i_31, float i_32, float i_33, float i_34,
			float i_41, float i_42, float i_43, float i_44
			);

		// Copy constructor & assignment operator
		Matrix4x4(const Matrix4x4 & i_other);
		Matrix4x4 & operator = (const Matrix4x4 & i_other);
		
		// Useful matrix creators
		static Matrix4x4 CreateIdentity(void);
		static Matrix4x4 CreateXRotation(float i_RotationRadians);
		static Matrix4x4 CreateYRotation(float i_RotationRadians);
		static Matrix4x4 CreateZRotation(float i_RotationRadians);

		static Matrix4x4 CreateTranslation(float i_transX, float i_transY, float i_transZ);
		static Matrix4x4 CreateTranslation(Vector3 i_vectrans);
		static Matrix4x4 CreateScale(float i_scaleX, float i_scaleY, float i_scaleZ);

		// Invert this instance
		inline void Invert(void);
		// Return a matrix that is the inverse of this instance but don't modify this instance
		inline Matrix4x4 GetInverse(void) const;

		// Transpose this instance
		void Transpose(void);
		// return a matrix that is a transpose of this instance but don't modify this instance
		Matrix4x4 GetTranspose(void) const;
	
		// Multiply Matrix * Matrix
		// return *this * i_other
		Matrix4x4 operator*(const Matrix4x4 & i_other) const;

		//Get any row or col
		Vector4 Row(size_t i_row) const;
		Vector4 Col(size_t i_col) const;
	
		// Multiply Vector by Matrix
		// return v * M
		Vector4 MultiplyLeft(const Vector4 & i_Vector) const;
		// return M * v
		Vector4 MultiplyRight(const Vector4 & i_Vector) const;
		inline static float deg2rad(const float deg);

		static const FastMatrix4x4 Identity;

	private:
		float
			// m_<row><column>
			m_11, m_12, m_13, m_14, // row 1
			m_21, m_22, m_23, m_24, // row 2
			m_31, m_32, m_33, m_34, // row 3
			m_41, m_42, m_43, m_44; // row 4

		Matrix4x4 Multiply(const Matrix4x4 & i_other, Matrix4x4 & o_out) const;
	
		// internal inverse function used by Invert() and GetInverse()
		Matrix4x4 & InverseSSE(Matrix4x4 & o_out) const;
		Matrix4x4 & MultiplySSE(const Matrix4x4 & i_other, Matrix4x4 & o_out) const;

		inline bool IsNAN(const float i_val) const;
	};

	// Allows us to use V = M * V; (i.e. column vector)
	inline Vector4 operator*(const Matrix4x4 & i_mtx, const Vector4 & i_vec);
	// Allows us to use V = V * M; (i.e. row vector)
	inline Vector4 operator*(const Vector4 & i_vec, const Matrix4x4 & i_mtx);
}

#include "Matrix4x4-inl.h"

#endif