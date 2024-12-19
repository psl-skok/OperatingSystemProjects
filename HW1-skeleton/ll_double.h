#ifndef LL_DOUBLE_H
#define LL_DOUBLE_H

struct Node {
	struct request *data;

	struct Node *prev;
	struct Node *next;
};

struct List {
	struct Node *head;
	struct Node *tail;
};

void ll_init(struct List *list);
struct Node *ll_insert_head(struct List *list, struct request *data);
struct Node *ll_remove_tail(struct List *list);

#endif /* LL_DOUBLE_H */
