#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include "initNetwork.h"
#include "../manipulateImage/manipulatePixel.h"

SDL_Surface* Scale(SDL_Surface* img, int newImgWidth, int newImgHeight)
{
    int scaleW = img -> w / newImgWidth;
    int scaleH = img -> h / newImgHeight;
    SDL_Surface* newIMG = SDL_CreateRGBSurface(0, newImgWidth, newImgHeight, 32, 0, 0, 0 ,0);
    
    for (int w = 0; w < newImgWidth; w++)
    {
        for (int h = 0; h < newImgHeight; h++)
        {
            Uint32 pixel = GetPixel(img, w*scaleW, h*scaleH);
            PutPixel(newIMG, w, h, pixel);
        }
    }
    return newIMG;
}

size_t getFilesNumber(char *path)
{
    size_t nbOfFiles = 0;
    DIR *dirp;
    struct dirent *entry;

    dirp = opendir(path);
    while ((entry = readdir(dirp)) != NULL)
    {
        if(entry->d_type == DT_REG)
        {
            nbOfFiles++;
        }
    }
    closedir(dirp);
    return nbOfFiles;
}

size_t getNbOfInputs(char *path)
{
    size_t nbOfInputs = 0;
    for (size_t i = 1; i < 10; i++)
    {
        char tmp[100];
        strcpy(tmp, path);

        char nb[5];
        sprintf(nb, "%ld", i);

        strcat(tmp, nb);
        nbOfInputs += getFilesNumber(tmp);
    }

    char tmp[100];
    strcpy(tmp, path);

    char *none = "none";
    
    strcat(tmp, none);
    nbOfInputs += getFilesNumber(tmp);

    return nbOfInputs;
}

void convertImage(char *path, matrix *inputs, size_t curRow)
{
    SDL_Surface *img = IMG_Load(path);
    int h = img->h, w = img->w;
    if(img->h * img->w > inputs->cols)
    {
        //resize image
        SDL_Surface *nimg = Scale(img, 49, 49);
        h = nimg->h, w = nimg->w;
        SDL_FreeSurface(img);
        img = nimg;
    }
    Uint8 r;
	Uint8 g;
	Uint8 b;

    size_t k = 0;
    for (size_t i = 0; i < w; i++)
    {
        for (size_t j = 0; j < h; j++)
        {
            Uint32 tmpPix = GetPixel(img, i, j);
            SDL_GetRGB(tmpPix, img->format, &r, &g, &b);
            inputs->mat[curRow * inputs->cols + k] = r/255;
            k++;
        }
        
    }
    SDL_FreeSurface(img);
    
}

void setMatrices(char *data_set_path, matrix *inputs, matrix *expectedResults)
{
    size_t curRow = 0;
    for (size_t i = 1; i < 10; i++)
    {
        char path[100];
        strcpy(path, data_set_path);

        char nb[5];
        sprintf(nb, "%ld", i);

        strcat(path, nb);

        DIR *dirp;
        struct dirent *entry;

        dirp = opendir(path);
        while ((entry = readdir(dirp)) != NULL)
        {
            if(entry->d_type == DT_REG)
            {
                char tmp[100];
                strcpy(tmp, path);
                strcat(tmp, "/");
                strcat(tmp, entry->d_name);
                
                convertImage(tmp, inputs, curRow);

                expectedResults->mat[curRow * expectedResults->cols + i] = 1;
                curRow++;
            }
        }
        closedir(dirp);
    }

    char path[100];
    strcpy(path, data_set_path);

    char *none = "none";
    strcat(path, none);
    DIR *dirp;
    struct dirent *entry;

    dirp = opendir(path);
    while ((entry = readdir(dirp)) != NULL)
    {
        if(entry->d_type == DT_REG)
        {
            char tmp[100];
            strcpy(tmp, path);
            strcat(tmp, "/");
            strcat(tmp, entry->d_name);
            convertImage(tmp, inputs, curRow);

            expectedResults->mat[curRow * expectedResults->cols + 0] = 1;
            curRow++;
        }
    }
    closedir(dirp);
    
}

neuralNetwork *initNetwork(char *data_set_path, size_t nbOfHiddenLayers, size_t nbOfNeuronsByLayers[MAX_LAYER], matrix **inputs, matrix **expectedResults)
{
    neuralNetwork *nn = createNeuralNetwork(2500, nbOfHiddenLayers, nbOfNeuronsByLayers, 10);
    
    size_t nbOfInputs = getNbOfInputs(data_set_path);
    *inputs = matAlloc(nbOfInputs, 2500);
    *expectedResults = matAlloc(nbOfInputs, 10);
    setMatrices(data_set_path, *inputs, *expectedResults);
    return nn;
}

void convertImageToMat(char *path, matrix *dest)
{
    convertImage(path, dest, 0);
}

int getPrediction(matrix* mat)
{
    int pred = 0;
    float prob = mat->mat[0];
    for (size_t i = 1; i < mat->cols; i++)
    {
        if(mat->mat[i] > prob)
        {
            pred = i;
            prob = mat->mat[i];
        }
    }
    return pred;
}