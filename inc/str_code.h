#ifndef str_code_h
#define str_code_h

#include "str.h"

#define STR_CODE_UNKNOWN 0
#define STR_CODE_ID      1
#define STR_CODE_NUM     2
#define STR_CODE_UTF8    3
#define STR_CODE_LINE    4
#define STR_CODE_SIGN    5
#define STR_CODE_SPACE   6
#define STR_CODE_QUOTE   7
#define STR_CODE_BRACKET 8
#define STR_CODE_COMMENT 9

extern uint_t str_code_type(str* s, size_t pos);
extern size_t str_code_next(str* s, size_t pos, uint_t* type);      // return next position of code

#endif /* str_code_h */
