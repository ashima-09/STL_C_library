#include<stdio.h>
#include<stdlib.h>
#include<climits>

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

//this algo merges the two divided algos
void merge(int *v,int l,int mid, int r)
{
    int l_size=mid-l+1;
    int r_size=r-mid;

    int l_arr[l_size+1];
    int r_arr[r_size+1];

    for(int i=0; i<l_size; i++)
    l_arr[i]=v[i+l];
    for(int i=0; i<r_size; i++)
    r_arr[i]=v[mid+1+i];

    //setting the value of these two last arrays as INT_MAX so that any value compared to them
    //is always less than it
    //this step omitts the use of while loop and then shifting the pointer to compare the values
    l_arr[l_size]=INT_MAX;
    r_arr[r_size]=INT_MAX;

    int l_i=0,r_i=0;

    for(int i=l; i<=r; i++)
    {
        if(l_arr[l_i]<=r_arr[r_i])
        {
            v[i]=l_arr[l_i];
            l_i++;
        }
        else
        {
            v[i]=r_arr[r_i];
            r_i++;
        }
    }

}

//function recursively called
void mergesort(int *vector, int l, int r)
{
    if(l>=r)
    return;
    int mid=(l+r)/2;
    mergesort(vector,l,mid);
    mergesort(vector,mid+1,r);
    merge(vector,l,mid,r);
}

void vector_sort(struct vector*vector)
{
    mergesort(vector->vector, 0, vector->size-1);
}

//main function
int main()
{
    struct vector *v;
    //calling create function just once to initialise the vector
    v=vector_create(1);

    //now calling push_back as per requirement
    vector_push_back(v, 20);
    vector_push_back(v, 4);
    vector_push_back(v, 6);
    vector_push_back(v, 8);
    vector_push_back(v, 10);

    vector_display(v);

    *vector_at(v, 2)=5;

    //sorts the entire array using merge sort
    vector_sort(v);

    vector_display(v);

    //clears the entire vector
    vector_clear(v);
    return 0;
}