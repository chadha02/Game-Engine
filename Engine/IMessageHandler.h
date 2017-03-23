#pragma once
#ifndef _IMESSAGEHANDLER_H_
#define _IMESSAGEHANDLER_H_

#include "HeashedString.h"

namespace Engine
{
	namespace MessagingSystem
	{
		class IMessageHandler
		{
		public:
			virtual ~IMessageHandler() {}
			virtual void HandleMessage(const HeashedString & i_Message, void *) = 0;
		};
	}
}

#endif