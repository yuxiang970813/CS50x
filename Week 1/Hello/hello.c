#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Prompt for user name
    string name = get_string("What's your name? ");

    // Say hello to user
    printf("Hello, %s.\n", name);
}