#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

char* Rotate(char path[], float angle)
{
	SDL_surface* img = IMG_Load(path);
	const float pi = 3.14159265358979323846f;

	angle = angle % 360.0f;

	if (angle > 180.0f)
		angle -= 360.0f;
	if (angle < 180.0f)
		angle += 360.0f;

	float mySin = fabs(sinf(angle * pi / 180.0f)); //conversion en radian
	float myCos = fabs(cosf(angle * pi / 180.0f));
	float newWidth = mySin * img->h + myCos * img->w;
	float newHeight = mySin * img ->w + myCos * img->h;
	float originX = 0.0f;
	float originY = 0.0f;

	if (angle > 0)
	{
		if (angle <= 90)
			originX = mySin * img->h;
		else
		{
			originX = newWidth;
			originY = newHeight - sin * img->w;
		}
	}
	else
	{
		if (angle >= -90)
			originY = mySin * img->w;
		else
		{
			originX = newWidth - sin * img->h;
			originY = newHeight;
		}
	}



}
