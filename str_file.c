
#include <sys/stat.h>
#include <stdio.h>

#include "lib.h"
#include "com.h"
#include "str_file.h"

#define STR_LOAD_BUFFER 1024

static void _str_load_stdin(str* s) {

    char b[STR_LOAD_BUFFER + 1];

    run {

        size_t r = fread(b, 1, STR_LOAD_BUFFER, stdin); czb(r);

        b[r] = 0;

        str_add(s, b);

        if(r < STR_LOAD_BUFFER) break;

    }

}

str* str_load(str_pool* p, text_t source) {

    str* r;

    if(source == NULL) {

        r = str_init(p, NULL);

        _str_load_stdin(r);

        return r;

    }

    struct stat st;

    if(stat(source, &st) < 0 || (st.st_mode & S_IFREG) == 0 || st.st_size == 0) return NULL;

    FILE* f = fopen(source, "r"); if(f == NULL) return NULL;

    r = str_init(p, NULL);

    str_ask(r, st.st_size);

    char* d = str_data(r);

    fread(d, 1, st.st_size, f);

    fclose(f);

    return r;

}

bool str_save(str* s, text_t target) { 

    FILE* f;

    if(target) { 
      
        f = fopen(target, "w"); 
    
        if(f == NULL) return false;

    }
    else f = stdout;

    char*  d = str_data(s);
    size_t l = text_size(d, 0); if(l > 0) l--;

    size_t r = fwrite(d, l, 1, f);

    if(f != stdout) fclose(f);

    return r > 0;

}
