int printf(const char* fmt, ...);

int getint(int a) {
    return a;
}

int main(int argc, char** argv) {

    int a = 2;
    while (getint(2) == 4) {
        printf("Shouldn't be generated\n");
    }

    printf("%d\n", a);
    return 0;
}
