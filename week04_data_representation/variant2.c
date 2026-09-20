#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Print the bits of an int, most significant first. */
static void print_int_bits(int value)
{
    for (int bit = 31; bit >= 0; bit--) {
        putchar((value >> bit) & 1 ? '1' : '0');
        if (bit % 8 == 0 && bit != 0) {
            putchar(' ');
        }
    }
    putchar('\n');
}

/* Print the raw bytes of any object, lowest address first. */
static void print_raw_bytes(const void *object, size_t size)
{
    const unsigned char *byte = object;
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", byte[i]);
    }
    putchar('\n');
}

int main(void)
{
    int positive = 13;
    int negative = -13;

    printf("13  decimal = %d, octal = %o, hex = %X\n", positive, positive, positive);
    printf("13  bits    = ");  print_int_bits(positive);
    printf("-13 bits    = ");  print_int_bits(negative);

    printf("\nsum of the two: %d\n", positive + negative);

    float f = 0.1f;
    printf("\n0.1f occupies %zu bytes: ", sizeof f);
    print_raw_bytes(&f, sizeof f);
    printf("0.1f printed to 20 places: %.20f\n", (double)f);

    printf("\n'A' = %d, 'A' + 1 = %c, '7' - '0' = %d\n",
           'A', 'A' + 1, '7' - '0');

    printf("\nlargest int  = %d\n", 2147483647);
    printf("plus one     = %d\n", 2147483647 + 1);  /* see the note below */

    return EXIT_SUCCESS;
}
