int printf(const char* fmt, ...);

int getint() {
    return 4;
}

float getfloat() {
    return 3; // should autowiden to 3.f without issues
}

int main() {
    int t = -5;
    printf("%d\n", -t);
    printf("%d\n", -getint());
    printf("%d\n", ~getint());
    printf("%f\n", -getfloat());
    printf("%u\n", -5U);
    return -t;
}
