/* wrap by Richard Cavell v1.0 */
/* January-February 2017 */
/* buffer.c */

#include <stdio.h>
#include <stdlib.h>

#include "buffer.h"
#include "options.h"

static ch_type *text;

struct buffer_type
get_buffer(const struct options_type *options)
{
  struct buffer_type buffer;

  buffer.text = malloc(options->buffer_size);
  text = buffer.text;

  if (buffer.text == NULL)
  {
    fprintf(stderr, "Error: Couldn't acquire %lu bytes for the buffer\n",
            (unsigned long int) options->buffer_size);
    exit(EXIT_FAILURE);
  }

  buffer.buffer_size = options->buffer_size;
  buffer.bufpos = 0;

  return buffer;
}

static ch_type *text = NULL;

void free_buffer(void);

int
register_free_buffer(void)
{
  return atexit(free_buffer);
}

void
free_buffer(void)
{
  free(text);
  text = NULL;
}