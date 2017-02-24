/* wrap by Richard Cavell v1.0 */
/* January-February 2017 */
/* open_file.c */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "open_file.h"

#include "error.h"
#include "options.h"

static FILE *fp;

static int xclose_file(const char *fn); /* fn can be NULL */
static void wrap_file(FILE *fp, struct buffer_type *buffer,
                      const struct options_type *options);

int
open_file(const char *fn, struct buffer_type *buffer,
          const struct options_type *options)
{
  fp = fn ? fopen(fn, "r") : stdin;

  if (!fp)
  {
    xerror("Error: Couldn't open file %s. Error code: %d\n", fn, errno);

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

static void
wrap_file(FILE *fp, struct buffer_type *buffer,
          const struct options_type *options)
{
  static int done = 0;

  if (!done)
  {
    xprintf("Invocation: %s\n", options->invocation);
    xprintf("File parameters: %s\n",
            options->file_parameters ? "Yes" : "No" );
    xprintf("Buffer text: %p\n", buffer->text);
    xprintf("Buffer size: %lu\n", options->buffer_size);
    xprintf("Line length: %u\n", options->line_length);
    xprintf("Stops: %u\n", options->stops);
    xprintf("Always hyphenate: %s\n",
           options->always_hyphenate ? "Yes" : "No" );
    xprintf("File pointer : %p\n", fp);

    done = 1;
  }
  else
  {
    xprintf("New file pointer : %p\n", fp);
  }
}
