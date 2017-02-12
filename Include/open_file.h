/* wrap by Richard Cavell v1.0 */
/* January-February 2017 */
/* open_file.h */

#ifndef OPEN_FILE_H
#define OPEN_FILE_H

#include "buffer.h"
#include "options.h"

extern int open_file(const char *fn, struct buffer_type *buffer,
                     const struct options_type *options);

extern int register_close_file(void);

#endif
