/* Wrap by Richard Cavell v1.0 */
/* Copyright (c) 2017 Richard John Foster Cavell */
/* open_file.c */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "open_file.h"

#include "buffer.h"
#include "options.h"
#include "output.h"
#include "wrap.h"

static FILE *fp;

static int xclose_file(const char *fn); /* fn can be NULL */

int
open_file(const char *fn, struct buffer_type *buffer,
          const struct options_type *options)
{
  int ret_code = EXIT_SUCCESS;

  fp = fn ? fopen(fn, "r") : stdin;

  if (fp)
  {
    if (wrap_file(fp, buffer, options) /* wrap.h */
      == EXIT_FAILURE)
        ret_code = EXIT_FAILURE;

    if (xclose_file(fn) == EXIT_FAILURE)
        ret_code = EXIT_FAILURE;
  }
  else
  {
    xerror("Error: Couldn't open file %s. Error code: %d\n", fn, errno);

    /* The program continues but will eventually
       exit with EXIT_FAILURE */

    ret_code = EXIT_FAILURE;
  }

  return ret_code;
}

static FILE *fp = NULL;

static int
xclose_file(const char *fn)
{
  if (fp != NULL && fp != stdin)
  {
    int close_code = fclose(fp);

    fp = NULL;

    if (close_code == EOF)
    {
      xerror( "Error: Couldn't close file%s%s. Error code: %d\n",
              fn ? " " : "",
              fn ? fn : "",
              errno);

      /* If being called under normal conditions, the program continues
         but will eventually exit with EXIT_FAILURE.

         If being called by exit(), the program is failing. */

      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}

void
close_file(void)
{
  (void) xclose_file(NULL);
}

int
register_close_file(void)
{
  return atexit(close_file);
}
