int foo(int a){
    int b = 7, c = 2;
    b = 31;
    return a + 5;
}

int ptrtest(){
    int * i;
    int b = 0;
    i = &(*(&b));
    (*i) = (*i) + b;
    (*i) = foo(*i);
    return 0;
}

int main(){
    int b = 10;
    float d = 4.2;
    float e = 4.2;
    // z = x * y;
    b = 17;
    int a = 9;
    foo(a*10);
    float x, y = d + e, z;
    double w = 5.3;
    b =( a = 10);
    float* ad;
    (a >= b) & (4 > 2);
    return 0;
}
