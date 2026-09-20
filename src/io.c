/*
 * I/O operations.
 * 
 * Copyright (C) 2026 Rayleigh Gamma
 * See LICENSE for licensing information.
 */

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS 1
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "rayutils/diagnostics/log.h"
#include "rayutils/io.h"

RayBool
ray_io_file_create(const char *const path, const RayBool create_if_exists, const RayBool verbose_log)
{
	assert(path != NULL);

	const char *open_option = create_if_exists ? "w" : "x";
	FILE *file = fopen(path, open_option);

	if (file == NULL)
	{
		RAY_LOG_ERRNO(verbose_log, "fopen(\"%s\", \"%c\")\n", path, open_option);
		return false;
	}

	if (fclose(file) == EOF)
	{
		RAY_LOG_ERRNO(verbose_log, "fclose(%p)\n", file);
		return false;
	}

	return true;
}

RayBool
ray_io_file_read(char *const path, char **buffer, const bool verbose_log)
{
	assert(path != NULL);
	assert(buffer != NULL);

	FILE *file = fopen(path, "rb");

	if (file == NULL)
	{
		RAY_LOG_ERRNO(verbose_log, "fopen(\"%s\", \"rb\")\n", path);
		return false;
	}

	bool return_value = false;

	if (fseek(file, 0L, SEEK_END) != 0)
	{
		RAY_LOG_ERRNO(verbose_log, "fseek(%p, 0L, %d)", file, SEEK_END);
		goto end;
	}

	const long file_size = ftell(file);

	if (file_size == -1L)
	{
		RAY_LOG_ERRNO(verbose_log, "ftell(%p)", file);
		goto end;
	}

	if (fseek(file, 0L, SEEK_SET) != 0)
	{
		RAY_LOG_ERRNO(verbose_log, "fseek(%p, 0L, %d)", file, SEEK_SET);
		goto end;
	}

	*buffer = calloc(file_size + 1, sizeof(char));

	if (*buffer == NULL)
	{
		RAY_LOG_ERRNO(verbose_log, "calloc(%zu, %zu)", file_size + 1, sizeof(char));
		goto end;
	}

	if (fread(*buffer, sizeof(char), file_size, file) != (size_t) file_size)
	{
		RAY_LOG_ERRNO(verbose_log, "fread(%p, %zu, %zu, %p)", *buffer, sizeof(char), file_size, file);
		free(*buffer);
		goto end;
	}

	return_value = true;

end:
	if (fclose(file) == EOF)
	{
		RAY_LOG_ERRNO(verbose_log, "fclose(file%c", ')');
	}

	return return_value;
}
