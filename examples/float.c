int printf(const char* fmt, ...);

int getint(int a){
    return a;
}

int main() {
    float x = 5.0;
    double f = x;
    printf("%f\n", f);
    printf("%f\n", f/1.f);
    printf("%f\n", f/3.f);
    printf("%f\n", 1.0 + (getint(2) < getint(1)));
    return 0;
}
