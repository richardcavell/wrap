/* wrap by Richard Cavell v1.0 */
/* January-February 2017 */
/* error.h */

#ifndef ERROR_H
#define ERROR_H

extern void xprintf(const char *s, ...);
extern void xerror(const char *s, ...);    /* this returns to the caller */
extern void fail_msg(const char *s, ...);  /* this exits with EXIT_FAILURE */

#endif
