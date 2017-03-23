
#include <assert.h>
#include "ConsolePrint.h"

#include "Mutex.h"
#include "Semaphore.h"
#include "ThreadedFileProcessor.h"

namespace Engine
{
ThreadedFileProcessor::LoadTask::LoadTask( const char * i_pFileName )
{
	assert( i_pFileName );
	m_FileName = _strdup( i_pFileName );
}

ThreadedFileProcessor::LoadTask::~LoadTask()
{
	free( const_cast<char *>( m_FileName ) );
}

const char * ThreadedFileProcessor::LoadTask::GetFileName( void )
{
	return m_FileName;
}

ThreadedFileProcessor::ThreadedFileProcessor( HANDLE i_LoadThreadHandle, HANDLE i_ProcessThreadHandle ) :
	m_ShutdownRequestEvent( "ShutdownRequestEvent", false ),
	m_LoadThreadHandle( i_LoadThreadHandle ),
	m_ProcessThreadHandle( i_ProcessThreadHandle ),
	m_LoadQueueSemaphore( "LoadQueueSemaphore", 100, 0 ),
	m_LoadQueueMutex( "LoadQueueMutex" ),
	m_ProcessQueueSemaphore( "ProcessQueueSemaphore", 100, 0 ),
	m_ProcessQueueMutex( "ProcessQueueMutex" )
{
		ResumeThread( m_LoadThreadHandle );
		ResumeThread( m_ProcessThreadHandle );
}

static ThreadedFileProcessor * pInstance = NULL;

ThreadedFileProcessor & ThreadedFileProcessor::GetInstance( void )  // create a thread
{

	if( pInstance == NULL )
	{
		DWORD ThreadID = 0;

		HANDLE LoadThreadHandle = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE) ThreadedFileProcessor::LoadThreadFunction, NULL, CREATE_SUSPENDED, &ThreadID );
		assert( LoadThreadHandle != NULL );

		HANDLE ProcessThreadHandle = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE) ThreadedFileProcessor::ProcessThreadFunction, NULL, CREATE_SUSPENDED, &ThreadID );
		assert( ProcessThreadHandle != NULL );

		pInstance = new ThreadedFileProcessor( LoadThreadHandle, ProcessThreadHandle );
	}

	assert( pInstance != NULL );

	return *pInstance;
}

void ThreadedFileProcessor::Shutdown( void )  // shutting down a thread
{
	if( pInstance )
	{
		ThreadedFileProcessor & Me = GetInstance();

		Me.m_ShutdownRequestEvent.Signal();

		HANDLE ThreadHandles[] = { Me.m_LoadThreadHandle, Me.m_ProcessThreadHandle };

		DWORD Result = WaitForMultipleObjects( sizeof( ThreadHandles ) / sizeof( ThreadHandles[0] ), ThreadHandles, TRUE, INFINITE );

		delete pInstance;
		pInstance = NULL;
	}
}

void ThreadedFileProcessor::AddToLoadQueue( LoadTask & i_LoadTask )  // getting a mutex for the thread
{
	m_LoadQueueMutex.Lock();
		
	m_LoadQueue.push( &i_LoadTask );

	m_LoadQueueMutex.Release();

	m_LoadQueueSemaphore.Increment();
}

void ThreadedFileProcessor::AddToProcessQueue( struct _LoadTaskData & i_LoadTaskData )
{
	m_ProcessQueueMutex.Lock();

	m_ProcessQueue.push( &i_LoadTaskData );

	m_ProcessQueueMutex.Release();
	m_ProcessQueueSemaphore.Increment();// releasing the semaphore and incrementing it
}

ThreadedFileProcessor::LoadTask * ThreadedFileProcessor::GetFromLoadQueue( void )
{
	LoadTask * pLoadTask = NULL;

	m_LoadQueueMutex.Lock();

	if( !m_LoadQueue.empty() )
	{
		pLoadTask = m_LoadQueue.front();
		m_LoadQueue.pop();
	}

	m_LoadQueueMutex.Release();

	return pLoadTask;
}

ThreadedFileProcessor::_LoadTaskData * ThreadedFileProcessor::GetFromProcessQueue( void )
{
	struct _LoadTaskData * pLoadTaskData = NULL;

	m_ProcessQueueMutex.Lock();

	if( !m_ProcessQueue.empty() )
	{
		pLoadTaskData = m_ProcessQueue.front();
		m_ProcessQueue.pop();
	}

	m_ProcessQueueMutex.Release();

	return pLoadTaskData;
}

