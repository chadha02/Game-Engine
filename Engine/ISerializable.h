#pragma once
#ifndef _ISERIALIZABLE_H
#define _ISERIALIZABLE_H
#include <inttypes.h>
#include "HeashedString.h"

namespace Engine
{
	class Iserialisable
	{
	public:
		virtual HeashedString getSerializableType() = 0;
		virtual HeashedString getSerializableId() = 0;

		virtual size_t serializePosition(uint8_t* buffer) = 0;
		virtual size_t deserializePosition(uint8_t* buffer) = 0;
		virtual size_t serializeRotation(uint8_t* buffer) = 0;
		virtual size_t deserializeRotation(uint8_t* buffer) = 0;
		virtual size_t serializeVelocity(uint8_t* buffer) = 0;
		virtual size_t deserializeVelocity(uint8_t* buffer) = 0;


	};
}

#endif