char* malloc(int size);

int printf(char *format, ...);

int main() {
    char *c;
    char* d = "hello world\n";
    *c = *d;
    (d + 1);
    printf(c);
    return 0;
}
