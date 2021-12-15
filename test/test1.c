
#include "ccl/ccl.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    arg_init();
    arg_add_arg("file");
    arg_load(argc, argv);
    str *s = arg_arg("file");
    if(s == NULL) {
        printf("usage: test1 file\n");
        arg_fini();
        return 0;
    }
    printf("file = %s\n", str_data(s));
    arg_fini();
    return 0;
}
