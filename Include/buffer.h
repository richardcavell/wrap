/* wrap by Richard Cavell v1.0 */
/* January-February 2017 */
/* buffer.h */

#ifndef BUFFER_H
#define BUFFER_H

#include "config.h"
#include "options.h"

struct buffer_type
{
  ch_type *text;
  size_t buffer_size;
  unsigned int bufpos;
};

struct buffer_type get_buffer(const struct options_type *options);
int register_free_buffer(void);

#endif
