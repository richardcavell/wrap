/* Wrap by Richard Cavell v1.0 */
/* Copyright (c) 2017 Richard John Foster Cavell */
/* buffer.c */

#include <stdlib.h>

#include "buffer.h"

#include "options.h"
#include "output.h"

static ch_type *text;

struct buffer_type
create_buffer(const struct options_type *options)
{
  struct buffer_type buffer;

  text = buffer.text = malloc(options->buffer_size);

  if (text == NULL)
    fail_msg("Error: Couldn't acquire memory for the buffer\n");

  buffer.buffer_size = options->buffer_size;
  buffer.startpos = 0;
  buffer.endpos = 0;
  buffer.empty = 1;

  return buffer;
}

  /* This is only for our atexit()-registered fn */
static ch_type *text = NULL;

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

static void
  fill(FILE *fp, struct buffer_type *buffer);

static void
  detect_empty(struct buffer_type *buffer);

int
xgetchar(FILE *fp, struct buffer_type *buffer)
{
  int c;

  fill(fp, buffer);

  if (buffer->empty)
    return EOF;           /* Buffer and file stream are exhausted */

  c = buffer->text[buffer->startpos];

  ++buffer->startpos;

  if (buffer->startpos == buffer->buffer_size)
    buffer->startpos = 0;

  detect_empty(buffer);

  fill(fp, buffer);

  return c;
}

static void
  addchar(FILE *fp, struct buffer_type *buffer);

static int
  isfull(struct buffer_type *buffer);

static void
fill(FILE *fp, struct buffer_type *buffer)
{
  while (!isfull(buffer) && !feof(fp))
    addchar(fp, buffer);
}

static int
isfull(struct buffer_type *buffer)
{
  return ((buffer->empty == 0) &&
          (buffer->startpos == buffer->endpos));
}

  /* Can only be used after a char has been removed */
static void
detect_empty(struct buffer_type *buffer)
{
  if (buffer->startpos == buffer->endpos)
    buffer->empty = 1;
}

  /* assumes that buffer is not full */
static void
addchar(FILE *fp, struct buffer_type *buffer)
{
  const int BACKSPACE = '\b';
  int c = fgetc(fp);

  if (c == EOF)
  {
    /* do nothing */
  }
  else if (c != BACKSPACE)
  {
    buffer->text[buffer->endpos] = (ch_type) c;

    buffer->empty = 0;

    ++buffer->endpos;
    if (buffer->endpos == buffer->buffer_size)
      buffer->endpos = 0;
  }
  else /* c == BACKSPACE */
  {
    if (buffer->empty)
    {
      xerror("Error: Buffer under-run\n");
      /* The program continues */
    }

    if (buffer->endpos == 0)
      buffer->endpos = buffer->buffer_size;

    --buffer->endpos;

    detect_empty(buffer);

      /* Discard the character that has been backspaced over */
  }
}
