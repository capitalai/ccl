cmake_minimum_required(VERSION 3.19)
project(ccl)
add_library(ccl SHARED com.c dir.c std.c bag.c lot.c sac.c str.c str_file.c str_utf8.c obj.c pin.c seq.c pod.c set.c key.c var.c var_text.c)
install(TARGETS ccl LIBRARY DESTINATION lib PUBLIC_HEADER DESTINATION include/ccl)
