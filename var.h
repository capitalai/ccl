#ifndef var_h
#define var_h

#include "obj.h"
#include "str.h"

// var: named data collection of variant types

// set object to var will transfer ownership, then set any type of data to var will finish the object

typedef struct var var;
typedef struct val val;

struct val {

    uint_t v_type;  // type of pointer
    pbuf_t v_data;  // data of pointer
    utf8_t v_utf8;
    real_t v_real;
    ulli_t v_ulli;
    bool   v_bool;

};

enum {
  
  var_type_none,
  var_type_bool,
  var_type_int,
  var_type_num,
  var_type_char,
  var_type_ptr,
  var_type_str,
  var_type_bin,
  var_type_obj

};

extern size_t var_size(void);
extern size_t var_type(void);
extern size_t var_pass(parg_t a);  // check arguments and return required object data size

extern pobj_t var_init(pbuf_t b CAN_NULL, parg_t a CAN_NULL, pobj_t h CAN_NULL);
extern void   var_fini(pobj_t x);

inline var*   new_var  (void);
inline var*   new_var_c(pobj_t p_hold CAN_NULL);
inline void   del_var  (var*   o, pobj_t caller CAN_NULL);

// priority of types: obj, bin, str, ptr, char, num, int, bool

extern bool   var_set_obj (var* o, text_t k, pobj_t a);
extern bool   var_set_bin (var* o, text_t k, size_t s, data_t a CAN_NULL);
extern bool   var_set_str (var* o, text_t k, text_t a CAN_NULL);
extern bool   var_set_ptr (var* o, text_t k, uint_t t, pbuf_t p);
extern bool   var_set_char(var* o, text_t k, utf8_t v);
extern bool   var_set_num (var* o, text_t k, real_t v);
extern bool   var_set_int (var* o, text_t k, ulli_t v);
extern bool   var_set_bool(var* o, text_t k, bool   v);

extern pobj_t var_get_obj (var* o, text_t k);
extern cap*   var_get_bin (var* o, text_t k);
extern str*   var_get_str (var* o, text_t k);
extern val*   var_get_ptr (var* o, text_t k);
extern val*   var_get_char(var* o, text_t k);
extern val*   var_get_num (var* o, text_t k);
extern val*   var_get_int (var* o, text_t k);
extern val*   var_get_bool(var* o, text_t k);

extern uint_t var_get_type(var* o, text_t k);

inline var* new_var  (void)          { return (var*)var_init(NULL, NULL, NULL); }
inline var* new_var_c(pobj_t p_hold) { return (var*)var_init(NULL, NULL, p_hold); }

inline void del_var(var* o, pobj_t caller) { del_obj(var); }

#endif /* var_h */
