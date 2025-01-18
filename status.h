#pragma once

/* #define DEBUGF(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__) */
#define ERROR(parent, message) fprintf(stderr, "[E]\t%s: %s\n", parent, message)

enum status
{
	STATUS_OK,
	STATUS_ERROR,
};
