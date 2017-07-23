/* Wrap by Richard Cavell v1.0 */
/* Copyright (c) 2017 Richard John Foster Cavell */
/* file.c */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "file.h"

#include "buffer.h"
#include "options.h"
#include "output.h"
#include "wrap.h"

static FILE *fp;   /* declaration only - defined below */

static int _close_file(const char *fn); /* fn can be NULL */

void
process_file(const char *fn, struct buffer_type *buffer,
          const struct options_type *options, int *exit_code)
{
  fp = (fn ? fopen(fn, "r") : stdin);

  if (fp)
  {
    /* If *exit_code is set to EXIT_FAILURE, the program continues
       but will eventually exit with EXIT_FAILURE */

    if (wrap(fp, buffer, options) /* wrap.h */
      == EXIT_FAILURE)
        *exit_code = EXIT_FAILURE;

    if (_close_file(fn) == EXIT_FAILURE)
        *exit_code = EXIT_FAILURE;
  }
  else
  {
    xerror("Error: Couldn't open file %s. Error code: %d\n", fn, errno);
    *exit_code = EXIT_FAILURE;
  }
}

static FILE *fp = NULL;

/* This can be called either by process_file() (under normal conditions),
   or by exit() (meaning the program is failing) */

static int
_close_file(const char *fn)
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

      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}

void
close_file(void)
{
  (void) _close_file(NULL);
}

int
register_close_file(void)
{
  return atexit(close_file);
}