DWORD WINAPI ThreadedFileProcessor::LoadThreadFunction( void * i_pThreadData )
{
	ThreadedFileProcessor & Me = ThreadedFileProcessor::GetInstance();

	OVERLAPPED FileOverlapped;

	FileOverlapped.Offset = FileOverlapped.OffsetHigh = 0;
	FileOverlapped.hEvent = CreateEvent( NULL, FALSE, FALSE, NULL );

	HANDLE LoadHandles[] = { Me.m_ShutdownRequestEvent.GetHandle(), Me.m_LoadQueueSemaphore.GetHandle() };
	HANDLE ReadHandles[] = { Me.m_ShutdownRequestEvent.GetHandle(), FileOverlapped.hEvent };

	bool bDone = false;

	while( !bDone )
	{
		// Me.m_LoadQueueSemaphore.WaitAndDecrement();
		DWORD LQResult = WaitForMultipleObjects( sizeof( LoadHandles ) / sizeof( LoadHandles[0] ), LoadHandles, FALSE, INFINITE );

		if( LQResult == WAIT_OBJECT_0 )
		{
			bDone = true;
		}
		else if( LQResult == WAIT_OBJECT_0 + 1 )
		{
			ThreadedFileProcessor::LoadTask * pLoadTask = Me.GetFromLoadQueue();

			if( pLoadTask )
			{
				const char * pFileName = pLoadTask->GetFileName();
				assert( pFileName != NULL );

				Engine::ConsolePrint( "Processing %s\n", pFileName );

				HANDLE FileHandle = CreateFile( pFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL );

				if( FileHandle == INVALID_HANDLE_VALUE )
				{
					Engine::ConsolePrint( "Error Opening File %s : %d", pLoadTask, GetLastError() );
					delete pLoadTask;
				}
				else
				{
					DWORD FileSize = GetFileSize( FileHandle, NULL );
					assert( FileSize != INVALID_FILE_SIZE );

					uint8_t * pFileBuffer = new uint8_t[FileSize];
					assert( pFileBuffer != NULL );

					BOOL RFResult = ReadFile( FileHandle, pFileBuffer, FileSize, NULL, &FileOverlapped );
					if( (RFResult != TRUE)  && ( GetLastError() != ERROR_IO_PENDING ) )
					{
						Engine::ConsolePrint( "Failed reading %s : %d", pFileName, GetLastError() );
						delete pLoadTask;
					}
					else
					{
						DWORD RResult = WaitForMultipleObjects( sizeof( ReadHandles ) / sizeof( ReadHandles[0] ), ReadHandles, FALSE, INFINITE );
						if( RResult == WAIT_OBJECT_0 )
						{
							CancelIo( FileHandle );
							delete pLoadTask;
							bDone = true;
						}
						else if( RResult == WAIT_OBJECT_0 + 1 )
						{
							DWORD BytesRead = 0;
							BOOL GORResult = GetOverlappedResult( FileHandle, &FileOverlapped, &BytesRead, FALSE );
							assert( GORResult == TRUE );

							struct _LoadTaskData * pNewLoadTaskData = new _LoadTaskData( pFileBuffer, BytesRead, pLoadTask );

							Me.AddToProcessQueue( *pNewLoadTaskData );
						}
						else
						{
							Engine::ConsolePrint( "Failed waiting on File I/O %s : %d", pFileName, GetLastError() );
							delete pLoadTask;
						}
					}
				}
			}
		}
		else
		{
			assert( false );
		}
	}

	CloseHandle( FileOverlapped.hEvent );
	return 0;
}

DWORD WINAPI ThreadedFileProcessor::ProcessThreadFunction( void * i_pThreadData )
{
	ThreadedFileProcessor & Me = ThreadedFileProcessor::GetInstance();

	HANDLE WaitHandles[] = { Me.m_ShutdownRequestEvent.GetHandle(), Me.m_ProcessQueueSemaphore.GetHandle() };
	
	bool bDone = false;

	while( !bDone )
	{
		//Me.m_ProcessQueueSemaphore.WaitAndDecrement();
		DWORD result = WaitForMultipleObjects( sizeof( WaitHandles) / sizeof( WaitHandles[0] ), WaitHandles, FALSE, INFINITE );

		if( result == WAIT_OBJECT_0 )
		{
			bDone = true;
		}
		else if( result == (WAIT_OBJECT_0 + 1) )
		{
			ThreadedFileProcessor::_LoadTaskData * pLoadData = Me.GetFromProcessQueue();

			if( pLoadData )
			{
				pLoadData->m_pLoadTask->ProcessFileContents( pLoadData->m_pFileContents, pLoadData->m_FileContentsSize );

				delete [] pLoadData->m_pFileContents;

				delete pLoadData->m_pLoadTask;

				delete pLoadData;
			}
		}
		else
		{
			assert( false );
		}
	}

	return 0;
}

} // namespace Azra