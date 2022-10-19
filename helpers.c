#include "helpers.h"
#include <math.h>
#include <cs50.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j =0; j< width; j++)
        {
            RGBTRIPLE pixel = image [i][j];
            int average = round((pixel.rgbtRed + pixel.rgbtGreen + pixel.rgbtBlue)/3.0);
            image[i][j].rgbtRed = image[i][j].rgbtGreen = image[i][j].rgbtBlue = average;
        }
    }
}

int cap(int value)
{
    return value >255 ? 255 : value;
}

// convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j =0; j< width; j++)
        {
            RGBTRIPLE pixel = image[i][j];
            int originalRed = pixel.rgbtRed;
            int originalGreen = pixel.rgbtGreen;
            int originalBlue = pixel.rgbtBlue;
            image[i][j].rgbtRed =cap(round( 0.393 * originalRed + 0.769 * originalGreen + 0.189 * originalBlue));
            image[i][j].rgbtGreen = cap(round(0.349 * originalRed + 0.686 * originalGreen + 0.168 * originalBlue));
            image[i][j].rgbtBlue = cap(round(0.272 * originalRed + 0.534 * originalGreen + 0.131 * originalBlue));

        }
    }

}

void swap(RGBTRIPLE * pixel1, RGBTRIPLE * pixel2)
{
    RGBTRIPLE tem =*pixel1;
    *pixel1 =*pixel2;
    *pixel2 =tem;
}


// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j =0; j< width / 2 ; j++)
        {
           swap(&image[i][j], &image[i][width - 1-j]);
        }
    }
}

bool is_valid_pixel(int i , int j, int height, int width)
{
    return i >= 0 && i < height && j >=0 && j < width;
}

RGBTRIPLE get_blurred_pixel (int i , int j, int height, int width, RGBTRIPLE image[height][width])
{
    int redvalue, greenvalue, bluevalue; redvalue=bluevalue=greenvalue=0;
    int numofvalidpixels =0;
    for (int di = -1; di<=1; di++)
    {
        for (int dj = -1; dj<=1; dj++)
        {
            int new_i = i+di;
            int new_j = j + dj;
            if (is_valid_pixel(new_i,  new_j, height, width ))
            {
                numofvalidpixels++;
               redvalue+=image[new_i][new_j].rgbtRed;
               greenvalue+=image[new_i][new_j].rgbtGreen;
               bluevalue+=image[new_i][new_j].rgbtBlue;
            }
        }
    }
    RGBTRIPLE blurred_pixel;
    blurred_pixel.rgbtRed = round((float) redvalue / numofvalidpixels);
    blurred_pixel.rgbtGreen = round((float) greenvalue / numofvalidpixels);
    blurred_pixel.rgbtBlue = round((float) bluevalue / numofvalidpixels);
    return blurred_pixel;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE new_image [height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j =0; j< width; j++)
        {
            new_image[i][j] = get_blurred_pixel(i, j, height, width, image);
        }
    }

for (int i = 0; i < height; i++)
 for (int j =0; j< width; j++)
 image [i][j] = new_image[i][j];
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

// loop through each row and each pixel of the image
for (int i = 0; i < height; i++)
{
    for (int j = 0; j < width; j++)
    {
        int redx = 0;
        int greenx = 0;
        int bluex = 0;
        int redy = 0;
        int greeny = 0;
        int bluey = 0;

        // loop through the pixels 1 row and 1 column from the pixel
        for (int k = 0; k < 3; k++)
        {
            for (int l = 0; l < 3; l++)
            {
                // consider pixels in edges
                if (i - 1 + k < 0 || i - 1 + k > height - 1 || j - 1 + l < 0 || j - 1 + l > width - 1)
                {
                    continue;
                }

                // calcuate Gx for each color
                redx = redx + (image[i - 1 + k][j - 1 + l].rgbtRed * Gx[k][l]);
                greenx = greenx + (image[i - 1 + k][j - 1 + l].rgbtGreen * Gx[k][l]);
                bluex = bluex + (image[i - 1 + k][j - 1 + l].rgbtBlue * Gx[k][l]);

                // calculate Gy for each color
                redy = redy + (image[i - 1 + k][j - 1 + l].rgbtRed * Gy[k][l]);
                greeny = greeny + (image[i - 1 + k][j - 1 + l].rgbtGreen * Gy[k][l]);
                bluey = bluey + (image[i - 1 + k][j - 1 + l].rgbtBlue * Gy[k][l]);
            }
        }
        // calculate square root of Gx2 + Gy2 for each color
        float r = sqrt((redx * redx) + (redy * redy));
        float g = sqrt((greenx * greenx) + (greeny * greeny));
        float b = sqrt((bluex * bluex) + (bluey * bluey));

        // round to nearest integer and cap at 255
        int red =  round(r);
        int green = round(g);
        int blue = round(b);

        if (red > 255)
            red = 255;
        if (green > 255)
            green = 255;
        if (blue > 255)
            blue = 255;

        // replace color values with new value
        image[i][j].rgbtRed = red;
        image[i][j].rgbtBlue = blue;
        image[i][j].rgbtGreen = green;
    }
}

}

