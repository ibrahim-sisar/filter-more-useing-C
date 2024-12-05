#include "helpers.h"
#include <math.h>
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculate the average as a float, then round and cast to int
            int n =
                round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = n;
            image[i][j].rgbtGreen = n;
            image[i][j].rgbtRed = n;
        }
    }
    return;
}

// Reflect image horizontally

void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // Swap entire RGBTRIPLE structs instead of individual colors
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sumRed = 0, sumGreen = 0, sumBlue = 0;
            int pixelCount = 0;

            for (int m = i - 1; m <= i + 1; m++)
            {
                for (int n = j - 1; n <= j + 1; n++)
                {
                    if (m >= 0 && m < height && n >= 0 && n < width)
                    {
                        sumRed += image[m][n].rgbtRed;
                        sumGreen += image[m][n].rgbtGreen;
                        sumBlue += image[m][n].rgbtBlue;
                        pixelCount++;
                    }
                }
            }

            temp[i][j].rgbtRed = round((float) sumRed / pixelCount);
            temp[i][j].rgbtGreen = round((float) sumGreen / pixelCount);
            temp[i][j].rgbtBlue = round((float) sumBlue / pixelCount);
        }
    }

    // Copy the blurred values back to the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Define Sobel kernels
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Temporary array to store the new pixel values
    RGBTRIPLE temp[height][width];

    // Loop through each pixel in the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Initialize gradient sums for Gx and Gy
            int GxRed = 0, GxGreen = 0, GxBlue = 0;
            int GyRed = 0, GyGreen = 0, GyBlue = 0;

            // Apply Sobel kernels
            for (int m = -1; m <= 1; m++)
            {
                for (int n = -1; n <= 1; n++)
                {
                    int x = i + m;
                    int y = j + n;

                    // Ensure the pixel is within bounds
                    if (x >= 0 && x < height && y >= 0 && y < width)
                    {
                        // Apply Gx and Gy kernels for each color channel
                        GxRed += image[x][y].rgbtRed * Gx[m + 1][n + 1];
                        GxGreen += image[x][y].rgbtGreen * Gx[m + 1][n + 1];
                        GxBlue += image[x][y].rgbtBlue * Gx[m + 1][n + 1];

                        GyRed += image[x][y].rgbtRed * Gy[m + 1][n + 1];
                        GyGreen += image[x][y].rgbtGreen * Gy[m + 1][n + 1];
                        GyBlue += image[x][y].rgbtBlue * Gy[m + 1][n + 1];
                    }
                }
            }

            // Calculate the gradient magnitudes
            int outr = (int) round(sqrt(GxRed * GxRed + GyRed * GyRed));
            int outg = (int) round(sqrt(GxGreen * GxGreen + GyGreen * GyGreen));
            int outb = (int) round(sqrt(GxBlue * GxBlue + GyBlue * GyBlue));

            // Cap the values at 255
            temp[i][j].rgbtRed = (outr > 255) ? 255 : outr;
            temp[i][j].rgbtGreen = (outg > 255) ? 255 : outg;
            temp[i][j].rgbtBlue = (outb > 255) ? 255 : outb;
        }
    }

    // Copy the new values from temp array back to the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }

    return;
}
