#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include "initNetwork.h"
#include "../manipulateImage/manipulatePixel.h"

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
        h = 100;
        w = 100;
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
    neuralNetwork *nn = createNeuralNetwork(10000, nbOfHiddenLayers, nbOfNeuronsByLayers, 10);
    
    size_t nbOfInputs = getNbOfInputs(data_set_path);
    *inputs = matAlloc(nbOfInputs, 10000);
    *expectedResults = matAlloc(nbOfInputs, 10);
    setMatrices(data_set_path, *inputs, *expectedResults);
    return nn;
}
