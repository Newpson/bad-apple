#pragma once
#include "pbm.h"
#include "dllist.h"
#include <stdint.h>

struct diff
{
	uint8_t byte;
	struct node *pos;
};

struct comparator
{
	struct pbm pbm[2];
	struct pbm *previous;
	struct pbm *current;
	struct dllist *difflist;
	struct diff buffer[MAX_HEIGHT][MAX_WIDTH_B];
	int diffc;
};

enum status comparator_init(struct comparator *comp);
void comparator_free(struct comparator *comp);

void comparator_compare(struct comparator *comp);

enum status comparator_read(struct comparator *comp, FILE *frame);
enum status comparator_write(struct comparator *comp, FILE *output);
