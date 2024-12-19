#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ll_double.h"

void ll_init(struct List *list) {
	list->head = NULL;
	list->tail = NULL;
}

struct Node *ll_insert_head(struct List *list, struct request *data) {
	// Allocate a new node
	struct Node *new_node = (struct Node *) malloc( sizeof(struct Node) );

	// It is a good idea to initialize next and prev pointers right after allocation,
	// because otherwise they will contain garbage.
	new_node->prev = NULL;
	new_node->next = NULL;

	// The result of allocations can be NULL if you are out of memory.
	// That's why we have this if statement. Code like this in other functions too.
	if(new_node != NULL) {
		// Store the character data
		new_node->data = data;

		// If the head is NULL, the list is empty:
		//     Just make head and tail point to the node	
		if(list->head == NULL){
			list->head = new_node;
			list->tail = new_node;	
		}
		// Else, the next of new_node is head; the prev of head is new_node
		else{
			list->head->prev = new_node;
			new_node->next = list->head;
			list->head = new_node;
		}
	}

	return new_node;
}


struct Node *ll_remove_tail(struct List *list) {
	// Bizarro world version of the previous function
	struct Node *result = list->tail;

	if(result == NULL){
		return NULL;
	}

	// When you detach the tail, move the tail pointer of the list backward.
	list->tail = result->prev;

	// If the head is not NULL, the list is non-empty
	//     Make sure prev pointers are correct!
	if(list->tail != NULL){
		list->tail->prev = NULL;
	}
	
	// If the head became NULL, the list is empty
	//     Buf if your list is empty, just reinitialize the whole thing!
	else{
		ll_init(list);
	}

	return result;
}
