/* Wrap by Richard Cavell v1.0 */
/* January-February 2017 */
/* main.c */

#include <stdlib.h>

#include "buffer.h"
#include "error.h"
#include "open_file.h"
#include "options.h"

int
main(int argc, char *argv[])
{
  int exit_code = EXIT_SUCCESS;

  const struct options_type options
    = get_options(argc, argv);           /* options.h */

  struct buffer_type buffer
    = get_buffer(&options);              /* buffer.h */

        /* buffer.h */     /* open_file.h */
  if (reg_free_buffer() || reg_close_file())
    fail_msg("Error: Couldn't register atexit function\n");

  if (options.file_parameters)
  {
    while (*++argv)
      if (is_file(*argv))    /*  options.h  */
      {
            /* open_file.h */
        if (open_file(*argv, &buffer, &options) == EXIT_FAILURE)
          exit_code = EXIT_FAILURE;
      }
  }
  else
    if (open_file(NULL, &buffer, &options)    /* open_file.h */
        == EXIT_FAILURE)
      exit_code = EXIT_FAILURE;

  /* free_buffer() will be called at exit */

  return exit_code;
}
