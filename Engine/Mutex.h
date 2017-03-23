#ifndef __MUTEX_H
#define __MUTEX_H

#include "WaitableObject.h"

namespace Engine
{

class Mutex : public WaitableObject
{
public:
	Mutex( const char * i_pName );
	~Mutex();

	bool TryLock( void );
	void Lock( void );
	void Release( void );

};

} // namespace Engine

#endif // __MUTEX_H
