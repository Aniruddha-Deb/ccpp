int printf(const char *fmt, ...);

int main() {
    int b = 10;
    int *a = &b;
    *a = 20;
    printf("%d %d\n", *a, b);
    return 0;
}
