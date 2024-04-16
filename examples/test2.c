int factorial(int n)
{
	if (n <= 0)
		return 1;
	else return n*factorial(n-1);
	return -1;
}

int printf(char const *format, ...);

int fibonacci(int a, int b, int n)
{
	if (n <= 0)
		return a;

	while (n > 0) {
		int t;
		t = b;
		b = a+b;
		a = t;
		n = n-1;
	}
	return b;
}

int p;

int main()
{
  p =3;
  printf("%d", fibonacci(factorial(1), factorial(2), factorial(p)));
  return 0;
}