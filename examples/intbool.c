#include <complex.h>
int printf(const char* fmt, ...);

int main() {
    _Bool bool = 1;
    int a = 2, b = 5;
    float pz = 0.f, nz = -0.f;
    float notz = 1.f;

    if (a && b) printf("logical and\n");
    if (pz) printf("Positive Zero!\n");
    if (nz) printf("Negative Zero!\n");
    if (notz) printf("Not zero\n");
    if (a & b) printf("bitwise and\n");

    return 0;
}
