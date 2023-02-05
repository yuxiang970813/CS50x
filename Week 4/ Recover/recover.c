#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;
const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Open memory card
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open %s.\n", argv[1]);
        return 1;
    }

    // Keep track of JPEG found
    int JPEG_counter = 0;

    // Making new JPEG name
    char *filename = malloc(sizeof(char) * 8);
    sprintf(filename, "%03i.jpg", JPEG_counter);

    // Open output file
    FILE *output = fopen(filename, "w");
    if (output == NULL)
    {
        printf("Could not open %s.\n", filename);
        return 1;
    }

    // Reading file
    BYTE *buffer = malloc(BLOCK_SIZE * sizeof(BYTE));
    while (fread(buffer, 1, BLOCK_SIZE, input) == BLOCK_SIZE)
    {
        // Look for beginning of a JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // Found first JPEG
            if (JPEG_counter == 0)
            {
                fwrite(buffer, 1, BLOCK_SIZE, output);
                JPEG_counter++;
            }
            // Already found JPEG
            else
            {
                // Close the previous file
                fclose(output);

                // Open new file
                sprintf(filename, "%03i.jpg", JPEG_counter);
                output = fopen(filename, "w");
                fwrite(buffer, 1, BLOCK_SIZE, output);
                JPEG_counter++;
            }
        }
        else
        {
            if (JPEG_counter > 0)
            {
                fwrite(buffer, 1, BLOCK_SIZE, output);
            }
        }
    }
    // Close file
    fclose(input);
    fclose(output);

    // Free memory
    free(buffer);
    free(filename);
}