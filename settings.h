#pragma once

#define to_bytes(w) ((w)/8+((w)%8>0))

#define MAX_WIDTH 32
#define MAX_HEIGHT 63
#define MAX_WIDTH_B to_bytes(MAX_WIDTH)
#define MAX_DIFFC 14
