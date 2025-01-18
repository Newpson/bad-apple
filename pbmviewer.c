#include "pbm.h"

int main(int argc, char *argv[])
{
	FILE *input = fopen(argv[1], "rb");
	if (!input)
	{
		return 1;
	}

	pbm_t pbm;
	if (!pbm_read(&pbm, input))
	{
		return 1;
	}
	pbm_print(&pbm, stdout);

	fclose(input);
	return 0;
}
