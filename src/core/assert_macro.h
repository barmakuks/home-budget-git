#ifndef ASSERT_MACRO_H
#define ASSERT_MACRO_H

#include <assert.h>

#define ASSERT_RETURN(condition, result) if (!condition) {assert(false); return result;}

#define ASSERT_RETURN_VOID(condition) if (!condition) {assert(false); return;}

#endif // ASSERT_MACRO_H
