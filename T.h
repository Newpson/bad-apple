#pragma once
#include <stdint.h>

struct bpoint
{
	/* note that frames have been transposed */
	/* order is important! it will be written as xy (at least on x86) */
	uint8_t y: 6;
	uint8_t x: 2;
};

typedef struct bpoint T;

struct bpoint T_init();
int T_compare(T *a, T *b);
