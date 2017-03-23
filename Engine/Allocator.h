#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#include "FixedSizeallocator.h"
#include "HeapManager.h"

void * operator new(size_t i_size, HeapManager * i_pHeap);
void * operator new(size_t i_size, FixedSizeAllocator * i_pFixedAllocator);
void  operator delete(void * i_ptr, FixedSizeAllocator * i_pFixedAllocator);
void operator delete(void * i_ptr, HeapManager * i_pHeap);
void * operator new[](size_t i_size, HeapManager * i_pHeap);
void operator delete[](void * i_ptr, HeapManager * i_pHeap);

#endif //__ALLOCATOR_H__