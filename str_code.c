
#include "lib.h"
#include "str_code.h"

// ID      = 1
// NUM     = 2
// UTF8    = 3
// LINE    = 4
// SIGN    = 5
// SPACE   = 6
// QUOTE   = 7
// BRACKET = 8
// COMMENT = 9

static uint_t char_type[] = {

  0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 4, 0, 0, 4, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  6, 5, 7, 5, 5, 5, 5, 7, 8, 0, 5, 5, 5, 5, 5, 9,  // 20:  !"#$%&'()*+,-./
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 5, 5, 5, 5, 5, 5,  // 30: 0123456789:;<=>?
  5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 40: @ABCDEFGHIJKLMNO
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8, 5, 0, 5, 1,  // 50: PQRSTUVWXYZ[\]^_
  5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  // 60: `abcdefghijklmno
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8, 5, 0, 5, 0,  // 70: pqrstuvwxyz{|}~
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 80
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 90
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // A0
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // B0
  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,  // C0
  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,  // D0
  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,  // E0
  3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0   // F0

};

uint_t str_code_type(str* s, size_t pos) {

  size_t l = str_length(s);

  if(l < pos) return STR_CODE_UNKNOWN;

  text_t c = str_data(s) + pos;
  uint_t t = char_type[c[0]];

  if(t == STR_CODE_UTF8) {

    if(c[0] < 0xE0) return pos + 1 < l && c[1] > 0x7F && c[1] < 0xC0 ? STR_CODE_UTF8 : STR_CODE_UNKNOWN;
    if(c[0] < 0xF0) return pos + 2 < l && c[1] > 0x7F && c[1] < 0xC0 && 
                                          c[2] > 0x7F && c[2] < 0xC0 ? STR_CODE_UTF8 : STR_CODE_UNKNOWN;
    
    return pos + 3 < l && c[1] > 0x7F && c[1] < 0xC0 && 
                          c[2] > 0x7F && c[2] < 0xC0 && 
                          c[3] > 0x7F && c[3] < 0xC0 ? STR_CODE_UTF8 : STR_CODE_UNKNOWN;

  }

  if(t == STR_CODE_COMMENT) return pos + 1 < l && (c[1] == '*' || c[1] == '/') ? STR_CODE_COMMENT : STR_CODE_SIGN;

  return t;

}

size_t str_code_next(str* s, size_t pos, uint_t* type) {

  size_t l = str_length(s);

  if(l < pos) return pos;

  l -= pos;

  uint_t t = str_code_type(s, pos);
  text_t c = str_data(s) + pos;
  size_t n = 1;
  size_t p;
  bool   num_hex   = false;
  bool   num_point = false;
  bool   num_exp   = false;
  char   end_bracket;
  char   quote;

  if(type) *type = t;

  switch(t) {
    
    case STR_CODE_ID:  // [_A-Za-z][_A-Za-z0-9]*
    
      while(n < l) {

        uint t1 = char_type[c[n]];
        
        if(t1 == STR_CODE_ID || t1 == STR_CODE_NUM) n++; else break;

      }
      
    break;

    case STR_CODE_NUM:  // support hex format like 0x1f and float format like 1.2e-3

      if(c[0] == '0') {

        if(n < l && (c[1] == 'x' || c[1] == 'X')) {

          p = n;

          n++;

          while(n < l && (c[n] >= '0' && c[n] <= '9' || 
                          c[n] >= 'a' && c[n] <= 'f' || 
                          c[n] >= 'A' && c[n] <= 'F')) { num_hex = true; n++; }

          if(num_hex == false) n = p;

          break;

        } 

      }
      else {
        
        while(n < l && char_type[c[n]] == STR_CODE_NUM) n++; 

      }

      if(n < l) { 
        
        if(c[n] == '.') {

          p = n;

          n++;

          while(n < l && char_type[c[n]] == STR_CODE_NUM) { num_point = true; n++; }

          if(num_point == false) { n = p; break; }

        }

        if(c[n] == 'e' || c[n] == 'E') {

          p = n;

          n++;

          if(c[n] == '-') n++;

          while(n < l && char_type[c[n]] == STR_CODE_NUM) { num_exp = true; n++; }

          if(num_point == false) { n = p; break; }

        }

      }

    break;

    case STR_CODE_UTF8:

      if(c[0] < 0xE0) { n = 2; break; }
      if(c[0] < 0xF0) { n = 3; break; }

      n = 4;

    break;

    case STR_CODE_LINE:  // support CR, LF, CRLF, LFCR

      if(char_type[c[1]] == STR_CODE_LINE && c[0] != c[1]) n++;

    break;

    case STR_CODE_SIGN:  // support multiple sign characters

      while(n < l && char_type[c[n]] == STR_CODE_SIGN) n++; 

    break;

    case STR_CODE_SPACE:  // support multiple spaces

      while(n < l && char_type[c[n]] == STR_CODE_SPACE) n++; 

    break;

    case STR_CODE_QUOTE:  // '..' or "..."

      while(n < l && c[n] != c[0]) { if(c[n] == '\\' && n + 1 < l) n++; n++; }

      if(n < l) n++;

    break;

    case STR_CODE_BRACKET:  // support {...} [...] (...)

      switch(c[0]) {

        case '(': end_bracket = ')'; break;
        case '[': end_bracket = ']'; break;
        case '{': end_bracket = '}'; break;

      }

      while(n < l) {

        if(c[n] == end_bracket) { n++; break; }

        if(char_type[c[n]] == STR_CODE_QUOTE) {

          quote = c[n];

          n++;

          while(n < l && c[n] != quote) { if(c[n] == '\\' && n + 1 < l) n++; n++; }

          if(n < l) n++;

        }

      }

    break;

    case STR_CODE_COMMENT:

      n = 2;

      if(c[1] == '*') {

        do {

          while(n < l && c[n] != '*') n++; 

          if(n < l) n++;

        } while(n < l && c[n] != '/');

        if(n < l) n++;

      }
      else {

        while(n < l && char_type[c[n]] != STR_CODE_LINE) n++; 

        if(n < l) {

          if(n + 1 < l && char_type[c[n + 1]] == STR_CODE_LINE && c[n] != c[n + 1]) n++;

          n++;

        }

      }

    break;

  }

  return n + pos;

}

size_t str_code_line(str* s, size_t pos, text_t end_chars) {

  size_t l = str_length(s);
  text_t c = str_data(s) + pos;
  size_t n = pos;

  if(end_chars && *end_chars) {

    while(n < l && char_type[*c] != STR_CODE_LINE) {

      text_t e = end_chars; while(*e && *c != *e) e++; if(*e) break;

      c++;
      n++;

    }

  }
  else {

    while(n < l && char_type[*c] != STR_CODE_LINE) {

      c++;
      n++;

    }

  }

  return n;

}
