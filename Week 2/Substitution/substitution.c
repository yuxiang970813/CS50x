#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Function prototype
int check_alphabet(string text);
string upper_string(string text);
int chech_repeat(string text);

int main(int argc, string argv[])
{
    // Check for valid key
    if (argc != 2)
    {
        // Remind the user how to use the program
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // Convert key to uppercase
    string key = upper_string(argv[1]);

    if (argc == 2)
    {
        // Check key length
        if (strlen(key) != 26)
        {
            printf("Key must contain 26 characters.\n");
            return 1;
        }
        // Check for non-alphabetic char
        else if (check_alphabet(key) == 0)
        {
            printf("Key must contain alphabetic characters.\n");
            return 1;
        }
        // Check for repeated char(case-insensitive)
        else if (chech_repeat(key) == 0)
        {
            printf("Key must  not contain repeated characters.\n");
            return 1;
        }
    }

    // Get plaintext
    string plain_text = get_string("plaintext: ");

    // Encipher
    for (int i = 0; i < strlen(plain_text); i++)
    {
        if (islower(plain_text[i]))
        {
            plain_text[i] = tolower(key[plain_text[i] - 97]);
        }
        else if (isupper(plain_text[i]))
        {
            plain_text[i] = key[plain_text[i] - 65];
        }
    }

    // Print cipher text
    printf("ciphertext: %s\n", plain_text);
}

// Check for non-alphabetic char
int check_alphabet(string text)
{
    // Iterate each letter
    for (int i = 0; i < strlen(text); i++)
    {
        // Return 0 if find non-alphabetic char
        if (isalpha(text[i]) == 0)
        {
            return 0;
        }
    }

    return 1;
}

// Convert string to uppercase
string upper_string(string text)
{
    for (int i = 0; i < strlen(text); i++)
    {
        text[i] = toupper(text[i]);
    }

    return text;
}

// Check for repeated char(case-insensitive)
int chech_repeat(string text)
{
    // Iterate each word
    int len = strlen(text);
    for (int i = 0; i < len; i++)
    {
        // Iterate other word
        for (int j = i + 1; j < len; j++)
        {
            // Retrun 0 if find repeated char
            if (text[j] == text[i])
            {
                return 0;
            }
        }
    }

    return 1;
}