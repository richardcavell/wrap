/* Wrap by Richard Cavell (under development) */
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

static void close_file(void);
static void _close_file(const char *fn,       /* fn can be NULL */
                        int *exit_code);      /* exit_code can be NULL */

void
process_file(const char *fn, struct buffer_type *buffer,
          const struct options_type *options, int *exit_code)
{
  static int cf_has_been_registered = 0;

  fp = (fn ? fopen(fn, "r") : stdin);

  if (fp)
  {
    if (!cf_has_been_registered)
    {
      if (atexit(close_file))
        fail_msg("Error: Couldn't register process_file callback\n");

      cf_has_been_registered = 1;
    }

    wrap(fp, buffer, options);  /* wrap.h */
    _close_file(fn, exit_code);
  }
  else
  {
    xerror("Error: Couldn't open file %s. Error code: %d\n", fn, errno);
    *exit_code = EXIT_FAILURE;

    /* The program continues,
       but will eventually exit with EXIT_FAILURE */

  }
}

static void
close_file(void)
{
  _close_file(NULL, NULL);
}

static FILE *fp = NULL;

/* This can be called either by process_file() (under normal conditions),
   or by exit() (meaning the program is failing) */

static void
_close_file(const char *fn, int *exit_code)
{
  if (fp != NULL && fp != stdin)
  {
    int close_code = fclose(fp);

    fp = NULL;

    if (close_code == EOF)
    {
      if (fn)
        xerror( "Error: Couldn't close file %s. Error code: %d\n",
                fn, errno);
      else
        xerror( "Error: Couldn't close file. Error code: %d\n",
                errno);

      if (exit_code)
        *exit_code = EXIT_FAILURE;
    }
  }
}
