/* wrap by Richard Cavell v1.0 */
/* January 2017 */
/* main.c */

#include <stdlib.h>

#include "open_file.h"
#include "options.h"

int
main(int argc, char *argv[])
{
  const struct options_t options 
                         = get_options(argc, argv);  /* options.h */

  if (options.files)
  {
    while (*++argv)
      if (**argv != hyphen)             /*  options.h  */
        open_file(*argv, options);      /* open_file.h */
  }
  else
    open_file(NULL, options);           /* open_file.h */

  return EXIT_SUCCESS;
}
