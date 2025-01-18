#include "T.h"

struct bpoint T_init()
{
	return (struct bpoint) {0};
}

/* strcmp() logic */
int T_compare(T *a, T *b)
{
	return a->x == b->x && a->y == b->y ? 0 : 1;
}
