#ifndef KMALLOC_H
#define KMALLOC_H

#include<stdint.h>

/**
 * Allocate @p number pages/frames of memory.
 *
 * @param number Number of pages/frames of memory that need to be allocated.
 *
 * @return Address of the first allocated byte.
 */
void *palloc(uint64_t number);

/**
 * Deallocates @p number pages/frames of memory previously allocated with palloc.
 *
 * @param address Address of the first allocated byte previously allocated with palloc.
 * @param number Number of pages/frames of memory that need to be deallocated (assumed to match what was used in palloc).
 */
void pfree(void* address, uint64_t number);

/**
 * Allocates @p size bytes of memory.
 *
 * @param size Number of bytes of memory that need to be allocated.
 *
 * @return Address of the first allocated byte.
 */
void *kmalloc(uint64_t size);

/**
 * Changes the size (and potentially the location) of a chunk of memory previously allocated with kmalloc.
 *
 * @param address Address of the first allocated byte previously allocated with kmalloc.
 * @param size Number of bytes of memory in the new area. If the number of bytes is bigger than what was used in kmalloc,
 *               the chunk of memory might need to be moved to a different location. If the number of bytes is not bigger
 *               than what was used in kmalloc, the chunk of memory cannot be moved to a different location.
 *
 * @return Address of the first allocated byte of the resized chunk of memory.
 */
void *krealloc(void *address, uint64_t size);

/**
 * Deallocates memory previously allocated with kmalloc.
 *
 * @param address Address of the first allocated byte previously allocated with kmalloc.
 */
void kfree(void *address);

#endif /* KMALLOC_H */
