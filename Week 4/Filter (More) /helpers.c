#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate row
    for (int i = 0; i < height; i++)
    {
        // Iterate column
        for (int j = 0; j < width; j++)
        {
            // Calculate average colour
            int average_colour = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);

            // Cover average_colour to original pixel
            image[i][j].rgbtBlue = average_colour;
            image[i][j].rgbtGreen = average_colour;
            image[i][j].rgbtRed = average_colour;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate row
    for (int i = 0; i < height; i++)
    {
        // Iterate half column
        for (int j = 0; j < width / 2; j++)
        {
            // Temp pixel to store
            RGBTRIPLE tmp = image[i][j];

            // Swap pixels
            int opposite = width - 1 - j;
            image[i][j] = image[i][opposite];
            image[i][opposite] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // tmp to store
    RGBTRIPLE tmp[height][width];

    // Iterate row
    for (int i = 0; i < height; i++)
    {
        // Iterate column
        for (int j = 0; j < width; j++)
        {
            // Initialize colour counter
            float blue_counter = 0;
            float green_counter = 0;
            float red_counter = 0;
            float pixel_counter = 0;

            // Iterate 9 x 9
            for (int x = i - 1; x <= i + 1; x++)
            {
                for (int y = j - 1; y <= j + 1; y++)
                {
                    // Ignore outside edge
                    if (x >= 0 && x < height && y >= 0 && y < width)
                    {
                        blue_counter += image[x][y].rgbtBlue;
                        green_counter += image[x][y].rgbtGreen;
                        red_counter += image[x][y].rgbtRed;
                        pixel_counter++;
                    }
                }
            }

            // Calculate average then copy to tmp
            tmp[i][j].rgbtBlue = round(blue_counter / pixel_counter);
            tmp[i][j].rgbtGreen = round(green_counter / pixel_counter);
            tmp[i][j].rgbtRed = round(red_counter / pixel_counter);
        }
    }

    // Cover tmp to image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = tmp[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Copy image to tmp
    RGBTRIPLE tmp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            tmp[i][j] = image[i][j];
        }
    }

    // Iterate row
    for (int i = 0; i < height; i++)
    {
        // Iterate column
        for (int j = 0; j < width; j++)
        {
            // Initialize Gx counter
            float Gx_Blue_counter = 0;
            float Gx_Green_counter = 0;
            float Gx_Red_counter = 0;

            // Calculate Gx (-1 to 1)
            for (int x = i - 1; x <= i + 1; x += 2)
            {
                for (int y = j - 1, gx = -1; y <= j + 1; y += 2, gx += 2)
                {
                    if (x >= 0 && x < height && y >= 0 && y < width)
                    {
                        Gx_Blue_counter += gx * tmp[x][y].rgbtBlue;
                        Gx_Green_counter += gx * tmp[x][y].rgbtGreen;
                        Gx_Red_counter += gx * tmp[x][y].rgbtRed;
                    }
                }
            }
            // Calculate Gx (-2 to 2)
            for (int x = j - 1, gx = -2; x <= j + 1; x += 2, gx += 4)
            {
                if (x >= 0 && x < width)
                {
                    Gx_Blue_counter += gx * tmp[i][x].rgbtBlue;
                    Gx_Green_counter += gx * tmp[i][x].rgbtGreen;
                    Gx_Red_counter += gx * tmp[i][x].rgbtRed;
                }
            }

            // Initialize Gy counter
            float Gy_Blue_counter = 0;
            float Gy_Green_counter = 0;
            float Gy_Red_counter = 0;

            // Calculate Gy (-1 to 1)
            for (int x = i - 1, gy = -1; x <= i + 1; x += 2, gy += 2)
            {
                for (int y = j - 1; y <= j + 1; y += 2)
                {
                    if (x >= 0 && x < height && y >= 0 && y < width)
                    {
                        Gy_Blue_counter += gy * tmp[x][y].rgbtBlue;
                        Gy_Green_counter += gy * tmp[x][y].rgbtGreen;
                        Gy_Red_counter += gy * tmp[x][y].rgbtRed;
                    }
                }
            }
            // Calculate Gy (-2 to 2)
            for (int x = i - 1, gy = -2; x <= i + 1; x += 2, gy += 4)
            {
                if (x >= 0 && x < height)
                {
                    Gy_Blue_counter += gy * tmp[x][j].rgbtBlue;
                    Gy_Green_counter += gy * tmp[x][j].rgbtGreen;
                    Gy_Red_counter += gy * tmp[x][j].rgbtRed;
                }
            }

            // Compute new value
            int Gxy_Blue = round(sqrt(pow(Gx_Blue_counter, 2) + pow(Gy_Blue_counter, 2)));
            int Gxy_Green = round(sqrt(pow(Gx_Green_counter, 2) + pow(Gy_Green_counter, 2)));
            int Gxy_Red = round(sqrt(pow(Gx_Red_counter, 2) + pow(Gy_Red_counter, 2)));

            // Ignore over 255
            if (Gxy_Blue > 255)
            {
                Gxy_Blue = 255;
            }
            if (Gxy_Green > 255)
            {
                Gxy_Green = 255;
            }
            if (Gxy_Red > 255)
            {
                Gxy_Red = 255;
            }

            // Cover new value to image
            image[i][j].rgbtBlue = Gxy_Blue;
            image[i][j].rgbtGreen = Gxy_Green;
            image[i][j].rgbtRed = Gxy_Red;
        }
    }
    return;
}