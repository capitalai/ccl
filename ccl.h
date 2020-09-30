//
//  ccl.h
//  lib_ccl
//
//  Created by user on 2018/7/18.
//  Copyright © 2018年 Infinita. All rights reserved.
//

#ifndef ccl_h
#define ccl_h

#include "std.h"
#include "lot.h"
#include "sac.h"
#include "str.h"
#include "pin.h"
#include "seq.h"
#include "pod.h"
#include "set.h"
#include "key.h"

// memory management: bag, sac, lot
// building object:             obj
// basic data collection:       pin, seq, pod
// indexed data:                          set, key, txt
// complex data:                          var, dim, row, dot, dic

// TODO:
//
//  - add var, dim, dot (can add / del for data, text, ptr, obj, var, dim, dot)
// 
//  - deprecated design
//
//  - add set object: pod + a list which can store data with index
//  - add hub object: set + child hub
//  - add box object: set + a list which can store data with same index
//  - add let object: set for creating typed data
//  - add row object: lot + can add a series of typed data
//  - add txt object: pod + string item only
//  - add key object: set + string item only
//  - add lex object: hub + string item only
//  - add tag object: box + string index
//  - add fit object: lot + can add a series of string processing function
//  - add def object: set + string index -> string data
//  - add var object: set + string index -> typed data
//  - add pad object: row + binary output functions for each item
//  - add dim object: multiple index mapping to data
//  - add map object: complex data matching and mapping to data

// ISSUES:
//
//  - when finishing object, remove pick, do not remove hold
//  - add all dropped memory to sac slot
//  - check memory problem of sac
//

#endif /* ccl_h */
