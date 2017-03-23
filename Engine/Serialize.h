#pragma once
#ifndef _SERIALIZE_H
#define _SERIALIZE_H
#include<vector>
#include "ISerializable.h"
#include <fstream>
namespace Engine
{
	class Serialize
	{
		std::vector<Iserialisable *> SerialisableObjects;
		uint8_t* serialisationBuffer;
		uint8_t* bufferEndMarker;
		uint8_t* startMarker;
		size_t sizeOfBuffer;

	public:
		Serialize();
		~Serialize();
		void SerialiseObjects();
		void DeSerialiseObjects();
		bool Initialize(size_t bufferSize);
		void AddSerializableObject(Iserialisable* object);
		void ShutDown();

	};
}

#endif