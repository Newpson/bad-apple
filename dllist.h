#pragma once
#include "T.h"
#include <stddef.h>
#include <stdbool.h>

struct node
{
	struct node *prev;
	struct node *next;
	T value;
};

struct dllist
{
	size_t size;
	struct node *tail;
};

struct node *node_init(struct node *prev, struct node *next, T value);

struct dllist *dllist_init(void);
void dllist_free(struct dllist *dllist);

void dllist_push_front(struct dllist *list, T value);
void dllist_push_back(struct dllist *list, T value);
void dllist_pop_front(struct dllist *list);
void dllist_pop_back(struct dllist *list);
T dllist_front(struct dllist *list);
T dllist_back(struct dllist *list);

struct node *dllist_begin(struct dllist *list);
struct node *dllist_end(struct dllist *list);

struct node *dllist_find(struct dllist *list, T value);
void dllist_push(struct dllist *list, struct node *i, T value);
void dllist_pop(struct dllist *list, struct node *i);

size_t dllist_size(struct dllist *list);
bool dllist_empty(struct dllist *list);
