# pragma once

namespace Engine
{

	class HeashedString
	{
	public:
		HeashedString();
		~HeashedString();

		HeashedString(const char * i_string);
		HeashedString(const HeashedString & i_other);
		HeashedString & operator=(const HeashedString & i_other);

		unsigned int Get() const;

		inline bool operator==(const HeashedString & i_other) const;
		inline bool operator != (const HeashedString & i_other) const;
		inline bool operator < (const HeashedString & i_other) const;

		static unsigned int Hash(const char * i_string);
		static unsigned int Hash(const void * i_bytes, unsigned int i_count);
	private:
		unsigned int 	m_Hash;


	};

}

#include "HashedStringIni.h"

