#include "settings.h"
#include "status.h"
#include "T.h"
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#define gotoyx(y, x) printf("\033[%u;%uf", (y), (x))
#define setcursor() printf("\033[?25h")
#define nocursor() printf("\033[?25l")
#define erase() printf("\033[2J")
#define hlight() printf("\033[7m")
#define normal() printf("\033[27m")

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		ERROR("framescat", "Invalid arguments. Usage: frameviewer FRAMESBIN");
		return STATUS_ERROR;
	}

	FILE *frames = fopen(argv[1], "rb");
	if (!frames)
	{
		ERROR("framescat", "Error reading binary file");
		return STATUS_ERROR;
	}

	struct bpoint EOD = {.x = 3, .y = 63};
	struct bpoint p;
	uint8_t byte;

	while (!feof(frames))
	{
		for (int i = 0; i < MAX_DIFFC; ++i)
		{
			fread(&p, sizeof(struct bpoint), 1, frames);
			if (T_compare(&p, &EOD)) /* not EOD but _transposed_ coords */
			{
				fread(&byte, sizeof(uint8_t), 1, frames); /* difference */
				int y = 1+8*p.x;
				for (int b = 0; b < 8; ++b)
				{
					gotoyx(y+b, 2*p.y);
					printf("%s", (byte >> (7-b))&1 ? "  " : "██");
				}
			}
			else
			{
				break;
			}
		}
		usleep(100000);
	}

	fclose(frames);

	return STATUS_OK;
}
