int factorial(int n)
{
	if (n <= 0)
		return 1;
	else return n*factorial(n-1);
}

int fibonacci(int a, int b, int n, int*** r)
{
	if (n <= 0){
		(*(*(*r))) = a;
		return a;
	}

	while (n > 0) {
		int t;
		t = b;
		b = a+b;
		a = t;
		n = n-1;
	}
	(*(*(*r))) = b;
	return b;
}

int printf(char * format, ...);

int main()
{
  int ***a;
  int **b;
  int *c;
  int d;
  c = &d;
  b = &c;
  a = &b;
  fibonacci(factorial(1), factorial(2), factorial(5), a);
  printf("%d", d);
}
