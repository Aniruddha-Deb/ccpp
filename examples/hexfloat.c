int printf(char const* format, ...);

float getfloat(){
    float p = 500.0;
    return 0x3AD.4p2f + p;
}

int main() {
    float f = getfloat();
    printf("%f\n", f);
    return 0;
}
