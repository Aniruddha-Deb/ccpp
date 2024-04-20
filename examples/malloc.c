void* malloc(unsigned long size);
void free(void* ptr);

int printf(const char *format, ...);

int main() {
    char *s = malloc(14);
    s = "Hello World!\n";
    s[13] = 0;
    printf(s);
    free(s);
    return 0;
}
