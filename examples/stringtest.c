int printf(const char * format, ...);

int main(){
    char *c = "Hello";
    char *d = "World";
    char *e = "Multiple";
    char *f = "String";
    signed char *g = "Literals";
    g = "NO";
    printf("%s %s %s %s %s\n", c, d, e, f, g);
    return 0;
}
