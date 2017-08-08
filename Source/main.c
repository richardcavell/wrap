/* Wrap by Richard Cavell (under development) */
/* Copyright (c) 2017 Richard John Foster Cavell */
/* main.c */

#include <stdlib.h>

#include "buffer.h"
#include "file.h"
#include "options.h"
#include "output.h"

static void
main_process_fname(const char *fname, struct buffer_type *buffer,
          const struct options_type *options, int *exit_code);

int
main(int argc, char *argv[])
{
  int exit_code = EXIT_SUCCESS;

  const struct options_type options
    = get_options(argc, (const char **) argv);           /* options.h */

  struct buffer_type buffer
    = create_buffer(&options);           /* buffer.h */

  if (options.filenames == NULL)
    main_process_fname(NULL, &buffer, &options, &exit_code);
  else
  {
    const char **fname = *(options.filenames);

    while (*fname)
      main_process_fname(*fname++, &buffer, &options, &exit_code);
  }

  return exit_code;
}

static void
main_process_fname(const char *fname, struct buffer_type *buffer,
          const struct options_type *options, int *exit_code)
{
  if (is_stdin(fname))   /* options.h */
    fname = NULL;

  process_file(fname, buffer, options, exit_code);   /* file.h */
}
