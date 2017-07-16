/* Wrap by Richard Cavell v1.0 */
/* Copyright (c) 2017 Richard John Foster Cavell */
/* file.h */

#ifndef FILE_H
#define FILE_H

#include "buffer.h"
#include "options.h"

/* Pass NULL for fn to use stdin */
extern int open_file(const char *fn, struct buffer_type *buffer,
                     const struct options_type *options);

/* This must be called at the start of the program. */
extern int register_close_file(void);

#endif
