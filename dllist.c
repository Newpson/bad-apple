#include "dllist.h"
#include <stdlib.h>
#include <stddef.h>

struct node *node_init(struct node *prev, struct node *next, T value)
{
	struct node *node = malloc(sizeof(struct node));
	node->prev = prev;
	node->next = next;
	node->value = value;
	return node;
}

struct dllist *dllist_init(void)
{
	struct dllist *list = malloc(sizeof(struct dllist));
	list->size = 0;
	list->tail = node_init(NULL, NULL, T_init());
	list->tail->next = list->tail->prev = list->tail;
	return list;
}

void dllist_free(struct dllist *list)
{
	if (list)
	{
		struct node *i = list->tail;
		do
		{
			struct node *g = i;
			i = i->next;
			free(g);
		}
		while (i != list->tail);
		free(list);
	}
}

void dllist_push_front(struct dllist *list, T value)
{
	list->tail->next = node_init(list->tail, list->tail->next, value);
	list->tail->next->next->prev = list->tail->next;
	++list->size;
}

void dllist_push_back(struct dllist *list, T value)
{
	list->tail->value = value;
	list->tail->next = node_init(list->tail, list->tail->next, T_init());
	list->tail = list->tail->next;
	list->tail->next->prev = list->tail;
	++list->size;
}

void dllist_pop_front(struct dllist *list)
{
	struct node *garbage = list->tail->next;
	garbage->next->prev = list->tail;
	list->tail->next = garbage->next;
	free(garbage);
	--list->size;
}

void dllist_pop_back(struct dllist *list)
{
	struct node *garbage = list->tail;
	garbage->next->prev = garbage->prev;
	list->tail = garbage->prev;
	list->tail->next = garbage->next;
	free(garbage);
	--list->size;
}

T dllist_front(struct dllist *list)
{
	return list->tail->next->value;
}

T dllist_back(struct dllist *list)
{
	return list->tail->prev->value;
}

struct node *dllist_begin(struct dllist *list)
{
	return list->tail->next;
}

struct node *dllist_end(struct dllist *list)
{
	return list->tail;
}

struct node *dllist_find(struct dllist *list, T value)
{
	for (struct node *i = dllist_begin(list); i != dllist_end(list); i = i->next)
	{
		if (T_compare(&i->value, &value))
		{
			return i;
		}
	}
	return NULL;
}

void dllist_push(struct dllist *list, struct node *i, T value)
{
	struct node *node = node_init(i->prev, i, value);
	i->prev->next = node;
	i->prev = node;
	++list->size;
}

void dllist_pop(struct dllist *list, struct node *i)
{
	i->prev->next = i->next;
	i->next->prev = i->prev;
	free(i);
	--list->size;
}

size_t dllist_size(struct dllist *list)
{
	return list->size;
}

bool dllist_empty(struct dllist *list)
{
	return list->size == 0;
}
