
#include "StringPool.h"
#include <malloc.h>
#include "StringPool.h"
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

namespace Engine
{
	StringPool::StringPool(uint8_t *i_pPool, size_t i_sizePool) :
		m_pSize(i_sizePool), m_pStart(i_pPool), m_pCurrent(i_pPool), m_pEnd(i_pPool + i_sizePool)
	{

	}

	StringPool * StringPool::Create(size_t i_bytesInPool)
	{
		uint8_t *i_pPool = reinterpret_cast<uint8_t *> (_aligned_malloc(i_bytesInPool, 4));

		return new StringPool(i_pPool, i_bytesInPool);
	}


	StringPool::~StringPool()
	{
		
		_aligned_free(m_pStart);
	}

	const char * StringPool::add(const char * i_pString)
	{
			
		//uint8_t * pStrSize = m_pCurrent;
		*m_pCurrent = (uint8_t)(strlen(i_pString));

		assert(m_pCurrent + sizeof(uint8_t)+strlen(i_pString) + 1 <= m_pEnd); 

		if (find(i_pString))
		{
			return find(i_pString);
		}

		

		char *stringAdd = reinterpret_cast<char *> (m_pCurrent + sizeof(uint8_t)); // 1 byte aage pehle byte pe size store kar rakha he
		strcpy_s(stringAdd, strlen(i_pString) + 1, i_pString);

		m_pCurrent += strlen(i_pString) + 1 + sizeof(uint8_t);
		return stringAdd;

	}

	const char * StringPool::find(const char * i_pString)
	{
		uint8_t *start = m_pStart;
		char *strFound = NULL;

		while (start < m_pCurrent)
		{
			uint8_t *startLength = start;
			char *stringFromStart = reinterpret_cast<char *>(start + sizeof(uint8_t));
			if (*startLength == strlen(i_pString) && strcmp(stringFromStart, i_pString) == 0)
			{
				strFound = stringFromStart;

				break;
			}
			else
				start += *startLength + 1 + sizeof(uint8_t);
		}

		return strFound;
	}

}
