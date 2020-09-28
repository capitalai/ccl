//
//  str.h
//  ccl
//
//  Created by user on 2020/9/16.
//  Copyright © 2020 capita. All rights reserved.
//

#ifndef str_h
#define str_h

#include "bag.h"

// str:      text string
// str_pool: text string pool

typedef struct str      str;
typedef struct str_pool str_pool;

extern size_t str_def_size    (size_t new_size);
extern size_t str_add_def_rate(size_t new_rate);

extern str_pool* str_pool_init(size_t s CAN_ZERO, pobj_t h CAN_NULL);  // s = base buffer size, 0 is default size
extern void      stp_pool_fini(str_pool* t);                           // finish str from pool if no holder

extern str*   str_init(str_pool* t, text_t source CAN_NULL);  // buffer size = max(base buffer size, aligned source length)
extern void   str_fini(str* s);

extern size_t str_size(str* s);
extern char*  str_data(str* s);

extern str*   str_ask(str* s, size_t n);           // ask string buffer size (total size)
extern str*   str_add(str* s, text_t d CAN_NULL);

#endif /* str_h */
