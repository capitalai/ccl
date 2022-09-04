
#include "ccl/txt_file.h"
#include "ccl/lib.h"
#include "ccl/str_code.h"
#include "ccl/str_file.h"
#include <stdio.h>

bool txt_load(txt* t, text_t source_file, txt_loader* f) {
    str_pool* sp = str_pool_init(0, NULL);
    str*      s  = str_init(sp, NULL);

    cnf(str_load(s, source_file));

    size_t p = 0;
    size_t n = str_length(s);

    ifx(f) f = txt_load_line;

    while(p < n) p = f(t, s, p);

    str_pool_fini(sp);

    return false;
}

bool txt_save(txt* t, text_t target_file) {
    FILE* f;

    if(target_file) {
        f = fopen(target_file, "w");

        if(f == NULL) return false;

    } else f = stdout;

    size_t n = txt_count(t);
    size_t w = 0;

    for(size_t i = 0; i < n; i++) {
        str* s = txt_get(t, i);

        w += fwrite(str_data(s), str_length(s), 1, f);
        w += fwrite("\n", 1, 1, f);
    }

    if(f != stdout) fclose(f);
    else fflush(f);

    return w > 0;
}

size_t txt_load_line(txt* t, str* s, size_t p) {
    char*  d = str_data(s) + p;
    size_t n = str_length(s) - p;
    size_t i = 0;

    while(i < n && d[i] != '\n') i++;

    str* ts = txt_add(t, NULL, NULL);

    str_set(ts, d, i + 1);

    return d[i] == '\n' ? p + i + 1 : p + i;
}

size_t txt_load_trim_line(txt* t, str* s, size_t p) {
    char*  d = str_data(s) + p;
    size_t n = str_length(s) - p;
    size_t i = 0;

    while(i < n && d[i] != '\n') i++;

    if(i > 0) {
        str* ts = txt_add(t, NULL, NULL);

        str_set(ts, d, i + 1);
    }

    return d[i] == '\n' ? p + i + 1 : p + i;
}

size_t txt_load_trim_space(txt* t, str* s, size_t p) {
    char*  d = str_data(s) + p;
    size_t n = str_length(s) - p;
    size_t i = 0;

    while(i < n && d[i] == ' ' || d[i] == '\t') i++;

    size_t b = i;  // begin
    size_t e = b;  // end

    while(i < n && d[i] != '\n') {
        if(d[i] != ' ' && d[i] != '\t') e = i + 1;

        i++;
    }

    if(i > 0) {
        str* ts = txt_add(t, NULL, NULL);

        str_set(ts, d + b, e - b + 1);
    }

    return d[i] == '\n' ? p + i + 1 : p + i;
}

size_t txt_load_code(txt* t, str* s, size_t p) {
    char*  d = str_data(s);
    size_t n = str_length(s);
    uint_t x;
    size_t b = p;
    size_t e;

    do {
        e = str_code_next(s, b, &x);
        if(x == STR_CODE_SPACE || x == STR_CODE_COMMENT) b = e;
        else break;
    } while(e < n);

    if(e > b) {
        str* ts = txt_add(t, NULL, NULL);

        str_set(ts, d + b, e - b + 1);
    }

    return 0;
}
