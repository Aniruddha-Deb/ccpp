int printf(const char* fmt, ...);

int var1 = 2;
unsigned int var2, var3;
float var4;
int *ptr;

int fun(int b){
    var2 = var3 = 5;
    var2 += b;
    ptr = &var3;
    var2 = 1;
    (*ptr) = var2 + 1;
}

int main(){
    fun(3);
    printf("%d %ud %ud %f %d\n", var1, var2, var3, var4, *ptr);
}
