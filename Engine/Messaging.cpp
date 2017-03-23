#include "Messaging.h"
#include "HeashedString.h"

namespace Engine
{
	namespace MessagingSystem
	{
		Messaging::Messaging()
		{
			m_Handler = new std::map<HeashedString, std::vector <IMessageHandler *>>();
		}

		void Messaging::RegisterMessageHandler(const HeashedString & i_Message, IMessageHandler * i_pHandler)
		{
			(*m_Handler)[i_Message].push_back(i_pHandler);
		}

		void Messaging::DeregisterMessageHandler(const HeashedString & i_Message, IMessageHandler * i_pHandler)
		{
			assert( m_Handler->find(i_Message) != m_Handler->end() );  // make sure message is registerd with a handler and is in the map

			std::vector<IMessageHandler *> vectorHandle = (*m_Handler)[i_Message];
			// make sure the handler is in the vector that is registerd for the map
			assert(std::find(vectorHandle.begin(), vectorHandle.end(), i_pHandler) != vectorHandle.end());
				
			// find the position of the handler in vector and swap it with last element then remove from end
			std::vector<IMessageHandler *>::iterator it = std::find(vectorHandle.begin(), vectorHandle.end(), i_pHandler);
			std::iter_swap(it, --vectorHandle.end());
			vectorHandle.pop_back();
			return;
		}

		void Messaging::NotifyMessage(const HeashedString & i_Message, void * i_pMessageDetails)
		{
			std::vector<IMessageHandler *> vectorHandle = (*m_Handler)[i_Message];
			for (std::vector<IMessageHandler *>::iterator it = vectorHandle.begin(); it != vectorHandle.end(); ++it)
			{
				(*it)->HandleMessage(i_Message, i_pMessageDetails);
			}
		}

		Messaging:: ~Messaging()
		{
			delete m_Handler;
		}
	}
}