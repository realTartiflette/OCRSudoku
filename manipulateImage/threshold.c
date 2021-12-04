#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_stdinc.h>
#include <stdio.h> 
#include <stdlib.h>
#include "math.h"
#include "manipulatePixel.h"

#define MAX(X, Y)  ((X) > (Y) ? (X) : (Y))


SDL_Surface *Threshold(SDL_Surface *img, int radius)
{
	SDL_Surface* newIMG = SDL_CreateRGBSurface(0, img->w, img->h, 32, 0, 0, 0 ,0);
	char* name = "results/thresholdIMG.jpg";

	int tier1 = 0;
	int tier2 = 0;
	int tier3 = 0;
	int tier4 = 0;
	int tier5 = 0;

	Uint8 r;
	Uint8 g;
	Uint8 b;

	int sum;
	Uint32 newPixel;

	for (int x = 0; x < img->w; x++)
	{
		
		for (int y = 0; y < img->h; y++)
		{
			Uint32 tmpPix = GetPixel(img, x, y);
			SDL_GetRGB(tmpPix, img->format, &r, &g, &b);
			sum = (r+g+b)/3;
			if (sum < 52)
				tier1++;
			else if (sum < 103)
				tier2++;
			else if (sum < 154)
				tier3++;
			else if (sum < 205)
				tier4++;
			else
				tier5++;
		}
	}


	float nbPixel = (float)(tier1+tier2+tier3+tier4+tier5);
	int ref;

	if (((float)tier1/nbPixel) > 0.06)
		ref = 80;
	else if (((float)tier2/nbPixel) > 0.06)
		ref = 110; 
	else if (((float)tier3/nbPixel) > 0.06)
		ref = 135;
	else if (((float)tier4/nbPixel) > 0.06)
		ref = 180;
	else 
		ref = 210;
	
	for (int x = 0; x < img->w; x++)
	{
		
		for (int y = 0; y < img->h; y++)
		{
			Uint32 tmpPix = GetPixel(img, x, y);
			SDL_GetRGB(tmpPix, img->format, &r, &g, &b);
			sum = (r+g+b)/3;
			if (sum < ref)
				newPixel = SDL_MapRGB(img->format, 255, 255, 255);
			else
				newPixel = SDL_MapRGB(img->format, 0, 0, 0);

			PutPixel(newIMG, x, y, newPixel);
		}
	}
	

	IMG_SaveJPG(newIMG, name, 100);
	return newIMG;
}


/*SDL_Surface* Threshold(SDL_Surface *img)
{
    SDL_Surface* newIMG = SDL_CreateRGBSurface(0, img->w, img->h, 32, 0, 0, 0 ,0);
	char* name = "results/thresholdIMG.jpg";
    Uint32 newPixel;

	Uint8 r, g, b;

	// Prepare variables
	float histogram[256] = {0.0F};
	int ref = 0;
    int sum;
	
	// Prepare histogram
	for (int y = 0; y < img->h; y++)
	{
		for (int x = 0; x < img->w; x++)
		{
            Uint32 tmpPix = GetPixel(img, x, y);
			SDL_GetRGB(tmpPix, img->format, &r, &g, &b);
			sum = (r+g+b)/3;
			histogram[sum]++;
		}
	}

	// Otsu's algorithm (https://en.wikipedia.org/wiki/Otsu%27s_method)
	for(int i = 0; i < 256; i++)
	{
		histogram[i] /= (img->w * img->h);
	}
	
	float ut = 0;
	for (int i = 0; i < 256; i++)
	{
		ut += i * histogram[i];
	}

	int max_k = 0;
	float max_sigma_k = 0;
	for(int k = 0; k < 256; k++)
	{
		float wk = 0;
		for(int i = 0; i <= k; i++)
		{
			wk += histogram[i];
		}
		float uk = 0;
		for(int i = 0; i <= k; i++)
		{
			uk += i * histogram[i];
		}

		float sigma_k = 0;
		if(wk != 0 && wk != 1)
		{
			sigma_k = ((ut * wk - uk) * (ut * wk - uk)) / (wk * (1 - wk));
		}
		if(sigma_k > max_sigma_k)
		{
			max_k = k;
			max_sigma_k = sigma_k;
		}
	}
	ref = max_k;

	for (int x = 0; x < img->w; x++)
	{
        for (int y = 0; y < img->h; y++)
        {
            Uint32 tmpPix = GetPixel(img, x, y);
            SDL_GetRGB(tmpPix, img->format, &r, &g, &b);
            sum = (r+g+b)/3;
            if (sum < ref)
                newPixel = SDL_MapRGB(img->format, 255, 255, 255);
            else
                newPixel = SDL_MapRGB(img->format, 0, 0, 0);

            PutPixel(newIMG, x, y, newPixel);
        }
    }
	IMG_SaveJPG(newIMG, name, 100);
	return newIMG;
}*/


