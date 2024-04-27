char* malloc(unsigned int size);
void free(char* ptr);

int printf(const char *format, ...);

int main() {
    char *s = malloc(14);
    *s = 'b';
    printf(s);
    free(s);
    return 0;
}
