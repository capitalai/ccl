#ifndef txt_file_h
#define txt_file_h

#include "txt.h"

typedef size_t txt_loader(txt* t, str* s, size_t p);

extern size_t txt_load_line(txt* t, str* s, size_t p);        // normal line (default)
extern size_t txt_load_trim_line(txt* t, str* s, size_t p);   // skip empty line
extern size_t txt_load_trim_space(txt* t, str* s, size_t p);  // trim space and skip empty line
extern size_t txt_load_code(txt* t, str* s, size_t p);        // load code terms

extern bool txt_load(txt* t, text_t source_file CAN_NULL, txt_loader* f CAN_NULL);
extern bool txt_save(txt* t, text_t target_file CAN_NULL);

#endif /* txt_file_h */
