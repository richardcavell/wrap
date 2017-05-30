/* Wrap by Richard Cavell v1.0 */
/* Copyright (c) 2017 Richard John Foster Cavell */
/* buffer.c */

#include <stdlib.h>

#include "buffer.h"

#include "error.h"
#include "options.h"

static ch_type *text;

struct buffer_type
create_buffer(const struct options_type *options)
{
  struct buffer_type buffer;

  buffer.text = malloc(options->buffer_size);
  text = buffer.text;

  if (buffer.text == NULL)
    fail_msg("Error: Couldn't acquire %lu bytes for the buffer\n",
             (unsigned long int) options->buffer_size);

  buffer.buffer_size = options->buffer_size;
  buffer.bufpos = 0;
  buffer.endpos = 0;
  buffer.empty = 1;

  return buffer;
}

static ch_type *text = NULL;  /* This is only for our atexit()-registered fn */

int
register_free_buffer(void)
{
  void free_buffer(void);
  return atexit(free_buffer);
}

void
free_buffer(void)
{
  free(text);
  text = NULL;
}
