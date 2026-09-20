/*
 * Log function implementation.
 * 
 * Copyright (C) 2026 Rayleigh Gamma
 * See LICENSE for licensing information.
 */

#include <assert.h>
#include <stdarg.h>

#include "rayutils/diagnostics/log.h"

#define RAY_LOG_METADATA_GET_CODE(metadata) ((metadata) >> 16)
#define RAY_LOG_METADATA_OPTIONS_MASK 0x00ff

void
ray_log(const RayU8 metadata, FILE *const file, const RaySpan span, char *const message_format, ...)
{
	assert(file != NULL);
	assert(message_format != NULL);

	const RayU16 code = RAY_LOG_METADATA_GET_CODE(metadata);
	const RayU8 options = metadata & RAY_LOG_METADATA_OPTIONS_MASK;

	if (fputc('[', file) == EOF)
	{
			perror("Failed to log: fputc failed");
	}

	if ((options & RAY_LOG_METADATA_OPTION_ERRNO) != 0)
	{
		if (fputc('N', file) == EOF)
		{
			perror("Failed to log: fputc failed");
			return;
		}
	}

	if ((options & RAY_LOG_METADATA_OPTION_INTERNAL) != 0)
	{
		if (fputc('I', file) == EOF)
		{
			perror("Failed to log: fputc failed");
			return;
		}
	}

	char warning_level_character = '\0';

	switch (options & 3) {
		case RAY_LOG_METADATA_OPTION_WARNING: {
			warning_level_character = 'W';
		} break;

		case RAY_LOG_METADATA_OPTION_ERROR: {
			warning_level_character = 'E';
		} break;
	}

	if (fprintf(file, "%c] ", warning_level_character) < 0)
	{
		perror("Failed to log: fprintf failed");
		return;
	}

	if ((options & RAY_LOG_METADATA_OPTION_VERBOSE) != 0)
	{
		if (fprintf(file, "(code: 0x%.4x) ", code) < 0)
		{
			perror("Failed to log: fprintf failed");
			return;
		}
	}

	if ((options & RAY_LOG_METADATA_OPTION_ERRNO) != 0)
	{
		if (fputs("Internal error ", file) == EOF) {
			perror("Failed to log: fputs failed");
			return;
		}
	}

	if (fprintf(file, "at %s", span.stream) < 0)
	{
		perror("Failed to log: fprintf failed");
		return;
	}

	if (fprintf(file, "[%zu", span.line) < 0)
	{
		perror("Failed to log: fprintf failed");
		return;
	}

	if (span.column > 0)
	{
		if (fprintf(file, ":%zu", span.column) < 0)
		{
			perror("Failed to log: fprintf failed");
			return;
		}
	}

	if (fprintf(file, "]") < 0)
	{
		perror("Failed to log: fprintf failed");
		return;
	}

	if (span.function != NULL)
	{
		if (fprintf(file, " in %s(...)", span.function) < 0)
		{
			perror("Failed to log: fprintf failed");
			return;
		}
	}

	if (fprintf(file, ": ") < 0)
	{
		perror("Failed to log: fprintf failed");
		return;
	}

	va_list arguments;
	va_start(arguments, message_format);

	if (vfprintf(file, message_format, arguments) < 0)
	{
		perror("Failed to log: vprintf failed");
	}

	va_end(arguments);
}
