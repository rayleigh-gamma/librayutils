/*
 * I/O operations.
 * 
 * Copyright (C) 2026 Rayleigh Gamma
 * See LICENSE for licensing information.
 */

#ifndef RAYUTILS_IO_H
#define RAYUTILS_IO_H

#include "rayutils/types.h"

RayBool ray_io_file_create(const char *const path, const RayBool create_if_exists, const RayBool verbose_log);
RayBool ray_io_file_read(char *const path, char **buffer, const bool verbose_log);

#endif /* RAYUTILS_IO_H */
