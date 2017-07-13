/* Wrap by Richard Cavell v1.0 */
/* Copyright (c) 2017 Richard John Foster Cavell */
/* main.c */

#include <stdlib.h>

#include "buffer.h"
#include "error.h"
#include "open_file.h"
#include "options.h"

static void
main_wrap(const char *fname, struct buffer_type *buffer,
          const struct options_type *options, int *exit_code);

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

  if (options.file_parameters == 0)
    main_wrap(NULL, &buffer, &options, &exit_code);
  else
    while (*++argv)
      main_wrap(*argv, &buffer, &options, &exit_code);

  /* free_buffer() will be called at exit */

  return exit_code;
}

static void
main_wrap(const char *fname, struct buffer_type *buffer,
          const struct options_type *options, int *exit_code)
{
  if (is_stdin(fname))   /* options.h */
    fname = NULL;

  if (fname == NULL || is_filename(fname))             /* options.h */
  {
    int ret_code = open_file(fname, buffer, options);  /* open_file.h */

    if (ret_code != EXIT_SUCCESS)
      *exit_code = ret_code;
  }
}
