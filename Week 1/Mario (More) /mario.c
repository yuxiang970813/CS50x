#include <cs50.h>
#include <stdio.h>

// Function prototype
void print_hash(int x);

int main(void)
{
    // Prompt for height(must >= 1, <= 8)
    int height;
    do
    {
        height = get_int("Height: ");
    } while (height < 1 || height > 8);

    // Print result
    for (int i = 1; i <= height; i++)
    {
        // Print spaces
        for (int j = height; j > i; j--)
        {
            printf(" ");
        }

        // Print left hashes
        print_hash(i);

        // Print gap
        printf("  ");

        // Print righ hashes
        print_hash(i);

        // Next row
        printf("\n");
    }
}

void print_hash(int x)
{
    for (int i = 0; i < x; i++)
    {
        printf("#");
    }
}