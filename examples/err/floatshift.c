int printf(const char* fmt, ...);

int main() {
    float f = 1.5f;
    f <<= 2;
    printf("%f\n", f);
    return 0;
}
