#pragma once

#define IN      /* input value */
#define OPT_IN  /* optional input value (can be NULL) */
#define OUT     /* output value */
#define OPT_OUT /* optional output value (can be NULL) */

#define NEW /* return pointer, which is allocated memory */

/*
If the OPT_OUT data pointer is NULL,
then the pointer is re-pointed to the reserved value,
which avoids accessing the NULL address.*/
#define MAKE_OPTIONAL(type, name) \
type _opt_reserved_##name = 0; if(name == 0) {name = &_opt_reserved_##name;};