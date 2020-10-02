//
//  inc.h
//  lib_ccl
//
//  Created by user on 2018/6/21.
//  Copyright © 2018年 Infinita. All rights reserved.
//

#ifndef inc_h
#define inc_h

// inc: base defines and macros. include this header file if no other includes.

#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>
#include <limits.h>

// function usage directive

#define CAN_NULL
#define CAN_ZERO
#define FOR_PASS

// type declaration

typedef unsigned char      byte_t;
typedef unsigned int       uint_t;
typedef unsigned long long ulli_t;  // unsigned long long integer
typedef long double        real_t;
typedef const void*        data_t;
typedef const char*        text_t;

// generic type using void*

typedef void* parg_t;    // any argument pointer for creating object
typedef void* pobj_t;    // any object   pointer
typedef void* pbuf_t;    // any buffer   pointer

typedef union ptr ptr;

union ptr {

    void*   p;
    byte_t* b;
    char*   c;

};

typedef struct utf8_t utf8_t;

struct utf8_t {

    byte_t c[4];

};

// useful macros

#define STR(s)    #s
#define PAD(a, b) a ## b

#endif /* inc_h */
