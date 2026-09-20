#include <stdio.h>

int main(void)
{
    double a = 0.1, b = 0.2;
    printf("%.20f\n", a + b);
    printf("%s\n", (a + b == 0.3) ? "equal" : "not equal");
    return 0;
}
