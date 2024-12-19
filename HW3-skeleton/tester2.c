#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>

#include "kmalloc.h"
#include "ll_double.h"

#define MIN(X, Y) (X < Y ? X : Y);

#define NUM_THREADS 8

void *threadFunction(void * data) {
	// Allocate at most 512K in total
	//   8 threads should *not* be able to allocate 512KB within 4MB of memory because of the bookkeeping information
	//   kept by the internal implementation of kmalloc()
	int remaining_bytes = 524288;

	// Keeps track of the allocated chunks
	struct list allocated_chunks;

	ll_init(&allocated_chunks);

	// Allocation counter
	int counter = 0;

	while(remaining_bytes > 0) {
		// For every 10 kmalloc() calls, do a kfree() call
		if(counter % 10) {
			struct node *first = ll_remove_head(&allocated_chunks);

			if(first) {
				kfree(first->data);
			}
		}
		else {
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
	}

	// Frees all data previously allocated
	struct node *first;

	while((first = ll_remove_head(&allocated_chunks))) {
		kfree(first->data);
	}

	return NULL;
}

int main(void) {
	// Feed the random number generator
	srand(time(NULL));

	pthread_t threadNumber[NUM_THREADS];
	int threadData[NUM_THREADS];

	for(int i = 0; i < NUM_THREADS; i++) {
		threadData[i] = i;
	}

	pthread_attr_t config;

	pthread_attr_init(&config);
	pthread_attr_setdetachstate(&config, PTHREAD_CREATE_JOINABLE);

	// Launches all threads -- they are automatically started
	for(int i = 0; i < NUM_THREADS; i++) {
		if(pthread_create(&threadNumber[i], &config, threadFunction, (void *) &threadData[i]) != 0) {
			fprintf(stderr, "Error creating thread\n");

			return EXIT_FAILURE;
		}
	}

	// Blocks main thread until all others return

	for(int i = 0; i < NUM_THREADS; i++) {
		if(pthread_join(threadNumber[i], NULL) != 0) {
			fprintf(stderr, "Error joning threads\n");

			return EXIT_FAILURE;
		}
	}

	printf("All threads finished without crashing. That doesn't mean the program is right. Please check with debug.\n");
	return EXIT_SUCCESS;
}
