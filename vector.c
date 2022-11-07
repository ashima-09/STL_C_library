#include<stdio.h>
#include<stdlib.h>

//creating a vector node
struct vector{
    int *vector;
    long long int capacity;
    long long int size;
};

//function allocating the memory to the dynamic array at the initial stage
struct vector* vector_create(int size)
{
    //allocating memory equal to size of just 1 integer bit
    //after that realloc will be used to allocate memory
    struct vector *vec =(struct vector *)malloc(sizeof(struct vector));
    vec->vector=(int *)malloc(size*sizeof(int));
    vec->capacity=size;
    vec->size=size;
    return vec;
}

//pushes the integer value into the dynamic array
void vector_push_back(struct vector *vector,int n)
{
    //the concept behind this if block is:-
    //in a vector container whenever the size of the dynamic array equals to the total memory allocated to the array
    //it doubles its space itself
    //i.e. now the space of the array doubles and its ready again to store more elements
    //this is the actual working of c++ vector
    if(vector->size==vector->capacity)
    {
        vector->vector=(int *)realloc(vector->vector, 2*(vector->size)*sizeof(int));
        vector->capacity*=2;
    }
    vector->vector[vector->size]=n;
    vector->size++;
    return;
}

//it pops out the last element
//mainly it reduces the size of the vector array
void vector_pop_back(struct vector *vector)
{
    vector->size--;
}

//simply displays the dynamic array
void vector_display(const struct vector *vector)
{
    for(long long int i=0;i<vector->size;i++)
    {
        printf("%d ",vector->vector[i]);
    }
    printf("\n");
    return;
}

//this function takes in the value of an index and if the index is within the size of the the vector array
//it return the pointer at that index
//else if the index crosses the value of size of the vector, it returns NULL
int* vector_at(const struct vector *vector,long long int index)
{
    int *v;
    v=vector->vector;
    if(index>=vector->size)
    {
        return NULL;
    }
    // for(long long int i=0;i<index;i++)
    // {
    //     v++;
    // }
    return &v[index];
}

//find function returns the index value of the element to be searched
//basically this is the search function
long long int vector_find(const struct vector *vector,int n)
{
    int ind=-1;
    int *v;
    v=vector->vector;
    for(int i=0;i<vector->size;i++)
    {
        if(v[i]==n)
        {
            ind=i;
            break;
        }
    }
    //if the element is not present is not present it returns -1
    //else returns the value of index
    return ind;
}

//clear function basically equates size to 0
//hence if we ask for a value at an index greater than zero, it will return NULL
//since vector_at funcion is designed as such
void vector_clear(struct vector *vector)
{
    vector->size=0;
}

//main function
int main()
{
    struct vector *v;
    //calling create function just once to initialise the vector
    v=vector_create(1);

    //now calling push_back as per requirement
    vector_push_back(v, 2);
    vector_push_back(v, 4);
    vector_push_back(v, 6);
    vector_push_back(v, 8);
    vector_push_back(v, 10);

    vector_display(v);

    *vector_at(v, 2)=5;

    vector_display(v);

    //clears the entire vector
    vector_clear(v);
    
    vector_push_back(v, 1);
    vector_push_back(v, 11);
    vector_push_back(v, 11);
    vector_display(v);
    printf("%d\n",*vector_at(v, 2));
    return 0;
}