int printf(const char* fmt, ...);

int main() {
    int a = 5;
    int *b = &a;
    int **c = &b;
    int ***d = &c;

    ***d = 8; // wow
    printf("%d %d %d %d", a, *b, **c, ***d);
    return 0;
}
