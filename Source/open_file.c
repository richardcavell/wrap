/* wrap by Richard Cavell v1.0 */
/* January 2017 */
/* open_file.c */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "config.h"
#include "open_file.h"
#include "options.h"

static void wrap_file(FILE *fp, struct options_t options);

void open_file(const char *fn, struct options_t options)
{
  FILE *fp = (fn) ? fopen(fn, "r") : stdin;
  ch_t *buffer = NULL;

  if (fn && !fp)
  {
    fprintf(stderr, "Error: Couldn't open file %s. Error code: %d\n",
            fn, errno);
    exit(EXIT_FAILURE);
  }

  buffer = malloc(options.buffer_size * sizeof(ch_t));

  wrap_file(fp, options);

  if (buffer)
  {
    free(buffer);
    buffer = NULL;
  }

  if (fn && EOF == fclose(fp))
  {
    fprintf(stderr, "Error: Couldn't close file %s. Error code: %d\n",
            fn, errno);
    exit(EXIT_FAILURE);
  }
}

static void
wrap_file(FILE *fp, struct options_t options)
{
  static int done = 0;

  if (!done)
  {
    printf("Invocation: %s\n", options.invocation);
    printf("Files: %s\n", options.files ? "Yes" : "No" );
    printf("Buffer size: %lu\n", options.buffer_size);
    printf("Line length: %u\n", options.line_length);
    printf("Stops: %u\n", options.stops);
    printf("Always hyphenate: %s\n", options.always_hyphenate ? "Yes" : "No" );
    printf("FP : %p\n", fp);
  }

  done = 1;
}
