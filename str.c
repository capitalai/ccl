#include "lib.h"
#include "str.h"
#include "sac.h"
#include "obj.h"

#if     SIZE_LEVEL == 1

#define DEF_STR_SIZE 16

#else
#if     SIZE_LEVEL == 2

#define DEF_STR_SIZE 32

#else

#define DEF_STR_SIZE 64

#endif
#endif

#define DEF_STR_ADD_RATE 4

struct str_pool {

    bag*   b;
    size_t s;
    pobj_t h;

};

struct str {

    cap* data;
    cap* self;

};

static size_t def_str_size     = DEF_STR_SIZE;
static size_t def_str_add_rate = DEF_STR_ADD_RATE;

size_t str_def_size(size_t new_size) {

    if(new_size) def_str_size = MAX(new_size, DEF_STR_SIZE);

    return def_str_size;

}

size_t str_add_def_rate(size_t new_rate) {

  if(new_rate) def_str_add_rate = new_rate;

  return def_str_add_rate;

}

str_pool* str_pool_init(size_t s, pobj_t h) {

    bag* pick = h ? pobj_pick(h) : sac_init();

    str_pool* t = cap_data(bag_take(pick, t_size(str_pool), 0));

    t->b = pick;
    t->s = MAX(s, def_str_size);
    t->h = h;

    return t;

}

void str_pool_fini(str_pool* t) {

    cce(t->h);

    bag_fini(t->b);

}

str* str_init(str_pool* t, text_t source) {

    cap* x = bag_take(t->b, t_size(str), 0);
    str* s = cap_data(x);

    size_t n = source ? text_size(source, SIZE_MAX) : 0; if(n < t->s) n = t->s;

    s->data = bag_take(t->b, d_size(n), 0);
    s->self = x;

    cap_set_hold(s->data, t->h);

    if(source) text_copy(cap_data(s->data), source, n);

    return s;

}

void str_fini(str* s) {

    bag* p = cap_pick(s->data);

    bag_drop(p, s->data);
    bag_drop(p, s->self);

}

size_t str_size(str* s) { return cap_size(s->data); }

char*  str_data(str* s) { return cap_data(s->data); }

str* str_ask(str* s, size_t n) {

    size_t m  = str_size(s); ifn(n > m) return s;

    size_t n1 = d_size(n);

    bag*   b  = cap_pick(s->data);

    cap*   t  = bag_take(b, n1, 0);

    text_copy(cap_data(t), cap_data(s->data), n1);

    cap_set_hold(t, cap_hold(s->data));

    bag_drop(b, s->data);

    s->data = t;

    return s;

}

str* str_add(str* s, text_t d) {

    ifx(d) return s;

    size_t bs = cap_size(s->data);
    size_t s1 = text_size(cap_data(s->data), bs);
    size_t s2 = text_size(d, SIZE_MAX);
    size_t ss = s1 + s2 - 1;

    if(ss > bs) {

        size_t n = CEIL(ss, def_str_size); n += n / def_str_add_rate;

        str_ask(s, n * def_str_size);

        bs = cap_size(s->data);

    }

    char*  b = cap_data(s->data);

    text_copy(b + s1 - 1, d, s2);

    return s;

}

