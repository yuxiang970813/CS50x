#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

// Function prototype
int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // Getting user input
    string input_text = get_string("Text: ");

    // Calculate Coleman-Liau Index
    double L = 100.00 * count_letters(input_text) / count_words(input_text);
    double S = 100.00 * count_sentences(input_text) / count_words(input_text);
    int index = round(0.0588 * L - 0.296 * S - 15.8);

    // Output
    if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

// Count the number of letters
int count_letters(string text)
{
    // Keep track of letters
    int counter = 0;

    // Iterate each char
    for (int i = 0; i < strlen(text); i++)
    {
        // Update counter if find alphabet
        if (isalpha(text[i]))
        {
            counter++;
        }
    }

    return counter;
}

// Count the number of words
int count_words(string text)
{
    // Keep track of letters
    int counter = 0;

    // Iterate each char
    int len = strlen(text);
    for (int i = 0; i < len; i++)
    {
        // Update counter if find space(ignore multiple spaces)
        if (text[i] == ' ' && text[i + 1] != ' ')
        {
            counter++;
        }
    }

    // Check containing at least one word
    if (len > 0)
    {
        counter++;

        // Ignore start with space
        if (text[0] == ' ')
        {
            counter--;
        }

        // Ignore end with a space
        if (text[len - 1] == ' ')
        {
            counter--;
        }
    }

    return counter;
}

// Counter the number of sentences
int count_sentences(string text)
{
    // Keep track of sentences
    int counter = 0;

    // Iterate each char
    for (int i = 0; i < strlen(text); i++)
    {
        // Update counter if find '.', '!' or '?'
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            counter++;
        }
    }

    return counter;
}