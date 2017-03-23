#include "FixedSizeAllocator.h"
#include "HeapManager.h"

#include <iostream>
#include <stdio.h>
#include <conio.h>

void * operator new(size_t i_size, FixedSizeAllocator * i_pFixedAllocator)
{
	std::cout << "Assigning memory through fixed size allocator \n";

	return i_pFixedAllocator->_alloc();
	
}
void operator delete(void * i_ptr, FixedSizeAllocator * i_pFixedAllocator)
{
	std::cout << "freeing memory from fixed size allocator .press enter \n";
		i_pFixedAllocator->_free(i_ptr);
	_getch();

}

	void * operator new(size_t i_size, HeapManager * i_pHeap)
	{
		std::cout << "Assigning memory through 2 .press enter\n";
		void *p = i_pHeap->Allocate(i_size);
		return p;
	}


	void operator delete(void * i_ptr, HeapManager * i_pHeap){
		std::cout << "freeing memory through 2 .press enter\n";
		if (i_ptr){
			i_pHeap->Free(i_ptr);
		}
		_getch();
	}

	void * operator new[](size_t i_size, HeapManager * i_pHeap)
	{
		std::cout << "allocating memory through heapmanager..press enter \n";
		
		return i_pHeap->Allocate(i_size);
				
	}

	void operator delete[](void * i_ptr, HeapManager * i_pHeap)
	{
		std::cout << "freeing memory from heapmanger..press enter \n";
		
		i_pHeap->Free(i_ptr);
		_getch();
	}
