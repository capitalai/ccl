
#include "ccl/str_code.h"
#include "ccl/lib.h"

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

    0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 4, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 6, 5, 7, 5, 5, 5, 5, 7, 8, 0, 5, 5, 5, 5, 5, 9,  // 20:  !"#$%&'()*+,-./
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 5, 5, 5, 5, 5, 5,                          // 30: 0123456789:;<=>?
    5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,                          // 40: @ABCDEFGHIJKLMNO
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8, 5, 0, 5, 1,                          // 50: PQRSTUVWXYZ[\]^_
    5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,                          // 60: `abcdefghijklmno
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8, 5, 0, 5, 0,                          // 70: pqrstuvwxyz{|}~
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                          // 80
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                          // 90
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                          // A0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                          // B0
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,                          // C0
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,                          // D0
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,                          // E0
    3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0                           // F0

};

uint_t str_code_type(str* s, size_t pos) {
    size_t l = str_length(s);

    if(l < pos) return STR_CODE_UNKNOWN;

    byte_t* c = (byte_t*)str_data(s) + pos;
    uint_t  t = char_type[c[0]];

    if(t == STR_CODE_UTF8) {
        if(c[0] < 0xE0) return pos + 1 < l && c[1] > 0x7F && c[1] < 0xC0 ? STR_CODE_UTF8 : STR_CODE_UNKNOWN;
        if(c[0] < 0xF0)
            return pos + 2 < l && c[1] > 0x7F && c[1] < 0xC0 && c[2] > 0x7F && c[2] < 0xC0 ? STR_CODE_UTF8 : STR_CODE_UNKNOWN;

        return pos + 3 < l && c[1] > 0x7F && c[1] < 0xC0 && c[2] > 0x7F && c[2] < 0xC0 && c[3] > 0x7F && c[3] < 0xC0
               ? STR_CODE_UTF8
               : STR_CODE_UNKNOWN;
    }

    if(t == STR_CODE_COMMENT) return pos + 1 < l && (c[1] == '*' || c[1] == '/') ? STR_CODE_COMMENT : STR_CODE_SIGN;

    return t;
}

struct str_code {
    const byte_t* const c;
    const size_t        l;
    size_t              n;
};

typedef struct str_code str_code;

typedef void str_code_fun(str_code* d);

static void str_code_unknown(str_code* d);
static void str_code_id(str_code* d);
static void str_code_num(str_code* d);
static void str_code_utf8(str_code* d);
static void str_code_line(str_code* d);
static void str_code_sign(str_code* d);
static void str_code_space(str_code* d);
static void str_code_quote(str_code* d);
static void str_code_bracket(str_code* d);
static void str_code_comment(str_code* d);

static str_code_fun* str_codes[] = {

    str_code_unknown, str_code_id,    str_code_num,   str_code_utf8,    str_code_line,
    str_code_sign,    str_code_space, str_code_quote, str_code_bracket, str_code_comment

};

static void str_code_unknown(str_code* d) { ++d->n; }

static void str_code_id(str_code* d) {
    ++d->n;

    while(d->n < d->l) {
        uint t1 = char_type[d->c[d->n]];

        if(t1 == STR_CODE_ID || t1 == STR_CODE_NUM) ++d->n;
        else break;
    }
}

