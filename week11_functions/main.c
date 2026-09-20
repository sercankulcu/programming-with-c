void try_to_double(int n)
{
    n = n * 2;             /* modifies the copy */
}

int main(void)
{
    int value = 5;
    try_to_double(value);
    printf("%d\n", value); /* 5 — unchanged */
}
