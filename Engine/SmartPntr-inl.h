# pragma once
#include "SmartPntr.h"

namespace Engine
{
	template <class T> inline void SmartPntr<T>::ReleseRefCount()
	{
		assert(m_object);
		assert((*m_refcount) != 0);
			if (--(*m_refcount) == 0)
			{
				delete m_refcount;
				delete m_object;
			}
		
	}

	template <class T> inline void SmartPntr<T> ::AcquireRef(const SmartPntr <T>  & i_object)
	{
		m_refcount = i_object.m_refcount;
		m_object = i_object.m_object;
		if (m_refcount !=nullptr)
		(*m_refcount)++;
	}
}