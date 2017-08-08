/* Wrap by Richard Cavell (under development) */
/* Copyright (c) 2017 Richard John Foster Cavell */
/* wrap.c */

#include <stdio.h>
#include <stdlib.h>

#include "wrap.h"

#include "buffer.h"
#include "options.h"
#include "output.h"

void
wrap(FILE *fp, struct buffer_type *buffer,
     const struct options_type *options)
{
  const char **fname = NULL;

  xprintf("Filenames :         : (%p)\n", options->filenames);

  if (options->filenames)
    fname = *(options->filenames);

  while(fname && *fname)
    xprintf("  %s\n", fname++[0]);

  xprintf("File pointer is     : ");
           if (fp == stdin) xprintf("stdin\n");
           else             xprintf("%p\n", fp);
  xprintf("Buffer text is      : %10s\n", buffer->text);
  xprintf("Invocation          : %s\n",   options->invocation);
  xprintf("Buffer size         : %lu\n", options->buffer_size);
  xprintf("Line length         : %lu\n", options->line_length);
  xprintf("Stops               : %lu\n", options->stops);
  xprintf("Always hyphenate    : %s\n",  (options->always_hyphenate) ?
                                         "Yes" : "No" );

/*  while ((c = xgetchar(fp, buffer)) != EOF)
    putchar(c); */

/*
  int c;
  int in_word = 0;
  static unsigned int col = 0;  // persists between files

  while ((c = xgetchar(fp)) != EOF)
  {
    unsigned int new_col = new_column(col, c);



    if (new_col <= line_length - 1)
    {
      emit(c);
      col = new_col;
    }
    else if (isspace(c))
    {
      emit(c);
      emit(NEWLINE);
      col = 0;
    }
    else
    {
      assert(isprint(c));

      emit(HYPHEN);
      emit(NEWLINE);
      emit(c);
      col = 0;
    }

    in_word = !isspace(c);
  }
*/
}
