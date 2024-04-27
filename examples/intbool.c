// #include <complex.h>
int printf(const char* fmt, ...);

int getint(int a){
    return a;
}

float getfloat(float f){
    return f;
}

int main() {
    _Bool bool = 1;
    int a = 2, b = 5;
    float pz = 0.f, nz = -0.f;
    float notz = 1.f;

    if (getint(5) && getint(2)) printf("logical and\n");
    if (pz) printf("Positive Zero!\n");
    if (getfloat(nz) + 1.0  + (getfloat(notz) == getfloat(notz))) printf("Negative Zero!\n");
    if (getfloat(notz) + 1.0) printf("Not zero\n");
    if (a & b) printf("bitwise and\n");
    int x = (getint(2) > getint(1)) + getint(2);
    printf("helloworld %d\n", x);

    return 0;
}
