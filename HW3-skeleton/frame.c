#include "frame.h"
#include <stdio.h>
#include <string.h>
#define bitmap_size 1024/8

uint64_t frames_allocated;

uint64_t frames_available;

// No need to change or initialize this. This is "empty" space that simulate your physical memory.
_Alignas(4096) char memory[MEMORY_SIZE];


// You should define a bitmap to keep track of the allocated frames here.
// Look at the handout for details on size
uint8_t bitmap[1024/8];

void frame_init() {
	// Initialize global variables
	// Initialize the bitmap
	frames_allocated = 0;
	frames_available = 1024;
	memset(bitmap,0,bitmap_size);
}

void bit_set(uint64_t frame_number){
	//take one bit in position and set it to 1 using left shift opperation (1<<3)
	bitmap[frame_number / 8] |= 1 << (frame_number % 8);
}

void bit_unset(uint64_t frame_number){
	//take one bit and set it to 0
	bitmap[frame_number / 8] &= ~(1 << (frame_number % 8));
}

int get_value(uint64_t frame_number){
	return (bitmap[frame_number / 8] >> (frame_number % 8)) & 1;
}

int64_t allocate_frame(int number_frames) {
	// Consult the bitmap and return the first available frame number, marking it as allocated
	// Increase the frames_allocated, decrease frames_available
	int consecutive_counter = 0;
	for(int i = 0; i < 1024; i++){
		if(get_value(i) == 0){
			consecutive_counter++;
			if(consecutive_counter == number_frames){
				for(int j = 0; j < number_frames; j++){
					bit_set(i - number_frames + 1);
				}
				frames_allocated = frames_allocated + number_frames;
				frames_available = frames_available - number_frames;
				return i - number_frames + 1;
			}
		}
		else{
			consecutive_counter = 0;
		}
	}
	return -1; // Return according to what's documented in the header file for this module
}

int64_t deallocate_frame(uint64_t frame_number, int number_frames) {
	// Mark the frame as deallocated in the bitmap
	// Decrease the frames_allocated, increase frames_available
	for(int i = 0; i < number_frames; i++){
		bit_unset(frame_number);
		frames_allocated--;
		frames_available++;
	}

	return -1; // Return according to what's documented in the header file for this module
}
