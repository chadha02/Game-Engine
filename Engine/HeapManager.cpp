#include "HeapManager.h"
#include <assert.h>


	HeapManager::HeapManager(){};
	void HeapManager::Initialize(void *i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors)
	{
		assert(i_pMemory);
		assert(i_sizeMemory);
		assert(i_numDescriptors * sizeof(MemBlockDesc) < i_sizeMemory);

		MemBlockDesc * pMemBlocks = reinterpret_cast<MemBlockDesc *>(i_pMemory);

		InitializeFreeDescriptors(pMemBlocks, i_numDescriptors);
		uint8_t *	 pFreeMemoryStart = reinterpret_cast<uint8_t *>(pMemBlocks + i_numDescriptors); // free memory starting point
		uint8_t * pFreeMemoryEnd = reinterpret_cast<uint8_t *>(i_pMemory)+i_sizeMemory;  // free memory ending

		m_pFreeMemory = GetFreeDescriptor();

		assert(m_pFreeMemory);
		m_pFreeMemory->m_pBase = reinterpret_cast<uint8_t *>(pFreeMemoryStart);  // the base of the first discriptor ponts to the starting pos of free memory
		m_pFreeMemory->m_size = pFreeMemoryEnd - m_pFreeMemory->m_pBase; //the size of the free memory pointed by discriptor 
		m_pFreeMemory->m_pNext = NULL;
		m_pAllocatedMemory = NULL; // No memeory has been allocated yet

	}

	void HeapManager::InitializeFreeDescriptors(MemBlockDesc *i_pFirstDiscriptor, unsigned int i_numDecriptors)
	{
		m_pFreeDiscriptors = i_pFirstDiscriptor; // first descriptor

		MemBlockDesc * desc = i_pFirstDiscriptor;
		assert(i_numDecriptors != 0);
		while (i_numDecriptors - 1)
		{
			desc->m_pNext = desc + 1;
			desc = desc->m_pNext;
			i_numDecriptors--;
		}
		desc->m_pNext = NULL;
	}

	HeapManager::MemBlockDesc * HeapManager::GetFreeDescriptor()
	{
		MemBlockDesc * freeDesc = m_pFreeDiscriptors;
		if (freeDesc)
		{
			m_pFreeDiscriptors = freeDesc->m_pNext;
		}
		return freeDesc;
	}

	void * HeapManager::Allocate(const size_t i_sizeAlloc)
	{
		MemBlockDesc * freeMemoryDesc = FindBestFreeMemoryDescriptor(i_sizeAlloc); // find a discriptor suitable for this size first
		MemBlockDesc * desc = NULL;
		if (freeMemoryDesc)
			desc = AllocateFromBlockEnd(*freeMemoryDesc, i_sizeAlloc);
		return desc ? reinterpret_cast<void *>(desc->m_pBase) : desc;  // return the base pointed by this discriptor else return null
	}

	HeapManager::MemBlockDesc * HeapManager::FindBestFreeMemoryDescriptor(const size_t i_size)
	{
		MemBlockDesc * desc = m_pFreeMemory;
		while (desc && desc->m_size < i_size) desc = desc->m_pNext;
		return desc;
	}

	HeapManager::MemBlockDesc * HeapManager::AllocateFromBlockEnd(MemBlockDesc & i_Block, size_t i_size)
	{
		uint8_t * pNewBlockStart = i_Block.m_pBase + i_Block.m_size - i_size; // /confusion
		assert(pNewBlockStart >= i_Block.m_pBase);

		if (pNewBlockStart < i_Block.m_pBase)
			return NULL;

		MemBlockDesc * pDesc = GetFreeDescriptor();
		//assert(pDesc);

		if (!pDesc)
			return NULL;

		pDesc->m_size = i_Block.m_pBase + i_Block.m_size - pNewBlockStart;
		pDesc->m_pBase = pNewBlockStart;
		pDesc->m_pNext = NULL;

		i_Block.m_size = pNewBlockStart - i_Block.m_pBase;

		AddToAllocatedDescriptors(pDesc); //Adding pDesc to AllocatedMemory List
		assert(m_pAllocatedMemory);
		return pDesc;
	}



	bool HeapManager::Free(void * i_pMemory)
	{
		//Finding the i_pMemory in Allocated list and get the corresponding descriptor
		MemBlockDesc * desc = m_pAllocatedMemory;
		assert(m_pAllocatedMemory);
		MemBlockDesc * prevDesc = NULL;
		while (desc) {
			if (desc->m_pBase == i_pMemory)
			{
				break;
			}
			else
			{
				prevDesc = desc;
				desc = desc->m_pNext;
			}
		}

		assert(desc); //Cannot Find the descriptor

		//Move that descriptor to the free memory descriptors list
		if (prevDesc)
		{
			prevDesc->m_pNext = desc->m_pNext;
		}
		else
		{
			m_pAllocatedMemory = desc->m_pNext;
		}
		//Remove contents inside the memory but before that how can i call the deconstructor of that object?
		AddToFreeMemoryDescriptors(desc);
		return desc != NULL;
	}

	void HeapManager::AddToAllocatedDescriptors(MemBlockDesc * i_pAllocatedDesc)
	{
		i_pAllocatedDesc->m_pNext = m_pAllocatedMemory;
		m_pAllocatedMemory = i_pAllocatedDesc;
	}

	void HeapManager::AddToFreeMemoryDescriptors(MemBlockDesc * i_pFreeMemDesc)
	{
		i_pFreeMemDesc->m_pNext = m_pFreeMemory;  // get the freed discriptor to the initial memmory 
		m_pFreeMemory = i_pFreeMemDesc;  // initial memeory now points to this discriptor
	}

	void HeapManager::GarbageCollect()
	{
		SortMemoryDescBlocks(m_pFreeMemory);
		RecurMemoryCollect(m_pFreeMemory);
	}
	void HeapManager::SortMemoryDescBlocks(MemBlockDesc * i_headBlockDesc)
	{
		MemBlockDesc * tempDescPtr = i_headBlockDesc;

		unsigned int numOfDescs = 0;

		while (tempDescPtr)
		{
			tempDescPtr = tempDescPtr->m_pNext;
			numOfDescs++;
		}

		for (unsigned int i = 0; i < numOfDescs; i++) {
			tempDescPtr = i_headBlockDesc;
			while (tempDescPtr && tempDescPtr->m_pNext)
			{
				if (tempDescPtr->m_pBase > tempDescPtr->m_pNext->m_pBase)
				{
					size_t tempSize = tempDescPtr->m_size;
					uint8_t * tempBase = tempDescPtr->m_pBase;

					tempDescPtr->m_size = tempDescPtr->m_pNext->m_size;
					tempDescPtr->m_pBase = tempDescPtr->m_pNext->m_pBase;

					tempDescPtr->m_pNext->m_size = tempSize;
					tempDescPtr->m_pNext->m_pBase = tempBase;

				}
				else tempDescPtr = tempDescPtr->m_pNext;
			}
		}

	}

	void HeapManager::RecurMemoryCollect(MemBlockDesc * i_desc)
	{
		if (!i_desc) return;

		RecurMemoryCollect(i_desc->m_pNext);
		if (i_desc && i_desc->m_pNext)
		{
			AttemptToCombineBlocks(*i_desc, *(i_desc->m_pNext));
		}
	}
	bool HeapManager::AttemptToCombineBlocks(MemBlockDesc & i_Block1, MemBlockDesc & i_Block2)
	{
		// See if Block1 ends at the address right before Block2 starts
		// If so consume Block2's memory into Block1 and return
		// Block2's descriptor to the free descriptor pool

		if ((i_Block1.m_pBase + i_Block1.m_size) == (i_Block2.m_pBase))
		{
			// Expand Block1 to consume Block2's memory
			i_Block1.m_size += i_Block2.m_size;
			// Return Block2's descriptor to the free descriptor pool
			// since it's memory was consumed into Block1 and it's
			// descriptor is of no need
			i_Block1.m_pNext = i_Block2.m_pNext;
			AddToFreeDescriptors(&i_Block2);
			return true;
		}

		return false;
	}
	void HeapManager::AddToFreeDescriptors(MemBlockDesc * i_pFreeDesc)
	{
		i_pFreeDesc->m_pNext = m_pFreeDiscriptors;
		m_pFreeDiscriptors = i_pFreeDesc;
	}
