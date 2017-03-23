#pragma once
#ifndef _MESSAGING_H_
#define _MESSAGING_H_
#include <map>
#include <vector>
#include "IMessageHandler.h"

namespace Engine
{
	namespace MessagingSystem
	{
		class Messaging
		{
			std::map <HeashedString, std::vector <IMessageHandler *>>*m_Handler;

		public:
			Messaging();

			// pass the message identifier i_Message along with the handler to register handler for multiple messages
			void RegisterMessageHandler(const HeashedString & i_Message, IMessageHandler * i_pHandler);
			void DeregisterMessageHandler(const HeashedString & i_Message, IMessageHandler * i_pHandler);
			void NotifyMessage(const HeashedString & i_Message, void * i_pMessageDetails);
			~Messaging();
		};
	}
}

#endif