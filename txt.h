//
//  txt.h
//  ccl
//
//  Created by user on 2020/9/16.
//  Copyright © 2020 capita. All rights reserved.
//

#ifndef txt_h
#define txt_h

#include "bag.h"

// txt: text string allocator

typedef struct txt   txt;
typedef struct str   str;

extern size_t txt_def_size    (size_t new_size);  
extern size_t str_add_def_rate(size_t new_rate);

extern txt*   txt_init(size_t s CAN_ZERO, pobj_t h CAN_NULL);  // s = base buffer size, 0 is default size
extern void   txt_fini(txt* t);                                // also finish all str from this txt if no holder

extern str*   str_init(txt* t, text_t source CAN_NULL);  // buffer size = max(base buffer size, aligned source length)
extern void   str_fini(str* s);

extern size_t str_size(str* s);
extern char*  str_data(str* s);

extern str* str_ask(str* s, size_t n);           // ask string buffer size (total size)
extern str* str_add(str* s, text_t d CAN_NULL);

#endif /* str_h */
