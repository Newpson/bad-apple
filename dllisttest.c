#include "dllist.h"
#include <stdio.h>

void dllist_print(dllist_t *list)
{
	printf("(");
	for (node_t *i = dllist_begin(list); i != dllist_end(list); i = i->next)
	{
		printf("%d", i->value);
		if (i->next != dllist_end(list))
		{
			printf(", ");
		}
	}
	printf(")\n");
}

int main()
{
	dllist_t *list = dllist_init();

	dllist_push_back(list, 23);
	dllist_push_back(list, 24);
	dllist_push_back(list, 25);
	dllist_print(list);

	dllist_pop_front(list);
	dllist_pop_front(list);
	dllist_print(list);

	dllist_push_front(list, 13);
	dllist_push_front(list, 14);
	dllist_print(list);

	node_t *node = dllist_find(list, 13);
	if (node)
	{
		dllist_push(list, node, 111);
	}
	else
	{
		printf("Node not found\n");
	}
	dllist_print(list);

	node = dllist_find(list, 10);
	if (node)
	{
		dllist_pop(list, node);
	}
	else
	{
		printf("Node not found\n");
	}
	dllist_print(list);

	node = dllist_find(list, 13);
	if (node)
	{
		dllist_pop(list, node);
	}
	else
	{
		printf("Node not found\n");
	}
	dllist_print(list);

	dllist_free(list);
	return 0;
}
