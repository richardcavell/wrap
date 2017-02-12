/* wrap by Richard Cavell v1.0 */
/* January-February 2017 */
/* main.c */

#include <stdio.h>
#include <stdlib.h>

#include "buffer.h"
#include "config.h"
#include "error.h"
#include "open_file.h"
#include "options.h"

int
main(int argc, char *argv[])
{
  int exit_failure = 0;

  const struct options_type options
    = get_options(argc, argv);           /*  options.h  */

  struct buffer_type buffer
    = get_buffer(&options);              /* buffer.h */

      /* buffer.h */              /* openfile.h */
  if (register_free_buffer() || register_close_file())
    fail_msg("Error: Couldn't register atexit function\n");

  if (options.file_parameters)
  {
    while (*++argv)    /*  options.h  */
      if (is_file(*argv) && open_file(*argv, &buffer, &options)
                            == EXIT_FAILURE)
        exit_failure = 1;
  }
  else
    if (open_file(NULL, &buffer, &options)  /* open_file.h */
        == EXIT_FAILURE)
      exit_failure = 1;

  /* free_buffer() and close_file() will be called at exit */

  return exit_failure ? EXIT_FAILURE : EXIT_SUCCESS;
}
