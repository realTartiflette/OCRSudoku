#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
#include "manipulatePixel.h"
#include "hough.h"

void DrawLine(SDL_Surface *img, int x0, int y0, int x1, int y1, float wd, Uint32 pixel_color)
{
	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = dx - dy, e2, x2, y2;
	float ed = dx + dy == 0 ? 1 : sqrt((float) dx * dx + (float) dy * dy);


	Uint32 pixel = pixel_color;

	for (wd = (wd + 1) / 2 ; ; )
	{
		if (x0 >= 0 && y0 >= 0 && x0 < img -> h && y0 < img -> w)
		{
			PutPixel(img, y0, x0, pixel);
		}

		e2 = err;
		x2 = x0;

		if (2 * e2 >= -dx)
		{
			for (e2 += dy, y2 = y0; e2 < ed * wd && (y1 != y2 || dx > dy); e2 += dx)
			{
				if (x0 >= 0 && x0 < img -> h && (y2 + sy) >= 0 && (y2 + sy) < img -> w)
				{
					PutPixel(img, (y2 += sy), x0, pixel);
				}
			}

			if (x0 == x1)
			{
				break;
			}

			e2 = err;
			err -= dy;
			x0 += sx;
		}

		if (2 * e2 <= dy)
		{
			for (e2 = dx - e2; e2 < ed * wd && (x1 != x2 || dx < dy); e2 += dy)
			{
				if ((x2 + sx >= 0 && x2 + sx < img -> h) && (y0 >= 0 && y0 < img -> w))
				{
					PutPixel(img, y0, x2 += sx, pixel);
				}
			}

			if (y0 == y1)
			{
				break;
			}

			err += dx;
			y0 += sy;
		}


	}
}

