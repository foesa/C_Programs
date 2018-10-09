#include <stdio.h>
int main(void) {
    char c = 42;
    // double d = (double)c; The cast is not needed here, because ...
    double d = c;    // ... the conversion is done implicitly.
    printf("c = %d\n", c);
    printf("d = %f\n", d);
    return 0;
}
