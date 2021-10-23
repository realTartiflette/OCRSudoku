#ifndef EDGE_DECTECTOR_H
#define EDGE_DECTECTOR_H

#define radius 5

typedef struct point point;
struct point
{
    int x;
    int y;
};

char* detectLine(char* path);

#endif