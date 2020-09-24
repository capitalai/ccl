//
//  std.h
//  lib_ccl
//
//  Created by Capita Lai on 2018/7/21.
//  Copyright © 2018 Capita Lai. All rights reserved.
//

#ifndef std_h
#define std_h

#include "inc.h"
#include "bag.h"

typedef struct std_a std_a;

struct std_a {

    bool        rand;           // use random number
    bag_init_f* new_bag;        // default bag_init
    size_t      sac_pack_size;  // default sac pack size
    size_t      txt_size;       // default txt size
    size_t      str_add_rate;   // default txt add rate

};

extern std_a std;

extern void std_init(void);

#endif /* std_h */
