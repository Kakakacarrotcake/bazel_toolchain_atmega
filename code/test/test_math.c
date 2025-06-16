// #include "gtest/gtest.h"
#include "hdr/math.h"
#include <assert.h>


int main() {
    int x = 5;
    int y = 7;

    int x_y = x + y;
    assert( add(x, y) == x_y);

    return 0;
}