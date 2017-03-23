#include "Serialize.h"

namespace Engine
{
	Serialize::Serialize()
	{
		serialisationBuffer = NULL;
	}

	Serialize::~Serialize()
	{ } 

	bool Serialize::Initialize(size_t bufferSize)
	{
		serialisationBuffer = reinterpret_cast<uint8_t *> (malloc(bufferSize));
		startMarker = serialisationBuffer;
		bufferEndMarker = serialisationBuffer + bufferSize;
		sizeOfBuffer = bufferSize;
		return true;
	}
		
	void Serialize::AddSerializableObject(Iserialisable* object)
	{
		assert(object);
		SerialisableObjects.push_back(object);

	}
	
	void Serialize::SerialiseObjects()
	{
		assert(serialisationBuffer);
		for (size_t i = 0; i < SerialisableObjects.size(); i++)
		{
			uint32_t id = static_cast<uint32_t>(SerialisableObjects[i]->getSerializableId().Get());
			memcpy(serialisationBuffer, &id, sizeof(uint32_t));
			serialisationBuffer += sizeof(uint32_t);

			uint32_t type = static_cast<uint32_t>(SerialisableObjects[i]->getSerializableType().Get());
			memcpy(serialisationBuffer, &type, sizeof(uint32_t));
			serialisationBuffer += sizeof(uint32_t);

			uint8_t* sizeMarker = serialisationBuffer;
			memset(serialisationBuffer, 0, sizeof(size_t)); //2*sizeof(size_t)
			serialisationBuffer += sizeof(size_t); // //2*sizeof(size_t)
			
			size_t size1 = SerialisableObjects[i]->serializePosition(serialisationBuffer);
			serialisationBuffer += size1;
			
			size_t size2 = SerialisableObjects[i]->serializeRotation(serialisationBuffer);
			serialisationBuffer += size2;

			size_t size3 = SerialisableObjects[i]->serializeVelocity(serialisationBuffer);
			serialisationBuffer += size3;


			memset(sizeMarker, size1+size2+size3, sizeof(size_t));

			assert(serialisationBuffer < bufferEndMarker);

		}
		std::ofstream outfile("data\\SaveData.txt", std::ofstream::binary);
		outfile.write(reinterpret_cast<char*>(startMarker), sizeOfBuffer);
		outfile.close();

	}
	
	void Serialize::DeSerialiseObjects()
	{
		std::ifstream infile("data\\SaveData.txt", std::ifstream::binary);
		if (infile)
		{
			// get length of file:
			infile.seekg(0, infile.end);
			std::streamoff fileSize = infile.tellg();
			infile.seekg(0, infile.beg);
			assert(serialisationBuffer != NULL);
			infile.read(reinterpret_cast<char*>(startMarker), fileSize);
			serialisationBuffer = startMarker;
			serialisationBuffer = reinterpret_cast<uint8_t*>(serialisationBuffer);
			for (unsigned int i = 0; i < SerialisableObjects.size(); i++)
			{
				uint32_t hashedId;
				memcpy(&(hashedId), serialisationBuffer, sizeof(uint32_t));
				uint32_t id = static_cast <uint32_t>(SerialisableObjects[i]->getSerializableId().Get());
				if (id == hashedId)
				{
					//skipping data that we dont need
					serialisationBuffer += sizeof(uint32_t);
					serialisationBuffer += sizeof(uint32_t);
					serialisationBuffer += sizeof(size_t);
					size_t size = SerialisableObjects[i]->deserializePosition(serialisationBuffer);
					serialisationBuffer += size;
					size_t size2 = SerialisableObjects[i]->deserializeRotation(serialisationBuffer);
					serialisationBuffer += size2;
					size_t size3 = SerialisableObjects[i]->deserializeVelocity (serialisationBuffer);
					serialisationBuffer += size3;
				}
			}
		}
		infile.close();
	}

	void Serialize::ShutDown()
	{
		free(startMarker);
		
	}

}