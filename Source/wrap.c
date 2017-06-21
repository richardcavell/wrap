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
 (void) fp;
 (void) buffer;
 (void) options;

/*  int c;
  static unsigned int col = 0;

  while ((c = xgetchar(fp)) != EOF)
  {
    unsigned int new_col = new_column(col, c);

  }*/

 return EXIT_SUCCESS;
}