/*int mean(unsigned int *histo, int start, int end) {
  return histo[end - start];
}

int sum(unsigned int *histo, int start, int end) {
  int sum = 0;
  for (int i = start; i < end; i++)
    sum += (int) histo[i];
  return sum;
}

int otsu(unsigned int histo[256], int w, int h) 
{
  double final_thresh = -1.0;
  int final_t = -1;
  double mean_weight = 1.0 / (w * h);
  for (int t = 1; t < 255; t++) {
    double wb = (double) sum(histo, 0, t) * mean_weight;
    double wf = (double) sum(histo, t, 255) * mean_weight;

    int mub = mean(histo, 0, t);
    int muf = mean(histo, t, 255);

    double value = wb * wf * (mub - muf);
    value *= value;
    if (value > final_thresh) {
      final_thresh = value;
      final_t = t;
    }
  }
  return final_t;
}

SDL_Surface* Threshold(SDL_Surface *img)
{
    SDL_Surface* newIMG = SDL_CreateRGBSurface(0, img->w, img->h, 32, 0, 0, 0 ,0);
	char* name = "results/thresholdIMG.jpg";

	// Prepare variables
	unsigned int histogram[256] = {0};
	int ref = 0;
    unsigned int sum;

	Uint8 r, g, b;
	Uint32 newPixel;
	
	// Prepare histogram
	for (int y = 0; y < img->h; y++)
	{
		for (int x = 0; x < img->w; x++)
		{
            Uint32 tmpPix = GetPixel(img, x, y);
			SDL_GetRGB(tmpPix, img->format, &r, &g, &b);
			sum = (r+g+b)/3;
			histogram[sum]++;
		}
	}

	ref = otsu(histogram, img->w, img->h);

	for (int x = 0; x < img->w; x++)
	{
        for (int y = 0; y < img->h; y++)
        {
            Uint32 tmpPix = GetPixel(img, x, y);
            SDL_GetRGB(tmpPix, img->format, &r, &g, &b);
            sum = (r+g+b)/3;
            if (sum < ref)
                newPixel = SDL_MapRGB(img->format, 255, 255, 255);
            else
                newPixel = SDL_MapRGB(img->format, 0, 0, 0);

            PutPixel(newIMG, x, y, newPixel);
        }
    }
	IMG_SaveJPG(newIMG, name, 100);
	return newIMG;
}*/

/*int mean(SDL_Surface* img, int width, int height)
{
	int ref = 0;
	int sum = 0;
	Uint8 r, g, b;
	int nbPixels = width * height;

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			Uint32 tmpPix = GetPixel(img, x, y);
            SDL_GetRGB(tmpPix, img->format, &r, &g, &b);
            sum = (r+g+b)/3;
			ref += sum; 
		}
	}
	ref /= nbPixels;
	return ref;
}
SDL_Surface* Threshold(SDL_Surface *img)
{
	
    SDL_Surface* newIMG = SDL_CreateRGBSurface(0, img->w, img->h, 32, 0, 0, 0 ,0);
	char* name = "results/thresholdIMG.jpg";
    Uint32 newPixel;
	int sum = 0;
	int initial_ref = 0;
	int ref = mean(img, img->w, img->h);
	int limit = initial_ref;
	Uint8 r, g, b;
	int less = 0;
	int more = 0;
	int cptless = 0;
	int cptmore = 0;

	while (abs(initial_ref - ref) < limit)
	{
		initial_ref = ref;
		for (int x = 0; x < img->w; x++)
		{
			for (int y = 0; y < img->h; y++)
			{
				Uint32 tmpPix = GetPixel(img, x, y);
				SDL_GetRGB(tmpPix, img->format, &r, &g, &b);
				sum = (r+g+b)/3;
				if (sum < initial_ref)
				{
					less += sum;
					cptless++;
				} 
				else
				{
					more += sum;
					cptmore++;
				}
			}
		}

		less /= cptless;
		more /= cptmore;

		int ref = (more + less)/2;
	}

	for (int x = 0; x < img->w; x++)
	{
        for (int y = 0; y < img->h; y++)
        {
            Uint32 tmpPix = GetPixel(img, x, y);
            SDL_GetRGB(tmpPix, img->format, &r, &g, &b);
            sum = (r+g+b)/3;
            if (sum < ref)
                newPixel = SDL_MapRGB(img->format, 255, 255, 255);
            else
                newPixel = SDL_MapRGB(img->format, 0, 0, 0);

            PutPixel(newIMG, x, y, newPixel);
        }
    }
	IMG_SaveJPG(newIMG, name, 100);
	return newIMG;

}*/
