//
//  bag.h
//  lib_ccl
//
//  Created by Capita Lai on 2020/6/10.
//  Copyright © 2020 Capita Lai. All rights reserved.
//

#ifndef bag_h
#define bag_h

#include "inc.h"
#include "com.h"

// cap: capsule structure contains allocated memory buffer

typedef struct cap cap;
typedef struct bag bag;

// for function parameters

typedef bag* bag_init_f(void);

typedef cap* bag_take_f(bag* b, size_t s, size_t h);  // bag, size, open head size
typedef bool bag_drop_f(bag* b, cap*   m);
typedef void bag_fini_f(bag* b);

typedef void* pbag_t;  // any bag pointer

struct cap {

    size_t s_head;  // head size (include cap size)
    size_t s_data;  // data size
    pbag_t owner;   // it's bag pointer for checking
    pobj_t refer;   // reference object

};

struct bag {

    bag_take_f* take;    // memory allocator of this bagect
    bag_drop_f* drop;    // memory deallocator of this bagect
    bag_fini_f* fini;    // this bagect destructor
    size_t      type;    // type of bag
    size_t      s_base;  // base head size

};

extern void   set_bag_init(bag_init_f f);

extern bag*   bag_init(void);

inline cap*   bag_take(bag* b, size_t s CAN_ZERO, size_t h CAN_ZERO) { return b->take(b, s, h); }
inline bool   bag_drop(bag* b, cap*   p)                             { return b->drop(b, p); }
inline void   bag_fini(bag* b)                                       { b->fini(b); }
inline size_t bag_type(bag* b)                                       { return b->type; }

// fancy functions without checking

inline pbuf_t cap_data(cap* p) { return (pbuf_t)((char*)p + p->s_head); }
inline size_t cap_size(cap* p) { return p->s_data; }

inline void   cap_set_refer(cap* p, pobj_t o) { p->refer = o; }
inline pobj_t cap_refer(cap* p)               { return p->refer; }

inline void*  bag_add_bin(bag* b, size_t s,          data_t d) { return data_copy(cap_data(bag_take(b, s, 0)), d, s); }
extern void*  bag_add_str(bag* b, size_t s CAN_ZERO, text_t t);

inline size_t bag_head_size(bag* b, cap* p) { return p->s_head - b->s_base; }
inline pbuf_t bag_head_data(bag* b, cap* p) { return (char*)p + b->s_base; }

// macros

#define BAG_TAKE(o, t, h) bag_take(o, sizeof(t), h)

#endif /* bag_h */
