#ifndef RAYUTILS_DIAGNOSTICS_SPAN_H
#define RAYUTILS_DIAGNOSTICS_SPAN_H

#include <stddef.h>

typedef struct {
	char *stream;
	const char *function;
	size_t line;
	size_t column;
} RaySpan;

#endif /* RAYUTILS_DIAGNOSTICS_SPAN_H */
