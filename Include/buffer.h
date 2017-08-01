/* Wrap by Richard Cavell (under development) */
/* Copyright (c) 2017 Richard John Foster Cavell */
/* buffer.h */

#ifndef BUFFER_H
#define BUFFER_H

#include <stdio.h>
#include <stddef.h>

#include "config.h"
#include "options.h"

/*
   A single object of this type will be created at the start of
   the program, and passed to anything that works with the buffer.
*/

struct buffer_type
{
  ch_type *text;
  size_t buffer_size;
  size_t startpos;
  size_t endpos;
  int empty;
};

/*
   Call this at the start of the program.
   It creates the text buffer.
*/

extern struct buffer_type
create_buffer(const struct options_type *options);

/*
   Call this at the start of the program.
   It registers a callback with atexit().
*/

extern int register_free_buffer(void);

/* Use this in place of getchar() */
extern int xgetchar(FILE *fp, struct buffer_type *buffer);

#endif
