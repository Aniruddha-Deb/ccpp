int foo(int var, int* ptr);

void printf(char* format, ...);

int main(){
    // int x = 5, y =10 * x, z, w;
    // z = x;
    // int *p = &(*(&z));
    // w = 8 + y * x;
    // w = foo(w, &x);

    // if (y > 5) {
    //     w = 2;
    //     x = 2;
    //     w = w + x;
    // }

    // while ((y = 2) == 0){
    //     x = y * y;
    // }

    // return w + z + x + y;

    int a = 3,b = a,c = a + b,d,e = a + b * c / a,f = e * e,g,h;
 
    // a = (a + 1) + (b + 2) + (a = a + (b = (c + 9))) + (e = f = g = (c = (d = 7))) + d;
    a = 4;
    a = a + 2;
    a+=2;
    a+=b;
    e += a + b + c;
    b = (a = 2 + 3) + (b = a = a + 1);
    c = a + b;
    printf("%d %d %d %d %d",a , b, c, e, f);
    // a = a + (a = a + 1);
}