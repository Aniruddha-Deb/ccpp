int printf(const char* fmt, ...);

int main() {
    float f = 5.f;
    printf("%f\n", f);
    printf("%f\n", f/0.f);
    printf("%f\n", 0.f/0.f);
    return 0;
}
