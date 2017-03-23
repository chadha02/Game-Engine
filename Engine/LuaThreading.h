#pragma once

#ifndef _LUATHREADING_H_
#define _LUATHREADING_H_

#include "LuaFile.h"
#include "ThreadedFileProcessor.h"

namespace Engine
{

		class LuaThreading : public ThreadedFileProcessor::LoadTask
		{
		public:
			LuaThreading(const char * i_pFilename) :ThreadedFileProcessor::LoadTask(i_pFilename)
			{

			}
			virtual void ProcessFileContents(uint8_t * i_pBuffer, uint32_t i_BufferBytes)
			{
				if ((i_pBuffer != NULL) && (i_BufferBytes > 0))
				{
					char * temp = new char[i_BufferBytes + 1];
					memcpy(temp, i_pBuffer, i_BufferBytes);
					temp[i_BufferBytes] = 0;

					printf("%s countains:\n%s", GetFileName(), temp);

					delete temp;
				}
			}
			static void LoadFile(const char* fileName,Mutex *fileMutex)
			{
				ThreadedFileProcessor& processor = ThreadedFileProcessor::GetInstance();
				fileMutex->Lock();
				processor.AddToLoadQueue(*new LuaThreading(fileName));
				Sleep(1);
				fileMutex->Release();
			}

			static void ProcessFile(const char* fileName,  Mutex *fileMutex)
			{
				ThreadedFileProcessor& processor = ThreadedFileProcessor::GetInstance();
				fileMutex->Lock();
				processor.AddToLoadQueue(*new LuaThreading(fileName));
				Sleep(1);

				fileMutex->Release();
			}


		};
	}



#endif
