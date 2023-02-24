//
// Created by wuliuqi on 2023/2/24.
//

#include "include/util.h"
#include "util.h"

#include <cstdio>
#include <cstdlib>

void ErrorIf(bool condition, const char *errmsg) {
    if (condition) {
        perror(errmsg);
        exit(EXIT_FAILURE);
    }
}