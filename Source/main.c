/* Wrap by Richard Cavell v1.0 */
/* Copyright (c) 2017 Richard John Foster Cavell */
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
    = create_buffer(&options);           /* buffer.h */

        /* buffer.h */            /* open_file.h */
  if (register_free_buffer() || register_close_file())
    /* error.h */ fail_msg("Error: Couldn't register atexit function\n");

  if (options.file_parameters)
  {
    while (*++argv)
      if (is_file(*argv))    /*  options.h  */
      {
                       /* open_file.h */
        int ret_code = open_file(*argv, &buffer, &options);

        if (ret_code != EXIT_SUCCESS)
          exit_code = ret_code;
      }
  }
  else
    exit_code = open_file(NULL, &buffer, &options);    /* open_file.h */

  /* free_buffer() will be called at exit */

  return exit_code;
}
