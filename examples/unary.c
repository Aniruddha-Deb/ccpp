int printf(const char* fmt, ...);

int main() {
    int t = -5;
    printf("%d\n", -t);
    printf("%u\n", -5U);
    return -t;
}
