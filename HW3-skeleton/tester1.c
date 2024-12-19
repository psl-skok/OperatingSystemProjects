#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string.h>
#include<pthread.h>

#include "kmalloc.h"
#include "ll_double.h"

#define MIN(X, Y) (X < Y ? X : Y);

int main(void) {
	// Feed the random number generator
	srand(time(NULL));

	// Allocate at most 10K in total
	int remaining_bytes = 41960;

	// Keeps track of the allocated chunks
	struct list allocated_chunks;

	ll_init(&allocated_chunks);

	while(remaining_bytes > 0) {
		// Allocate between 1 and 4K bytes, up to the maximum of remaining bytes
		int chunk_size = MIN(remaining_bytes, (rand() % 4096) + 1);

		char *memory = kmalloc(chunk_size);

		if(!memory) {
			fprintf(stderr, "Memory is full (this is OK with limited memory), exiting");
			fflush(stdout);
			break;
		}

		remaining_bytes -= chunk_size;
		ll_insert_head(&allocated_chunks, memory);
	}

	// Frees all data previously allocated
	struct node *first;

	while((first = ll_remove_head(&allocated_chunks))) {
		kfree(first->data);
	}

	printf("Exiting without crashing. That doesn't mean the program is right. Please check with debug.\n");
	return EXIT_SUCCESS;
}
