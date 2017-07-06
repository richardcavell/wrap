/* Wrap by Richard Cavell v1.0 */
/* Copyright (c) 2017 Richard John Foster Cavell */
/* wrap.c */

#include <stdio.h>
#include <stdlib.h>

#include "wrap.h"

#include "buffer.h"
#include "options.h"

int
wrap_file(FILE *fp, struct buffer_type *buffer,
          const struct options_type *options)
{
  int c;

  (void) fp;
  (void) buffer;
  (void) options;

  while ((c = xgetchar(fp, buffer)) != EOF)
    putchar(c);

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

 return EXIT_SUCCESS;
}
