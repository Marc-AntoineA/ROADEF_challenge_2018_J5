#pragma once

#include <cstdlib>

unsigned int randint(unsigned int begin, unsigned int last) {
    return rand() % (last - begin) + begin;
}