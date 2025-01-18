#include "comparator.h"
#include "pbm.h"
#include "dllist.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <float.h>

enum status comparator_init(struct comparator *comp)
{
	comp->diffc = 0;
	comp->previous = comp->pbm+0;
	comp->current = comp->pbm+1;

	comp->difflist = dllist_init();
	for (int i = 0; i < MAX_HEIGHT; ++i)
	{
		for (int j = 0; j < MAX_WIDTH_B; ++j)
		{
			comp->buffer[i][j].byte = 0;
			comp->buffer[i][j].pos = NULL;
		}
	}

	return STATUS_OK;
}

void comparator_free(struct comparator *comp)
{
	dllist_free(comp->difflist);
}

static
void comparator_swap_frames(struct comparator *comp)
{
	struct pbm *temp = comp->previous;
	comp->previous = comp->current;
	comp->current = temp;
}

void comparator_compare(struct comparator *comp)
{
	comp->diffc = 0;
	for (int i = 0; i < MAX_HEIGHT; ++i)
	{
		for (int j = 0; j < MAX_WIDTH_B; ++j)
		{
			if (comp->previous->buffer[i][j] != comp->current->buffer[i][j])
			{
				++comp->diffc;
				comp->buffer[i][j].byte = comp->current->buffer[i][j];
				if (comp->buffer[i][j].pos)
				{
					dllist_pop(comp->difflist, comp->buffer[i][j].pos);
				}
				dllist_push_front(comp->difflist, (struct bpoint) {.y = i, .x = j});
				comp->buffer[i][j].pos = dllist_begin(comp->difflist);
			}
		}
	}
}

enum status comparator_read(struct comparator *comp, FILE *frame)
{
	comparator_swap_frames(comp);
	if (pbm_read(comp->current, frame) != STATUS_OK)
	{
		ERROR("comparator", "Error reading frame");
		return STATUS_ERROR;
	}
	return STATUS_OK;
}

static
void comparator_write_node(struct comparator *comp, struct node *node, FILE *output)
{
	struct bpoint p = node->value;
	fwrite(&p, sizeof(struct bpoint), 1, output);
	fwrite(&comp->buffer[p.y][p.x].byte, sizeof(uint8_t), 1, output);
	comp->buffer[p.y][p.x].pos = NULL;
}

enum status comparator_write(struct comparator *comp, FILE *output)
{
	static const uint8_t BYTE_EOD = 0xFF; /* end-of-differences reserved byte */

	if (!output)
	{
		ERROR("comparator", "Cannot write file");
		return STATUS_ERROR;
	}
	
	/* Bresenham's line algorithm logic. */
	/* Changes are only written when moving along x-axis. */
	float k = (float) comp->diffc / MAX_DIFFC;

	struct node *node = dllist_begin(comp->difflist);
	int x = 1; /* aka `written` */
	for (int y = 1; y <= comp->diffc && x <= MAX_DIFFC && node != dllist_end(comp->difflist); ++y)
	{
		if ((float)y/x > k) /* don't care if k <= 1 i.e. diffc <= MAX_DIFFC */
		{
			comparator_write_node(comp, node, output);
			struct node *fallback = node; /* avoiding use-after-free */
			node = node->next;
			dllist_pop(comp->difflist, fallback);
			++x;
		}
		else
		{
			node = node->next;
		}
	}
	
	node = dllist_end(comp->difflist)->prev;
	while (x <= MAX_DIFFC && !dllist_empty(comp->difflist))
	{
		comparator_write_node(comp, node, output);
		node = node->prev;
		dllist_pop_back(comp->difflist);
		++x;
	}

	if (x <= MAX_DIFFC)
	{
		fwrite(&BYTE_EOD, sizeof(uint8_t), 1, output);
	}

	return STATUS_OK;
}

