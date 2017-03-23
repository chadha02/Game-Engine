# pragma once
#include <stdint.h>
#include "../GLib/_Console/ConsolePrint.h"

# pragma once
#include <stdint.h>
#include "../GLib/_Console/ConsolePrint.h"


namespace Engine
{
	class StringPool
	{
		StringPool(uint8_t *i_pPool, size_t i_sizePool);
		uint8_t * m_pStart;
		uint8_t * m_pEnd;
		uint8_t * m_pCurrent;
		size_t m_pSize;

	public:
		static StringPool * Create(size_t i_bytesInPool);
		const char * add(const char * i_pString);
		const char * find(const char * i_pString);
		~StringPool();

	};
}