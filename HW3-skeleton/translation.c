#include<stdio.h>
#include <math.h>
#include "frame.h"
#include "translation.h"

void *table_root = NULL;

entry *make_node(){
	entry *node = (entry *) FRAME_ADDRESS(allocate_frame(1));
	memset(node, 0, 4096);
	return node;
}

int vm_map(uint64_t page, uint64_t frame, int number, uint16_t flags) {
	if(page == 0){
		return 0;
	}

	if(table_root == NULL){
		table_root = make_node();
	}

	for(int i = 0; i < number; i++){
		page += i;

		uint64_t a = page & 0x1FF;
		uint64_t b = (page >> 9) & 0x1FF;
		uint64_t c = (page >> 18) & 0x1FF;
		uint64_t d = (page >> 27) & 0x1FF;

		entry *first_pointer = table_root + a;
		if(first_pointer->address == 0){
			first_pointer->address = (uint64_t)new_node();
		}

		entry *second_pointer = first_pointer->address + b;
		if(second_pointer->address == 0){
			second_pointer->address = (uint64_t)new_node();
		}

		entry *third_pointer = second_pointer->address + c;
		if(third_pointer->address == 0){
			third_pointer->address = (uint64_t)new_node();
		}

		entry *fourth_pointer = third_pointer->address + d;
		if(fourth_pointer->address == 0){
			fourth_pointer->address = frame;
			fourth_pointer->flags = 1;
		}
		else{
			return 0;
		}
	}

	return 1;
}

entry *vm_find(uint64_t page){//return the point to the final entry or null if it doesnt exist, use this function to impliment unmap in like 2 lines of code
	uint64_t a = page & 0x1FF;
	uint64_t b = (page >> 9) & 0x1FF;
	uint64_t c = (page >> 18) & 0x1FF;
	uint64_t d = (page >> 27) & 0x1FF;

	if(page == 0){
		return 0;
	}

	entry *first_pointer = table_root + a;
	if(first_pointer->address == 0){
		return NULL;
	}

	entry *second_pointer = first_pointer->address + b;
	if(second_pointer->address == 0){
		return NULL;
	}

	entry *third_pointer = second_pointer->address + c;
	if(third_pointer->address == 0){
		return NULL;
	}

	entry *fourth_pointer = third_pointer->address + d;
	if(fourth_pointer->flags == 1){
		return fourth_pointer;
	}
	else{
		return NULL;
	}
} 

int vm_unmap(uint64_t page, int number) {
	if(page == 0){
		return 0;
	}

	for(int i = 0; i < number; i++){
		page += i;

		entry *last_pointer = vm_find(page);
		if(last_pointer->flags == 1){
			last_pointer->flags = 0;
			return 1;
		}
	}
	
	return 0;
}

uint64_t vm_locate(int number) {
	int counter = 0;
	for(int i = 1; i <= pow(2,36); i++){
		entry *current_page = vm_find(i);
		if(current_page->flags == 0 && counter == number){
			return i;
		}
		else if(current_page->flags == 0){
			counter++;
		}
		else{
			counter = 0;
		}

	}
	return 0;
}

uint64_t vm_translate(uint64_t virtual_address) {
	uint64_t virtual_page_number = virtual_address >> 12;
	uint64_t offset = virtual_address & 0xFFF;

	entry *physical_page_number = vm_find(virtual_page_number);
	if(physical_page_number == NULL){
		return UINT64_MAX;
	}
	else{
		return (physical_page_number->address << 12) | offset;
	}
}
