#pragma once
#include "status.h"
#include "settings.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

struct pbm
{
	int width;
	int width_b;
	int height;
	uint8_t buffer[MAX_HEIGHT][MAX_WIDTH_B];
};

enum status pbm_read(struct pbm *pbm, FILE *input);
enum status pbm_write(struct pbm *pbm, FILE *output);
enum status pbm_print(struct pbm *pbm, FILE *output);

