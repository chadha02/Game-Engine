#ifndef FIXEDSIZEALLOCATR_H_ 
#define FIXEDSIZEALLOCATR_H_ 

#include <stdint.h>

class FixedSizeAllocator
{
	struct Node
	{
		Node *m_pNext;
	};
	uint8_t* m_pBase;
	Node *m_pFree;

	size_t m_SizeBlock;
	size_t m_NumBlocks;
	Node * InitailiseNodes(uint8_t *, size_t, size_t);

public:
	void Initialise(size_t, size_t);
	void * _alloc();
	void _free(void *);
	~FixedSizeAllocator();
};


#endif