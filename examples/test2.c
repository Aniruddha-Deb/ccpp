int factorial(int n)
{
	if (n <= 0)
		return 1;
	else return n*factorial(n-1);
	return 0;
}

int printf(char const *format, ...);

int fibonacci(int a, int b, int n, int *r)
{
	if (n <= 0)
		(*r) =  a;

	while (n > 0) {
		int t;
		t = b;
		b = a+b;
		a = t;
		n = n-1;
	}
	(*r) = b;
	return 0;
}


int main()
{
  int p =3;
  int ans;
  fibonacci(factorial(1), factorial(2), factorial(p), &ans);
  printf("%d", ans);
  return 0;
}