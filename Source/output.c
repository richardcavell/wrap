/* Wrap by Richard Cavell (under development) */
/* Copyright (c) 2017 Richard John Foster Cavell */
/* output.c */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "output.h"

static void xfprintf(FILE *stream, const char *s,
                     va_list ap, const char *stream_name);

void
xprintf(const char *s, ...)
{
  va_list ap;

  va_start(ap, s);
  xfprintf(stdout, s, ap, "standard output");
  va_end(ap);
}

void
xerror(const char *s, ...)
{
  va_list ap;

  va_start(ap, s);
  xfprintf(stderr, s, ap, "standard error");
  va_end(ap);
}

void
fail_msg(const char *s, ...)
{
  va_list ap;

  va_start(ap, s);
  (void) vfprintf(stderr, s, ap);
  va_end(ap);

  exit(EXIT_FAILURE);
}

static void
xfprintf(FILE *stream, const char *s, va_list ap, const char *stream_name)
{
  if (vfprintf(stream, s, ap) < 0)
  {
    (void) fprintf(stderr, "Error: Unable to print to %s\n", stream_name);
    exit(EXIT_FAILURE);
  }
}
