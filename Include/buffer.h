/* Wrap by Richard Cavell v1.0 */
/* Copyright (c) 2017 Richard John Foster Cavell */
/* buffer.h */

#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>

#include "config.h"
#include "options.h"

struct buffer_type
{
  ch_type *text;
  size_t buffer_size;
  size_t bufpos;
  size_t endpos;
  int empty;
};

extern struct buffer_type
    create_buffer(const struct options_type *options);

/* Call this at the start of the program.
   It registers a callback with atexit().
*/

extern int register_free_buffer(void);

#endif
