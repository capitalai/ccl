#ifndef str_file_h
#define str_file_h

#include "str.h"

// load from file and save to file

// source is null will input from standard input  device (console)
// target is null will output to  standard output device (console)

extern str*   str_load(str_pool* p, text_t source CAN_NULL);  // load content from file into new str, return null if failed
extern bool   str_save(str* s,      text_t target CAN_NULL);  // save data to file and return true if succeeded

#endif /* str_file_h */
