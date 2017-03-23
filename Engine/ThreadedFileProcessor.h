#ifndef __THREADED_FILE_PROCESSOR_H
#define __THREADED_FILE_PROCESSOR_H

#include <stdint.h>
#include <queue>

#include <Windows.h>

#include "Event.h"
#include "Mutex.h"
#include "Semaphore.h"
#include <assert.h>

namespace Engine
{

class ThreadedFileProcessor
{
public:
	class LoadTask
	{
	public:
		LoadTask( const char * m_FileName );
		virtual ~LoadTask();

		const char * GetFileName( void );
		virtual void  ProcessFileContents( uint8_t * i_pBuffer, uint32_t i_BufferBytes ) = 0;
	private:
		const char *		m_FileName;
	};

	void AddToLoadQueue( LoadTask & i_LoadTask );

	static ThreadedFileProcessor & GetInstance( void );
	static void	Shutdown( void );

private:
	struct _LoadTaskData
	{
		uint8_t *				m_pFileContents;
		uint32_t 				m_FileContentsSize;

		LoadTask *				m_pLoadTask;

		_LoadTaskData( uint8_t * i_pFileContents, uint32_t i_FileContentsSize, LoadTask * i_pLoadTask ) :
			m_pFileContents( i_pFileContents ),
			m_FileContentsSize( i_FileContentsSize ),
			m_pLoadTask( i_pLoadTask )
		{
			assert( i_pFileContents != NULL );
			assert( i_FileContentsSize > 0 );
			assert( i_pLoadTask != NULL );
		}

	};

	HANDLE					m_LoadThreadHandle;
	HANDLE					m_ProcessThreadHandle;

	ManualResetEvent		m_ShutdownRequestEvent;
	Semaphore				m_LoadQueueSemaphore;
	Mutex					m_LoadQueueMutex;

	Semaphore				m_ProcessQueueSemaphore;
	Mutex					m_ProcessQueueMutex;

	std::queue<LoadTask *>	m_LoadQueue;
	std::queue< _LoadTaskData *>	m_ProcessQueue;

	ThreadedFileProcessor( HANDLE i_LoadThreadHandle, HANDLE i_ProcessThreadHandle );

	void AddToProcessQueue( struct _LoadTaskData & i_LoadTaskData );

	LoadTask * GetFromLoadQueue( void );
	struct _LoadTaskData * GetFromProcessQueue( void );

	static DWORD WINAPI LoadThreadFunction( void * i_pThreadData );
	static DWORD WINAPI ProcessThreadFunction( void * i_pThreadData );
};

} // namespace Engine

#endif // __THREADED_FILE_PROCESSOR_H
