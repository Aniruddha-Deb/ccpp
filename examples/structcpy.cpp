struct A {
    int a;
    int b;
};

void copy_struct() {
    A a{5, 2};
    A b = a; // invokes copy constructor of A
}