char* houghTransform(char* path)
{
    SDL_Surface* img = IMG_Load(path);
	char* name = "results/hough.jpg";

    /*int width = img->w;
    int height = img->h;

    Uint8 r;
    Uint8 g;
    Uint8 b;

    int d = sqrt((width*width) + (height*height));
    int min_d = -1 * d;
    int max_theta = 180;
    int min_theta = 0;
    int threshold = 800;

    double d_theta =  1;
//	double d_rho = (int) (2 * d + 1) / num_rhos;

	double thetas[181];
	double sin_thetas[181];
	double cos_thetas[181];
	double rhos[2 * d + 1];

	int step = 0;
	for (int i = 0; i < 181; i++)
	{
		thetas[i] = step;

		sin_thetas[i] = sin(thetas[i] * (M_PI / 180));
		cos_thetas[i] = cos(thetas[i] * (M_PI / 180));
		step += d_theta;
	}

	step = -d;


	for (int i = 0; i < 2 * d + 1; i++)
	{
		rhos[i] = step + i;
	}

    
    //int* accu = calloc(2*max_d*180, sizeof(int));
    int accu[2*d][180];
    for (int i = 0; i < 2*d; i++)
    {
        for (int j = 0; j < 180; j++)
        {
            accu[i][j] = 0;;
        }
    }

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            double edgeP1 = x - width/2;
            double edgeP2 = y - height/2;
			SDL_GetRGB(GetPixel(img, x, y), img->format, &r, &g, &b);
			if (r == 255)
            {
                for (int theta = min_theta; theta < max_theta; theta++)
                {
                    int rho = edgeP1*cos(theta)*(M_PI/2) + edgeP2*sin(theta)*(M_PI/2);
                    rho += d;
                    
                    accu[rho][theta]++;
                }
            }
        }
    }

    Uint32 color = SDL_MapRGB(img->format, 255, 0, 0);
    for (int i = 0; i < 2*d; i++)
    {
        for (int theta = 0; theta < 180; theta++)
        {
            //printf("acc = %i\n", accu[i][theta]);
            if (accu[i][theta] > threshold)
            {
                //float rho = i + min_d; 
                //float a = cos(theta*(M_PI/180));
                //float b = sin(theta*(M_PI/180));
                //float x0 = (a*rho) + width/2;
                //float y0 = (b*rho) + height/2;
                int rho = rhos[i];
				theta = thetas[theta];

				if (abs(theta - 45) < 10 || abs(theta - 135) < 20) // skip diagonal lines
					continue;

				if (abs(theta - 90) > 10 && abs(theta) > 10) // skewed line => skip 
					continue;

				float a = cos(theta*(M_PI/180));
                float b = sin(theta*(M_PI/180));
                float x0 = (a*rho) + width/2;
                float y0 = (b*rho) + height/2;
                int x1 = x0+1000*(-b);
                int y1 = y0+1000*a;
                int x2 = x0-1000*(-b);
                int y2 = y0-1000*a;


                DrawLine(img, y1, x1, y2, x2, 0.03*img->w, color);
                //int dx = x2 - x1;
                //int dy = y2 - y1;

                //printf("a = %f\n", a);
                //printf("b = %f\n", b);
                //printf("x0 = %i\n", x0);
                //printf("x1 = %i\n", x1);
                //printf("dx = %i\n", dx);
                //printf("dy = %i\n", dy);

                //for (int x = x1; x < x2; x++)
                //{
                //    int y = y1+dy*(x-x1)/dx;
                //    if (x < width && x > 0 && y < height && y > 0)
                //    {
                //        Uint32 averagePixel = SDL_MapRGB(img->format, 255, 0, 0);
			    //        PutPixel(img, x, y, averagePixel);
                //    }
                 //   
                //}
            }
            
        }
    }*/
    char* line_color = "red";
    /*if (num_rhos == 0 || num_thetas == 0)
	{
		errx(1, "Errror Hough : incorrect parameters\n");
		return ;
	}*/
    int threshold = 600;

	int d = (int) (sqrt((img -> h) * (img -> h) + (img -> w) * (img -> w)));

	double d_theta =  20;
//	double d_rho = (int) (2 * d + 1) / num_rhos;

	int thetas[181];
	double sin_thetas[181];
	double cos_thetas[181];
	double rhos[2 * d + 1];

	int step = 0;
	for (int i = 0; i < 181; i++)
	{
		thetas[i] = step;

		sin_thetas[i] = sin(thetas[i] * (M_PI / 180));
		cos_thetas[i] = cos(thetas[i] * (M_PI / 180));
		step += d_theta;
	}

	step = -d;
	for (int i = 0; i < 181; i++)
	{
		printf("theta[%i] = %f\n",i,thetas[i]);
	}


	for (int i = 0; i < 2 * d + 1; i++)
	{
		rhos[i] = step + i;
	}

	//printf("%f    %f\n", rhos[0], rhos[2 * d]);

	// init accumulator
	int accumulator[2 * d + 1][181];

	for (int i = 0; i < 2 * d + 1; i++)
	{
		for (int j = 0; j < 181; j++)
		{
			accumulator[i][j] = 0;
		}
	}


	// begin hough
	
	Uint32 pixel;
	Uint8 r, g, _b;

	int theta;
	double rho;
	
	int half_w = img -> w / 2;
	int half_h = img -> h / 2;

	int rho_index = 0;


	for (int y = 0; y < img -> h; y++)
	{
		for (int x = 0; x < img -> w; x++)
		{
			pixel = GetPixel(img, x, y);
			SDL_GetRGB(pixel, img -> format, &r, &g, &_b);

			double edge_point_1 = x - half_w;
			double edge_point_2 = y - half_h;

			if (r == 255 && g == 255 && _b == 255) // white pixel => edge pixel
			{
				for (int k = 0; k < 181; k += d_theta)
				{
					rho = (edge_point_1 * cos_thetas[k]) + (edge_point_2 * sin_thetas[k]);

					theta = thetas[k];

					double min_rho_abs = DBL_MAX;

					for (int l = 0; l <  2 * d + 1; l++)
					{
						// get rho index 
						if (fabs(rho - rhos[l]) < min_rho_abs)
						{
							rho_index = l;
							min_rho_abs = fabs(rho - rhos[l]);
						}
						if (rhos[l] > rho) // reduce hough run time: rhos array in increasing order => break if current val > rho
							break;
					}

					accumulator[rho_index][theta]++;
				}
			}
		}
	}

	double a, b, x0, y0;

	double x1, y1, x2, y2;

	for (int y = 0; y < 2 * d + 1; y++)
	{
		for (int x = 0; x < 181; x++)
		{
			if (accumulator[y][x] > threshold)
			{
				rho = rhos[y];
				theta = thetas[x];
				//printf("theta x[%i] = %f\n", x, theta);

				/*if (abs(theta - 45) < 10 || abs(theta - 135) < 20) // skip diagonal lines
					continue;

				if (abs(theta - 90) > 10 && abs(theta) > 10) // skewed line => skip 
					continue;*/

				a = cos(theta * (M_PI / 180));
				b = sin(theta * (M_PI / 180));
				
				x0 = (a * rho) + half_w;
				y0 = (b * rho) + half_h;

				x1 = (int) (x0 + 1000 * (-b));
				y1 = (int) (y0 + 1000 * (a));
				x2 = (int) (x0 - 1000 * (-b));
				y2 = (int) (y0 - 1000 * (a));
			
				DrawLine(img, x1, y1, x2, y2, 0.03*img->w,SDL_MapRGB(img->format, 255, 0, 0));
				//DrawLine(img, x2, y2, x1, y2, 0.03*img->w,SDL_MapRGB(img->format, 255, 0, 0));
			}
		}
	}
    IMG_SaveJPG(img, name, 100);
	return name;

}
