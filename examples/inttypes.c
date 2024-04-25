int printf(const char* fmt, ...);

int main() {
    short int a = 5;
    long int l = 10L;
    unsigned short c = 2;
    unsigned short k = 15;
    unsigned short u = k / c;
    double d = 8.2;
    printf("%d %ld %u %u %u\n", a, l, c, k, u);
    printf("%lf\n", d);
    return 0;
}
