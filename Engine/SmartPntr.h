//# pragma once
#ifndef SMART_PNTR_H_
#define SMART_PNTR_H_

#include <cstddef>
#include <assert.h>
#include "../GLib/_Console/ConsolePrint.h"


namespace Engine
{
	template<class T>
	class SmartPntr
	{
		T * m_object;
		long * m_refcount;
		inline void ReleseRefCount();
		inline void AcquireRef(const SmartPntr<T> & i_object);

	public:
		SmartPntr() : m_object(nullptr), m_refcount(nullptr)
		{}

		SmartPntr(T * i_object) : m_object(i_object), m_refcount(new long(1))
		{
			
		}

	SmartPntr(const SmartPntr<T> & i_object) : m_object(i_object.m_object), m_refcount(i_object.m_refcount)
		{
			if (m_refcount != nullptr)
				(*m_refcount)++;
		//	DEBUG_PRINT("initialised through other smart pointr");
		}

	T* getObject()
	{
		return m_object;
	}

	SmartPntr<T> & operator = ( T * i_object)
		{

		
		if (this->m_object != i_object)
		{
			//assert(m_object);
			if (m_refcount != nullptr && m_object != nullptr)
			{
				ReleseRefCount();
			}
			
			m_object = i_object;
			m_refcount = nullptr;
			if (i_object != nullptr)
		{
			m_refcount = new long;
			*m_refcount = 1;
		}

		}
			return *this;

	}

		SmartPntr<T> & operator = (const SmartPntr<T> & i_object)
		{
			
		if (m_refcount != nullptr && m_object != nullptr)
			{
				ReleseRefCount();
			}
			AcquireRef(i_object);
			return *this;

		}




		inline T & operator *()
		{
			assert(m_object != nullptr);
			return *m_object;
		}

		inline T * operator ->()
		{
			assert(m_object != nullptr);
			return m_object;
		}
		inline bool operator ==(const SmartPntr<T> & i_object)const
		{
			return m_object == i_object;
		}

		inline bool operator ==(const T * i_object)const
		{
			return m_object == i_object;
		}

		inline bool operator !=(const SmartPntr<T> & i_object) const
		{
			return m_object != i_object;
		}

		inline bool operator !=(const T * i_object)const
		{
			return m_object != i_object;
		}


		inline bool operator ! ()
		{
			return !m_object;
		}
		~SmartPntr()
		{
			
			if (m_refcount != nullptr && m_object != nullptr)
			{
				ReleseRefCount();
			}
			//	assert(*m_refcount > 0);


		}

	};
}
#include "SmartPntr-inl.h"

#endif