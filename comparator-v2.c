#include "pbm.h"
#include "status.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define WIDTH_OFFSET_B 2
#define HEIGHT_OFFSET 16 
#define DIFF_THRESHOLD 8 /* `1` means save all frames even if only one pixel differs */

static
int chunk_byte_ones(uint8_t byte)
{
	static const uint8_t NIBBLE_LOOKUP [16] =
	{
		0, 1, 1, 2, 1, 2, 2, 3,
		1, 2, 2, 3, 2, 3, 3, 4
	};
	
	return NIBBLE_LOOKUP[byte & 0x0F] + NIBBLE_LOOKUP[byte >> 4];
}

/* returns number of bits differs */
static
int chunk_compare(uint8_t chunka[8], uint8_t chunkb[8])
{
	int count = 0;
	for (int i = 0; i < 8; ++i)
	{
		count += chunk_byte_ones(chunka[i] ^ chunkb[i]);
	}
	return count;
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		ERROR("diffcat", "Invalid arguments. Usage: diffcat FRAME_1 [...]");
		return STATUS_ERROR;
	}

	const int framesc = argc-1;
	uint8_t chunks[framesc][8];

	FILE *frame;
	struct pbm pbm;
	for (int i = 1; i < argc; ++i)
	{
		frame = fopen(argv[i], "rb");

		pbm_read(&pbm, frame);
		for (int j = 0; j < 8; ++j)
		{
			chunks[i-1][j] = pbm.buffer[j+HEIGHT_OFFSET][WIDTH_OFFSET_B]; /* read top left 8x8 square */
		}

		fclose(frame);
	}

	/* unique chunks */
	uint8_t uchunks[framesc][8];
	memset(uchunks, 0, framesc*8);
	int uniquec = 0;

	for (int i = 0; i < framesc-1; ++i)
	{
		bool exists = false;
		for (int j = 0; j < uniquec; ++j)
		{
			if (chunk_compare(chunks[i], uchunks[j]) < DIFF_THRESHOLD)
			{
				exists = true;
			}
		}

		if (!exists)
		{
			memcpy(uchunks[uniquec++], chunks[i], 8);
		}
	}

	printf("Number of unique chunks: %d (out of %d)\n", uniquec, framesc);
	for (int k = 0; k < uniquec; ++k)
	{
		for (int i = 0; i < 8; ++i)
		{
			for (int b = 7; b >= 0; --b)
			{
				printf("%s", (uchunks[k][i] >> b)&1 ? "--" : "@@");
			}
			printf("\n");
		}
		printf("\n");
	}

	return STATUS_OK;
}
