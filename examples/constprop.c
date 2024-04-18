int foo(int var, int* ptr);

int main(){
    int x = 5, y =10 * x, z, w;
    z = x;
    int *p = &(*(&z));
    w = 8 + y * x;
    w = foo(w, &x);

    if (y > 5) {
        w = 2;
        x = 2;
        w = w + x;
    }

    while ((y = 2) == 0){
        x = y * y;
    }

    return w + z + x + y;
}