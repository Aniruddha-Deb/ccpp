int printf(const char* fmt, ...);

int main() {
    float x = 5.0;
    double f = x;
    printf("%f\n", f);
    printf("%f\n", f/1.f);
    printf("%f\n", f/3.f);
    return 0;
}
