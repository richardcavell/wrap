/* wrap by Richard Cavell v1.0 */
/* January-February 2017 */
/* error.c */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "error.h"

void
xprintf(const char *s, ...)
{
  int return_code;
  va_list ap;

  va_start(ap, s);
  return_code = vprintf(s, ap);
  va_end(ap);

  if (return_code < 0)
  {
    fprintf(stderr, "Unable to print to standard output\n");
    exit(EXIT_FAILURE);
  }
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