static void str_code_num(str_code* d) {
    bool num_hex   = false;
    bool num_point = false;
    bool num_exp   = false;

    size_t p;

    if(d->c[d->n] == '0') {
        ++d->n;

        if(d->n < d->l && (d->c[d->n] == 'x' || d->c[d->n] == 'X')) {
            p = d->n;

            ++d->n;

            while(d->n < d->l
                  && ((d->c[d->n] >= '0' && d->c[d->n] <= '9') || (d->c[d->n] >= 'a' && d->c[d->n] <= 'f')
                      || (d->c[d->n] >= 'A' && d->c[d->n] <= 'F'))) {
                num_hex = true;
                ++d->n;
            }

            if(num_hex == false) d->n = p;
        }

        // support octal or binary number ?

        return;

    } else {
        ++d->n;

        while(d->n < d->l && char_type[d->c[d->n]] == STR_CODE_NUM) ++d->n;
    }

    if(d->n < d->l) {
        if(d->c[d->n] == '.') {
            p = d->n;

            ++d->n;

            while(d->n < d->l && char_type[d->c[d->n]] == STR_CODE_NUM) {
                num_point = true;
                ++d->n;
            }

            if(num_point == false) {
                d->n = p;
                return;
            }
        }

        if(d->c[d->n] == 'e' || d->c[d->n] == 'E') {
            p = d->n;

            ++d->n;

            if(d->c[d->n] == '-') ++d->n;

            while(d->n < d->l && char_type[d->c[d->n]] == STR_CODE_NUM) {
                num_exp = true;
                ++d->n;
            }

            if(num_point == false) {
                d->n = p;
                return;
            }
        }
    }
}

static void str_code_utf8(str_code* d) {
    if(d->c[d->n] < 0xE0) {
        d->n += 2;
        return;
    }
    if(d->c[d->n] < 0xF0) {
        d->n += 3;
        return;
    }

    d->n += 4;
}

static void str_code_line(str_code* d) {
    if(char_type[d->c[d->n + 1]] == STR_CODE_LINE && d->c[d->n] != d->c[d->n + 1]) {
        d->n = 2;
        return;
    }

    ++d->n;
}

static void str_code_sign(str_code* d) {
    ++d->n;

    while(d->n < d->l && char_type[d->c[d->n]] == STR_CODE_SIGN) ++d->n;
}

static void str_code_space(str_code* d) {
    ++d->n;

    while(d->n < d->l && char_type[d->c[d->n]] == STR_CODE_SPACE) ++d->n;
}

static void str_code_quote(str_code* d) {  // quote is ended on same quote sign or line end

    byte_t q = d->c[d->n];

    ++d->n;

    while(d->n < d->l && d->c[d->n] != q && d->c[d->n] != 0x0D && d->c[d->n] != 0x0A) {
        if(d->c[d->n] == '\\' && d->n + 1 < d->l) ++d->n;
        ++d->n;
    }

    if(d->n < d->l && d->c[d->n] == q) ++d->n;
}

static void str_code_comment(str_code* d) {
    ++d->n;

    if(d->n < d->l) switch(d->c[d->n]) {
        case '*':

            ++d->n;

            if(d->n < d->l) do {
                    while(d->n < d->l && d->c[d->n] != '*') ++d->n;

                    if(d->n < d->l) ++d->n;

                } while(d->n < d->l && d->c[d->n] != '/');

            if(d->n < d->l) ++d->n;

            break;

        case '/':

            ++d->n;

            while(d->n < d->l && char_type[d->c[d->n]] != STR_CODE_LINE) ++d->n;  // not include end of line

            break;

        default: break;
        }
}

static void str_code_bracket(str_code* d) {
    byte_t e = 0;

    switch(d->c[d->n]) {
    case '(': e = ')'; break;
    case '[': e = ']'; break;
    case '{': e = '}'; break;
    }

    ++d->n;

    if(e)
        while(d->n < d->l) {
            if(d->c[d->n] == e) {
                ++d->n;
                break;
            }

            switch(char_type[d->c[d->n]]) {
            case STR_CODE_QUOTE: str_code_quote(d); break;
            case STR_CODE_COMMENT: str_code_comment(d); break;
            case STR_CODE_BRACKET: str_code_bracket(d); break;
            default: ++d->n; break;
            }
        }
}

size_t str_code_next(str* s, size_t pos, uint_t* type) {
    size_t l = str_length(s);

    if(pos > l) return l;

    str_code d = { (byte_t*)str_data(s), l, pos };
    uint_t   t = str_code_type(s, pos);

    if(type) *type = t;

    str_codes[t](&d);

    return d.n;
}
