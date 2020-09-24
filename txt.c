//
//  txt.c
//  ccl
//
//  Created by user on 2020/9/16.
//  Copyright © 2020 capita. All rights reserved.
//

#include "lib.h"
#include "txt.h"
#include "sac.h"

#if     SIZE_LEVEL == 1

#define DEF_TXT_SIZE 16

#else
#if     SIZE_LEVEL == 2

#define DEF_TXT_SIZE 32

#else

#define DEF_TXT_SIZE 64

#endif
#endif


struct txt {

    bag*   b;
    size_t s;
    pobj_t h;

};

struct str {

    cap* d;

};

static size_t def_txt_size = DEF_TXT_SIZE;

size_t txt_def_size(size_t new_size) {

    if(new_size) def_txt_size = MAX(new_size, DEF_TXT_SIZE);

    return def_txt_size;

}

txt* txt_init(size_t s, pobj_t h) {

    bag* pick = h ? pobj_pick(h) : sac_init();

    txt* t = cap_data(bag_take(pick, t_size(txt), 0));

    t->b = pick;
    t->s = MAX(s, def_txt_size);
    t->h = h;

    return t;

}

void txt_fini(txt* t) {

    cce(t->h);

    bag_fini(t->b);

}

str* str_init(txt* t, text_t source) {

    str* s = cap_data(bag_take(t->b, t_size(str), 0));

    size_t n = source ? text_size(source, SIZE_MAX) : 0; if(n < t->s) n = t->s;

    s->d = bag_take(t->b, d_size(n), 0);

    cap_set_hold(s->d, t->h);

    return s;

}

void str_fini(str* s) {

    cxe(s->d);
    
    bag_drop(cap_pick(s->d), s->d);

    s->d = NULL;

}

size_t str_size(str* s) { cxz(s->d); return cap_size(s->d); }

char*  str_data(str* s) { cxx(s->d); return cap_data(s->d); }

size_t str_ask_size(str* s, size_t n) {

    size_t m  = str_size(s); ifn(n > m) return m;

    size_t n1 = d_size(n);

    bag*   b  = cap_pick(s->d);

    cap*   t  = bag_take(b, n1, 0);

    text_copy(cap_data(t), cap_data(s->d), n1);

    cap_set_hold(t, cap_hold(s->d));

    bag_drop(b, s->d);

    s->d = t;

    return n1;

}

