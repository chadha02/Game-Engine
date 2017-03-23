#include "FixedSizeAllocator.h"

#include <malloc.h>
#include <assert.h>
#include <assert.h>
#include <stdio.h>


void FixedSizeAllocator::Initialise(size_t i_SizeBlocks, size_t i_NumBlocks)
{
	m_SizeBlock = i_SizeBlocks; // set the size of each block
	m_NumBlocks = i_NumBlocks;  // set the number of blocks
	m_pBase = (uint8_t *)_aligned_malloc((m_NumBlocks*sizeof(Node)) + (m_SizeBlock*m_NumBlocks), 4);

	m_pFree = InitailiseNodes(m_pBase, i_NumBlocks, i_SizeBlocks);  // set a free memory list
}

FixedSizeAllocator::Node* FixedSizeAllocator::InitailiseNodes(uint8_t * i_pBase, size_t i_NumBlocks, size_t i_SizeBlocks)
{
	Node *pBase = (Node *)i_pBase;
	for (unsigned int i = 0; i < i_NumBlocks; i++)
	{
		uint8_t *nextPtr = (i_pBase + 1);
		pBase->m_pNext = (Node*)(nextPtr + i_SizeBlocks);
		pBase = pBase->m_pNext;
	}
	pBase->m_pNext = NULL;
	return (Node*)i_pBase;
}

void * FixedSizeAllocator::_alloc()
{
	assert(m_pFree);
	void *pFreeBlock = m_pFree;
	m_pFree = m_pFree->m_pNext;
	
	return pFreeBlock;

}

void  FixedSizeAllocator::_free(void * i_ptr)
{
	assert(i_ptr);
	ptrdiff_t distance = (uint8_t *)i_ptr - m_pBase;
	assert(distance % (sizeof(Node *)+m_SizeBlock) == 0);
	Node * temp = (Node*)i_ptr;

	temp->m_pNext = m_pFree;
	m_pFree = temp;  // put it in the free memory list
	
}

FixedSizeAllocator::~FixedSizeAllocator()
{
	_aligned_free(m_pBase);
}
