
#include "ccl/ccl.h"
#include <stdio.h>

size_t dump_str(txt* o, txt_l* p);

int main(int argc, char* argv[]) {
    arg_init();
    arg_add_arg("file");
    arg_load(argc, argv);
    str* fn = arg_arg("file");
    if(fn == NULL) {
        printf("usage: test1 file\n");
        arg_fini();
        return 0;
    }
    printf("file = %s\n", str_data(fn));
    str_pool* sp = str_pool_init(0, NULL);
    if(sp == NULL) {
        printf("str_pool_init failed\n");
        return 0;
    }
    str* s = str_init(sp, str_data(fn));
    if(s == NULL) {
        printf("str_init failed\n");
        return 0;
    } else {
        printf("str = %s\n", str_data(s));
    }
    arg_fini();
    txt* t = new_txt();
    if(t == NULL) {
        printf("text_init() failed\n");
        return 0;
    }
    if(txt_load(t, str_data(s), NULL)) {
        printf("txt_count = %zu\n", txt_count(t));
    } else {
        printf("txt_load failed\n");
    }
    txt_loop(t, dump_str, NULL);
    return 0;
}

size_t dump_str(txt* o, txt_l* p) {
    printf("%s", str_data(p->d));
    return 0;
}
