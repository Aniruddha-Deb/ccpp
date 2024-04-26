int printf(const char* fmt, ...);

char getchar(){
    return 'c';
}

int getint(){
    return 5;
}

float getfloat(){
    return 5.f;
}

int main() {
    printf("%f\n", 5.f+(2+'c'));
    printf("%f\n", (getchar() + getint()) + getfloat() + 1.0);
    return 0;
}
