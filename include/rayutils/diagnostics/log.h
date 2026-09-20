/*
 * Log macros, you can define your own systems, categories and reasons using
 * enums.
 * 
 * Copyright (C) 2026 Rayleigh Gamma
 * See LICENSE for licensing information.
 */

#ifndef RAYUTILS_DIAGNOSTICS_LOG_H
#define RAYUTILS_DIAGNOSTICS_LOG_H

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "rayutils/diagnostics/span.h"
#include "rayutils/types.h"

#define RAY_LOG_METADATA_CODE(system, category, reason) ((system << 12) | (category << 8) | (reason))

#define RAY_LOG_ERRNO(verbose_log, function_format, ...)																										\
	ray_log( 																																					\
		RAY_LOG_METADATA_OPTION_INTERNAL | RAY_LOG_METADATA_OPTION_ERROR | RAY_LOG_METADATA_OPTION_ERRNO | (RAY_LOG_METADATA_OPTION_VERBOSE * verbose_log) |	\
		RAY_LOG_METADATA_CODE(0, 0, errno),																														\
		stderr, 																																				\
		(RaySpan) { .stream = __FILE__, .function = __func__, .line = __LINE__, .column = 0 },																	\
		function_format " failed: %s.\n", 																														\
		__VA_ARGS__, strerror(errno)				 																											\
	)

#define RAY_LOG_METADATA_CODE_SYS_CUSTOM 1
#define RAY_LOG_METADATA_CODE_CAT_CUSTOM 1

/* Options */
enum {
	RAY_LOG_METADATA_OPTION_WARNING = 1 << 0,
	RAY_LOG_METADATA_OPTION_ERROR = 1 << 1,
	RAY_LOG_METADATA_OPTION_INTERNAL = 1 << 2,
	RAY_LOG_METADATA_OPTION_ERRNO = 1 << 3,
	RAY_LOG_METADATA_OPTION_VERBOSE = 1 << 4,
};

void ray_log(const RayU8 metadata, FILE *const file, const RaySpan span, char *const message_format, ...);

#endif /* RAYUTILS_DIAGNOSTICS_LOG_H */
