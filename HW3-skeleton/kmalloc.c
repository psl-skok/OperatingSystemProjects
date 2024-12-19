#include "kmalloc.h"
#include "frame.h"
#include "translation.h"
#include "ll_double.h"

#include <stdbool.h>
// After you are done, you don't need to import it anymore. This is here just for the dummy code on malloc/free.
#include <stdlib.h>

struct list linked_list = {NULL, NULL};



void *palloc(uint64_t number) {
	// TODO:
	// 1) Allocate <parameter:number> frames of memory
	uint64_t frame_number = allocate_frame(number);
	// 2) Find the first page number (virtual) that is not mapped to a frame (physical), and <parameter:number> of them are consecutive
	
	// 3) IGNORE the result of the previous call, and map the frame number to itself
	uint64_t page_number = vm_locate(number); //(overwrites previous value, for the sake of simulation. See notes below.)
	page_number = frame_number;
	// vm_map(page_number, frame_number, number, /* use flags = 0 */);
	vm_map(page_number, frame_number, number, 0);
	// 4) Return the address of the first byte of the allocated page [see note below]
	return PAGE_ADDRESS(page_number);

	// NOTE:
	// - You are simulating the hardware automatic address translation in software.
	// - The palloc() function would return the address of the first virtual (mapped) address, based on the page number of step (2),
	//   but your application would not understand this since it only understands its own application addressing.
	// - Therefore, you are going to SIMULATE that there is another layer of automatic virtual addressing by
	//   always mapping page number X to the address of a frame number X
	// - Inside a real kernel, you wouldn't perform the overwrite of the page number in step (3),
	//   but the hardware would be performing the automatic translation between page_number to frame_number

	// Make sure, that in the end of this function:
	// vm_translate(page) is FRAME_ADDRESS(frame) 
}

void pfree(void* address, uint64_t number) {
	// TODO:
	// 1) Unmap the page number starting at the provided address (the address is the first byte of a page)
	// 2) Do that for the number of times provided in the parameter number
	for(int i = 0; i < number; i++){
		uint64_t* page_address = vm_translate(address);

		uint64_t page_number = PAGE_NUMBER(page_address);
		uint64_t frame_number = FRAME_NUMBER(page_address);
		
		vm_unmap(page_number, i);
		deallocate_frame(frame_number, i);
	}
}

void* block_init(){
	//call palloc which gives back address
	uint64_t* b = palloc(1);
	b[0] = &b[1];
	b[1] = 4096 - 3*sizeof(uint64_t);
	b[2] = 0;
	return b;
}

uint64_t* try_fit_data(uint64_t size, struct node* page){
	uint64_t* header = page;
	uint64_t* pred = page;
	uint64_t* freeList = header[0];
	u_int64_t* newFree;

	while(freeList != 0){
		if(freeList[0] > (size - 2*sizeof(uint64_t))){
			newFree = (uint64_t *)((char *)freeList + (size - 2*sizeof(uint64_t)));
			newFree[0] = freeList[0] - (size - 2*sizeof(uint64_t));
			newFree[1] = freeList[1];
			freeList[0] = size;
			freeList[1] = 0xc0dec0de;
			*pred = (uint64_t)newFree;
			return &header; //return the position of where the data is
		}
		pred = freeList + 1;
		freeList = (uint64_t)freeList[1];
	}
	return NULL;
}

void *kmalloc(uint64_t size) {
	// TODO:
	// - Implement a linked list of free chunks using only palloc() [see notes below]
	// - Use the first-fit strategy to allocate a chunk
	struct node* current = linked_list.head;
	while(current != NULL){
		uint64_t* p = try_fit_data(size, current);
		if(p != NULL){
			return p;
		}
		current = current->next;
	} 
	current = ll_insert_tail(&linked_list, block_init());
	return try_fit_data(size, current);
}

void *krealloc(void *address, uint64_t size) {
	// TODO:
	// - If the address is becoming smaller, return the last frames that have become unused with vm_unmap() and frame_deallocate()

	// - If the address is becoming bigger, and is possible to allocate new contiguous pages to extend the chunk size,
	//   allocate new frames and self-map their numbers to page numbers (see notes on the palloc() function for a discussion of self-mapping)

	// - If the address is becomming bigger, but it is not possible to allocate new contigous pages to extend the chunk size,
	//   use kmallloc() to allocate a new chunk, then use memcpy() to copy the original bytes to the new chunk, and return the new chunk's address.
	//   Before returning, free the old chunk (by merging their free space to another header)

	// Dummy code: you cannot use malloc/free/realloc
	// return realloc(address, size);

	int* new_data_pos = kmalloc(size);
	memcpy(new_data_pos, address, size);
	kfree(address);
	return new_data_pos;

}

void kfree(void *address) {
	// TODO:
	// - Make the space used by the address free
	// - Return any frames that have become unused with vm_unmap() and frame_deallocate()
	uint64_t* beg = (uint64_t*)(FRAME_ADDRESS(FRAME_NUMBER((char *)address)));
	uint64_t* block = (&address - 2 * sizeof(int64_t));
	while(beg[1] < block){
		beg = beg[1];
		if(beg + beg[0] + 2 * sizeof(int64_t) == block){
			beg[0] += block[0] + 2 * sizeof(int64_t);
		}
		else{
			block[1] = beg[1];
			beg[1] = block;
		}
	}

	// Dummy code: you cannot use malloc/free/realloc
	// return free(address);
}
