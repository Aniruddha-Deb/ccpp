int printf(const char* fmt, ...);

int main() {
    short int a = 5;
    long int l = 10L;
    unsigned short c = 2.0;
    unsigned short k = 15;
    unsigned short u = k / c;
    double d = 8.2;
    // short int a;
    // long int l;
    // unsigned short c;
    // unsigned short k;
    // unsigned short u;
    // double d;
    // a = 5;
    // l = 10L;
    // c = 2;
    // k = 15;
    // u = k / c;
    // d = 8.2;
    printf("%d %ld %u %u %u\n", a, l, c, k, u);
    printf("%lf\n", d);
    return 0;
}
