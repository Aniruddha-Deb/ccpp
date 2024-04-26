int printf(const char* fmt, ...);

int main() {
    float f = 5.f;
    printf("%f\n", f);
    printf("%f\n", f/1.f);
    printf("%f\n", 2.f/3.f);
    return 0;
}
