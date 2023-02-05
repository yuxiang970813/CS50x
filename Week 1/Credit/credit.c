#include <cs50.h>
#include <stdio.h>

void checksum(long num);

int main(void)
{
    // Prompt for input
    long number = get_long("Number: ");

    // Calculate checksum
    int checksum = 0;
    for (long i = number; i > 0; i /= 100)
    {
        // Get last 2 digit each iterate
        int j = i % 100;

        // Get the tens digit
        int tens_digit = 2 * (j / 10);
        if (tens_digit >= 10)
        {
            tens_digit = (tens_digit / 10) + (tens_digit % 10);
        }
        // Get the ones digit
        int ones_digit = j % 10;

        // Sum
        checksum += tens_digit + ones_digit;
    }
    checksum %= 10;

    // Check for starting digits
    int amex_start = number / 10000000000000;
    int mastercard_start = number / 100000000000000;
    int visa16_start = number / 1000000000000000;
    int visa13_start = number / 1000000000000;

    // Print result
    if (checksum == 0)
    {
        if (amex_start == 34 || amex_start == 37)
        {
            printf("AMEX\n");
        }
        else if (mastercard_start >= 51 && mastercard_start <= 55)
        {
            printf("MASTERCARD\n");
        }
        else if (visa13_start == 4 || visa16_start == 4)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}