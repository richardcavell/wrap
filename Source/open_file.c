/* wrap by Richard Cavell v1.0 */
/* January-February 2017 */
/* open_file.c */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "config.h"
#include "open_file.h"
#include "options.h"

static FILE *fp;

static int xclose_file(const char *fn);
static void wrap_file(FILE *fp, struct buffer_type *buffer,
                      const struct options_type *options);

int
open_file(const char *fn, struct buffer_type *buffer,
          const struct options_type *options)
{
  fp = fn ? fopen(fn, "r") : stdin;

  if (!fp)
  {
    fprintf(stderr, "Error: Couldn't open file %s. Error code: %d\n",
            fn, errno);

    /* The program continues but will eventually
       exit with EXIT_FAILURE */

    return EXIT_FAILURE;
  }

  wrap_file(fp, buffer, options);

  return xclose_file(fn);
}

static FILE *fp = NULL;

static int
xclose_file(const char *fn)
{
  if (fp && fp != stdin)
  {
    int close_code = fclose(fp);

    fp = NULL;

    if (close_code == EOF)
    {
      fprintf(stderr, "Error: Couldn't close file%s%s. Error code: %d\n",
              fn ? " " : "",
              fn ? fn : "",
              errno);

      /* The program continues but will eventually
         exit with EXIT_FAILURE */

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

static void
wrap_file(FILE *fp, struct buffer_type *buffer,
          const struct options_type *options)
{
  static int done = 0;

  if (!done)
  {
    printf("Invocation: %s\n", options->invocation);
    printf("File parameters: %s\n",
            options->file_parameters ? "Yes" : "No" );
    printf("Buffer : %p\n", buffer);
    printf("Buffer size: %lu\n", options->buffer_size);
    printf("Line length: %u\n", options->line_length);
    printf("Stops: %u\n", options->stops);
    printf("Always hyphenate: %s\n",
           options->always_hyphenate ? "Yes" : "No" );
    printf("FP : %p\n", fp);
  }

  done = 1;
}
