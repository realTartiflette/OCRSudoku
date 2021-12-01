#ifndef VECTOR_H
#define VECTOR_H

typedef struct sVectorList
{
    void **items;
    int capacity;
    int total;
} sVectorList;
//structure contain the function pointer
typedef struct sVector vector;
struct sVector
{
    sVectorList vectorList;
//function pointers
    int (*pfVectorTotal)(vector *);
    int (*pfVectorResize)(vector *, int);
    int (*pfVectorAdd)(vector *, void *);
    int (*pfVectorSet)(vector *, int, void *);
    void *(*pfVectorGet)(vector *, int);
    int (*pfVectorDelete)(vector *, int);
    int (*pfVectorFree)(vector *);
};

void vector_init(vector *v);
int vectorFree(vector *v);
int vectorDelete(vector *v, int index);
void *vectorGet(vector *v, int index);
int vectorSet(vector *v, int index, void *item);
int vectorPushBack(vector *v, void *item);
int vectorResize(vector *v, int capacity);
int vectorTotal(vector *v);


#endif
