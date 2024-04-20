#include <stdio.h>

int main() {
    int a[3] = {2, 3, 4};
    int *b = a;
    *(b++) += 100;
    printf("%d %d %d %d\n", a[0], a[1], a[2], (int)(b-a)); 
    return 0;
}
