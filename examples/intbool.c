#include <stdio.h>

int main() {
    int a = 2, b = 5;

    if (a && b) printf("logical and\n");
    if (a & b) printf("bitwise and\n");

    return 0;
}
