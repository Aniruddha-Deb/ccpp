void printf(const char* fmt, ...);

int glob = 2 * 3;

int main() {
    
    glob = 5;
    printf("%d\n", glob);

    return 0;
}
