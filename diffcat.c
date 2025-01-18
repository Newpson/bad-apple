#include "comparator.h"
#include "status.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	if (argc < 4)
	{
		ERROR("diffcat", "Invalid arguments. Usage: diffcat FRAME_1 FRAME_2 [...] OUTPUT");
		return STATUS_ERROR;
	}

	struct comparator comp;
	if (comparator_init(&comp) != STATUS_OK)
	{
		ERROR("diffcat", "Error initializing comparator");
		return STATUS_ERROR;
	}

	printf("Reading (1): %s\n", argv[1]);
	FILE *frame = fopen(argv[1], "rb");
	if (comparator_read(&comp, frame) != STATUS_OK)
	{
		ERROR("diffcat", "Error reading first frame");
		return STATUS_ERROR;
	}
	fclose(frame);

	FILE *output = fopen(argv[argc-1], "wb");
	for (int i = 2; i < argc-1; ++i)
	{
		printf("Reading (%d): %s\n", i, argv[i]);
		frame = fopen(argv[i], "rb");
		if (comparator_read(&comp, frame) != STATUS_OK)
		{
			ERROR("diffcat", "Error reading next frames");
			return STATUS_ERROR;
		}
		fclose(frame);

		comparator_compare(&comp);

		if (comparator_write(&comp, output) != STATUS_OK)
		{
			ERROR("diffcat", "Error writing differences to output file");
			return STATUS_ERROR;
		}
	}
	fclose(output);
	comparator_free(&comp);

	return STATUS_OK;
}
