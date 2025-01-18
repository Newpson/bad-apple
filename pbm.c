#include "pbm.h"
#include "status.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

static inline
enum status pbm_check_magic(FILE *input)
{
	char p4[2];
	fread(p4, sizeof(char), 2, input);
	if (strncmp(p4, "P4", 2))
	{
		ERROR("pbm", "Wrong magic bytes");
		return STATUS_ERROR;
	}
	return STATUS_OK;
}

static inline
enum status pbm_read_wh(int *w, int *h, FILE *input)
{
	fscanf(input, "%d %d ", w, h);
	if (*w < 0 || *h < 0 || *w > MAX_WIDTH || *h > MAX_HEIGHT)
	{
		ERROR("pbm", "Wrong width/height");
		return STATUS_ERROR;
	}
	return STATUS_OK;
}

enum status pbm_read(struct pbm *pbm, FILE *input)
{
	if (!input)
	{
		ERROR("pbm", "Cannot read file");
		return STATUS_ERROR;
	}

	if (pbm_check_magic(input) != STATUS_OK)
	{
		ERROR("pbm", "Error reading magic bytes");
		return STATUS_ERROR;
	}

	if (pbm_read_wh(&pbm->width, &pbm->height, input) != STATUS_OK)
	{
		ERROR("pbm", "Error reading width/height");
		return STATUS_ERROR;
	}
	pbm->width_b = to_bytes(pbm->width);

	for (int i = 0; i < pbm->height; ++i)
	{
		fread(pbm->buffer+i, sizeof(uint8_t), pbm->width_b, input);
	}

	return STATUS_OK;
}

enum status pbm_write(struct pbm *pbm, FILE *output)
{
	if (!output)
	{
		ERROR("pbm", "Cannot write file");
		return STATUS_ERROR;
	}

	fprintf(output, "P4 %d %d ", pbm->width, pbm->height);

	for (int i = 0; i < pbm->height; ++i)
	{
		fwrite(pbm->buffer+i, sizeof(uint8_t), pbm->width_b, output);
	}

	return STATUS_OK;
}

enum status pbm_print(struct pbm *pbm, FILE *output)
{
	if (!output)
	{
		ERROR("pbm", "Cannot write file");
		return STATUS_ERROR;
	}

	for (int i = 0; i < pbm->height; ++i)
	{
		for (int j = 0, x = 0; j < pbm->width_b; ++j)
		{
			for (int b = 7; b >= 0 && x < pbm->width; --b, ++x)
			{
				fprintf(output, "%s", (pbm->buffer[i][j] >> b)&1 ? "--" : "@@");
			}
		}
		printf("\n");
	}

	return STATUS_OK;
}
