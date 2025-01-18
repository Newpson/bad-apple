#include "pbm.h"

int main(int argc, char *argv[])
{
	FILE *input = fopen(argv[1], "rb");
	if (!input)
	{
		return 1;
	}

	pbm_t pbm;
	printf("Reading %s\n", argv[1]);
	if (!pbm_read(&pbm, input))
	{
		return 1;
	}
	pbm_print(&pbm, stdout);
	fclose(input);

	FILE *output = fopen("test.pbm", "wb");
	printf("Writing to test.pbm\n");
	pbm_write(&pbm, output);
	fclose(output);

	return 0;
}
