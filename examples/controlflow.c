

int printf(const char* format, ...);

int main(){
    int i = 0, j;
    while(i < 10){
        j = 0;
        while(j < 10){
            if (j % 2 == 0) {
                printf("%d ", j + 2);
            }
            else{
                printf("%d ", i + 0);
            }
            j+=1;
        }
        printf("\n");
        i+=1;
    }
}
