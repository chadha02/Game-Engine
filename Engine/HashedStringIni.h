#include <assert.h>
#include <string.h>

namespace Engine
{
	inline HeashedString::HeashedString() :
		m_Hash(Hash(""))
	{
	}

	inline HeashedString::HeashedString(const char * i_string) :
		m_Hash(Hash(i_string))
	{
	}

	inline HeashedString::HeashedString(const HeashedString & i_other) :
		m_Hash(i_other.m_Hash)
	{
	}

	inline HeashedString::~HeashedString()
	{

	}

	inline HeashedString & HeashedString::operator=(const HeashedString & i_other)
	{
		m_Hash = i_other.m_Hash;
		return *this;
	}

	inline unsigned int HeashedString::Get(void) const
	{
		return m_Hash;
	}

	inline bool HeashedString::operator==(const HeashedString & i_other) const
	{
		return m_Hash == i_other.m_Hash;
	}
	
	inline bool HeashedString::operator!=(const HeashedString & i_other) const
	{
		return !(m_Hash == i_other.m_Hash);
	}
	inline bool HeashedString::operator < (const HeashedString & i_other) const
	{
		return m_Hash < i_other.m_Hash;
	}

} // namespace Engine
