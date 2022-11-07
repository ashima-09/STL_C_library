#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


typedef struct node
{
  int data;
  struct node *r, *l, *p;
  int c; // 0 for red 1 for black
} node;

typedef struct set
{
  node *root;
  node *Null;
  int size;
} * set;

//-------------------------------------------------------------------------------------------------
//Functions defined

node * new_node(int key);
set create();
void LEFT_ROTATE(set t, node *x);
void RIGHT_ROTATE(set t, node *x);
void insertion_fixup(set t, node *x);
void insert(set t, int n);
void rb_transplant(set t, node *a, node *b);
node * Successor(set t, node *x);
void swap_c(node *x, node *y);
void delete_fixup(set t, node *x);
struct node *find(set t, int data);
void erase(set t, int key);
int size(set t);
node* begin(set t);
node* end(set t);
void display(set t, node *n);

//-------------------------------------------------------------------------------------------------
 
node *new_node(int key) // Creates a new node
{
  node *n = (node *)malloc(sizeof(node));
  n->c = 0;      // colour the new node red
  n->data = key; // set the key into node's data
  n->l = NULL;
  n->r = NULL;
  n->p = NULL;

  return n;
}


set create() // Creates the set (sets the tree->null to null)
{
  set t = malloc(sizeof(set));

  node *null = malloc(sizeof(node));
  null->c = 1; // set the colour of null to black (since the null is taken to be black for rbtree)
  null->l = NULL;
  null->r = NULL;
  null->p = NULL;

  t->Null = null;
  t->root = t->Null;

  return t;
}


void LEFT_ROTATE(set t, node *x)//Performs left rotation
{
  node *ptr = x->r;

  if (x->r->l != t->Null)
  { // if x->r->l is not null, then set its parent to x
    x->r->l->p = x;
  }
  // Changes the child of x->p to x->r
  if (x->p == t->Null)
  { // x is root
    t->root = x->r;
  }
  else if (x == x->p->l)
  { // makes x->r the left child of its x->p
    x->p->l = x->r;
  }
  else
  { // makes x->r the right child of x->p
    x->p->r = x->r;
  }
  x->r->p = x->p; // changes the parent of x->r from x to x->p

  x->p = x->r;
  x->r = x->r->l;

  ptr->l = x;
}


void RIGHT_ROTATE(set t, node *x)// Perform right rotation
{
  node *ptr = x->l;

  if (x->l->r != t->Null)
  { // if x->l->r is not null, then set its parent to x
    x->l->r->p = x;
  }
  // Changes the child of x->p to x->l
  if (x->p == t->Null)
  { // x is root
    t->root = x->l;
  }
  else if (x == x->p->r)
  { // makes x->l the right child of its x->p
    x->p->r = x->l;
  }
  else
  { // makes x->r the left child of x->p
    x->p->l = x->l;
  }
  x->l->p = x->p; // changes the parent of x->r from x to x->p

  x->p = x->l;
  x->l = x->l->r;

  ptr->r = x;
}


void insertion_fixup(set t, node *x) // balances the tree
{

  // while the colour of parent is red
  while (x->p->c == 0)
  {
    // if uncle is red, performs recolouring
    if (x->p->p->l->c == x->p->p->r->c)
    {
      x->p->p->c = 0;
      x->p->p->l->c = x->p->p->r->c = 1;
    }
    // if uncle is black
    else
    {
      if (x->p == x->p->p->l)
      { // x->p is the left child

        // if x is the right child
        if (x == x->p->r)
        { // perform left rotation with x->p as the root
          x = x->p;
          LEFT_ROTATE(t, x);
        }

        x->p->c = 1;              // change the color of x->p to black
        x->p->p->c = 0;           // change the colour of x grandparent to red
        RIGHT_ROTATE(t, x->p->p); // perform right rotation with x grandparent as root
      }
      else
      { // x->p is the right child

        // if x is the left child
        if (x == x->p->l)
        {
          x = x->p; // perform right rotation with x->p as the root
          RIGHT_ROTATE(t, x);
        }
        x->p->c = 1;             // change the color of x->p to black
        x->p->p->c = 0;          // change the colour of x grandparent to red
        LEFT_ROTATE(t, x->p->p); // perform left rotation with x grandparent as root
      }
    }
  }

  // in case the root got colored red in the process then turn it black
  t->root->c = 1; // make the root black

  return;
}

void insert(set t, int n) // insert a new node
{

  node *z = new_node(n); // creates a new node with node->data as n

  node *y = t->Null; // variable for the p of the added node
  node *ptr = t->root;

  while (ptr != t->Null)
  {
    y = ptr;
    if (z->data < ptr->data)
      ptr = ptr->l;
    else
      ptr = ptr->r;
  }
  z->p = y;

  if (y == t->Null)
  { // newly added node is root
    t->root = z;
  }
  else if (z->data == y->data)
    return;
  else if (z->data < y->data) // data of child is less than its p, l child
    y->l = z;
  else
    y->r = z;

  z->r = t->Null;
  z->l = t->Null;

  t->size++; // increment the size of the rb tree

  insertion_fixup(t, z);
}

// Replace the pointer to a with with pointer to b
void rb_transplant(set t, node *a, node *b)
{

  if (a->p == t->Null) // if a is the root
    t->root = b;
  else if (a == a->p->r) // if a is the right child
    a->p->r = b;
  else // if b is the left child
    a->p->l = b;

  b->p = a->p; // make the parent of b the parent of a
}


// Gives the* Successor to a node x
node * Successor(set t, node *x)
{
  while (x->l != t->Null)
    x = x->l;
  return x;
}


void swap_c(node *x, node *y) //swaps colors of two nodes
{
  int k = x->c;
  x->c = y->c;
  y->c = k;
}


void delete_fixup(set t, node *x) // balances the tree after deletion
{
  while (x != t->root && x->c == 1)
  { // if root is not double black and x is double black

    if (x == x->p->l)
    {
      node *sibling = x->p->r;
      if (sibling->c == 0)
      {                        // Case when sibling colour is red
        swap_c(sibling, x->p); // swap the color of sibling and parent
        LEFT_ROTATE(t, x->p);  // Left rotate with root as parent
        sibling = x->p->r;
      }

      // case when both child of sibling are black...
      // this is also the continuing case of above case i.e when parent is red and both the child are black
      if (sibling->l->c == 1 && sibling->r->c == 1)
      {
        sibling->c = 0; // color the sibling red
        x = x->p;
      }

      else
      {

        // case when nearer child of sibling is red
        if (sibling->r->c == 1)
        {
          sibling->l->c = 1;
          sibling->c = 0;
          RIGHT_ROTATE(t, sibling);
          sibling = x->p->r;
        }

        // also the continiuing case of above case , when farther child of sibling is red
        sibling->r->c = 1;
        LEFT_ROTATE(t, x->p);

        // complete and covers the case when x->p is red and sibling and both its children are black
        sibling->c = x->p->c;
        x->p->c = 1;

        x = t->root; //  breaks the loop and to color the root black
      }
    }

    // mirror image of above cases
    else
    {
      node *sibling = x->p->l;

      if (sibling->c == 0)
      {
        swap_c(sibling, x->p); // swap the color of sibling and parent
        RIGHT_ROTATE(t, x->p);
        sibling = x->p->l;
      }
      if (sibling->r->c == 1 && sibling->l->c == 1)
      {
        sibling->c = 0;
        x = sibling->p;
      }
      else
      {
        if (sibling->l->c == 1)
        {
          sibling->r->c = 1;
          sibling->c = 0;
          LEFT_ROTATE(t, sibling);
          sibling = x->p->l;
        }
        sibling->c = x->p->c;
        x->p->c = 1;
        sibling->l->c = 1;
        RIGHT_ROTATE(t, x->p);
        x = t->root;
      }
    }
  }
  x->c = 1; // color the root or the node black
}


struct node *find(set t, int data) // finds the node with given data
{
  struct node *ptr = t->root;
  while (ptr != t->Null)
  {
    if (ptr->data == data)
      return ptr;
    else if (ptr->data < data)
      ptr = ptr->r;
    else
      ptr = ptr->l;
  }
  if(ptr==t->Null)
  {
    printf("not found\n");
  }
  return ptr;
}


void erase(set t, int key) // deletes a node
{
  node *z= find(t,key);
  node *y = z;
  node *x;
  int y_orignal_c = y->c; // y's color is the color of the node to be deleted

  if (z->l == t->Null)
  {
    x = z->r;
    rb_transplant(t, z, z->r); // replaces z with its right child
  }
  else if (z->r == t->Null)
  {
    x = z->l;
    rb_transplant(t, z, z->l); // replaces z with its left child
  }

  else
  {
    // Removes original y from the tree and replace z with y

    y = Successor(t, z->r); // y -- "Sucessor" to the node to be deleted
    y_orignal_c = y->c;       // y's color -- "color of Sucessor" of the node to be deleted

    x = y->r;
    if (y->p == z)
    { // if y is the child of z, then replace the parent of right child of y with z
      x->p = z;
    }

    else
    {
      rb_transplant(t, y, y->r); // removes y from tree (no changes are done with left child since it is null)
      y->r = z->r;               // shift the right child of z to y
      y->r->p = y;
    }

    // replaces the node z with y
    rb_transplant(t, z, y);
    y->l = z->l;
    y->l->p = y;
    y->c = z->c;
  }

  // if color was black then call delete_fixup
  if (y_orignal_c == 1)
    delete_fixup(t, x);
  t->size--;
}

// Returns the size of the tree
int size(set t)
{
  return t->size;
}

//Gives the node corresponding to minimum value
node* begin(set t)
{
  node *x= Successor(t,t->root);
  return x;
}

//Gives the node corresponding to maximum node
node* end(set t)
{
  node* x=t->root;
   while(x->r!=t->Null)
   x=x->r;
   return x;
}

//Prints the entire tree in ascending order(inorder traversal)
void display(set t, node *n)
{
  if (n != t->Null)
  {
    display(t, n->l);
    printf("%d ", n->data);
    display(t, n->r);
  }
}

//-------------------------------------------------------------------------------------------------


struct pair //structure containing key and value.
{
    char *key;
    char *value;
};

struct bucket//structure containing a unsigned int count a pointer to a dynamic array of struct pair
{
    unsigned int count;
    struct pair *pairs;
};

struct map//structure containing a unsigned int count a pointer to a dynamic array of struct bucket
{
    unsigned int count;
    struct bucket *buckets;
};

//Prototypes of functions used in implementing map like structure.
struct map * map_new(int size);
int map_insert(struct map *map,const char *key,const char *value);
char * map_at(const struct map *map,const char*key);
void map_erase(const struct map*map,char *key,char *value);
unsigned int map_size(const struct map *map);
void map_display(const struct map *map);
static unsigned long hash(const char *str);
static struct pair *get_pair(struct bucket *bucket,const char *key);
//---------------------------------------------------------------------------

struct map * map_new(int size)//function to create new map.
{
    struct map *map;
    map=(struct map*)malloc(sizeof(struct map));//allocating memory to store struct map.
    if(map==NULL)//if memory allocation fails then return NULL.
    {
        return NULL;
    }
    map->count=size;//updating size of count of map.
    map->buckets=(struct bucket*)malloc(map->count * sizeof(struct bucket));//allocating memory to store struct bucket.
    if(map->buckets==NULL)//if memory allocation fails then return NULL.
    {
        free(map);
        return NULL;
    }
    memset(map->buckets, 0, map->count * sizeof(struct bucket));//initializing all buckets with a default value.
    return map;//returning pointer this map.
}


int map_insert(struct map *map,const char *key,const char *value)
{
    unsigned int index;//declaring an unsigned int index.
    struct bucket *bucket;//declaring an pointer to struct bucket.
    struct pair *pair,*temp;//declaring pointers to struct pair.
    char *temp_val;
    char *new_key,*new_value;
    if(map==NULL)//if map is poiting to NULL then return 0.
    {
        return 0;
    }
    if(key==NULL||value==NULL)//if key/value is pointing to NULL then return 0.
    {
        return 0;
    }
    index=hash(key)%map->count;//finding index using hashing finction.(taking remider of dividing value of function hash(function that maps every string to a unique unsigned int) by map->count).
    bucket=&(map->buckets[index]);//intializing buckets to map->bucket at index that we got from hashing function.
    pair=get_pair(bucket, key);//initializing pair to return value of function get_pair(function returns the pointer to struct pair which contains element same as key if no such pair is present then the function returns NULL).
    if(pair!=NULL)//if the key value is alreading present in the map then overwriting the old value with new value.
    {
        if(strlen(pair->value)<strlen(value))//if length of new value is larger than the old value then reallocting the size of old value and copying new value in it.
        {
            temp_val=(char *)realloc(pair->value, (strlen(value)+1)*sizeof(char));//reallocting the size of old value
            if(temp_val==NULL)//if memory allocation fails then return 0.
            {
                return 0;
            }
            pair->value=temp_val;
        }
        strcpy(pair->value, value);//copying new value in old value.
        return 1;
    }
    //if key is not present in map then allocating memory for new key and then checking whether the bucket is empty or not and then allocating memory for storing key and value and value and copying key and value to their respective positions(in the last).
    new_key=(char *)malloc((strlen(key)+1)*sizeof(char));
    if(new_key==NULL)
    {
        return 0;
    }
    new_value=(char *)malloc((strlen(value)+1)*sizeof(char));
    if(new_value==NULL)
    {
        free(new_key);
        return 0;
    }
    if(bucket->count==0) //checking whether the bucket is empty or not
    {
        bucket->pairs=(struct pair*)malloc(sizeof(struct pair));//allocating memory for storing key and value
        if(bucket->pairs==NULL)
        {
            free(new_key);
            free(new_value);
            return 0;
        }
        bucket->count=1;
    }
    else
    {
        temp=(struct pair*)realloc(bucket->pairs, (bucket->count+1) *sizeof(struct pair));//reallocating memory for storing key and value.
        if(temp==NULL)
        {
            free(new_key);
            free(new_value);
            return 0;
        }
        bucket->pairs=temp;
        bucket->count++;
    }
    pair=&(bucket->pairs[bucket->count-1]);//getting a pointer pointing to struct pair at last position of respective bucket.
    pair->key=new_key;
    pair->value=new_value;
    strcpy(pair->key, key);//copying key.
    strcpy(pair->value, value);//copying value.
    return 1;
}



char * map_at(const struct map *map,const char*key)//function to find respective value for a respective key.
{
    unsigned int index;//declaring an unsigned int index.
    struct bucket *bucket;//declaring an pointer to struct bucket.
    struct pair *pair;//declaring pointers to struct pair.
    if(map==NULL)//if map is poiting to NULL then return 0.
    {
        return NULL;
    }
    if(key==NULL)//if key is pointing to NULL then return 0.
    {
        return NULL;
    }
    index=hash(key)%map->count;//finding index using hashing finction.(taking remider of dividing value of function hash(function that maps every string to a unique unsigned int) by map->count).
    bucket=&(map->buckets[index]);//intializing buckets to map->bucket at index that we got from hashing function.
    pair=get_pair(bucket, key);//initializing pair to return value of function get_pair(function returns the pointer to struct pair which contains element same as key if no such pair is present then the function returns NULL).
    if(pair==NULL)//if the key value is alreading present in the map then returning NULL.
    {
        return NULL;
    }
    return pair->value;//returning value of the key.
}

void map_erase(const struct map*map,char *key,char *value)//function to delete pair containing a key and value.
{
    unsigned int index;//declaring an unsigned int index.
    struct bucket *bucket;//declaring an pointer to struct bucket.
    struct pair *pair,*temp;//declaring pointers to struct pair.
    if(map==NULL)//if map is poiting to NULL then return NULL.
    {
        return;
    }
    if(key==NULL)//if key is pointing to NULL then return NULL.
    {
        return;
    }
    index=hash(key)%map->count;//finding index using hashing finction.(taking remider of dividing value of function hash(function that maps every string to a unique unsigned int) by map->count).
    bucket=&(map->buckets[index]);//intializing buckets to map->bucket at index that we got from hashing function.
    pair=get_pair(bucket, key);//initializing pair to return value of function get_pair(function returns the pointer to struct pair which contains element same as key if no such pair is present then the function returns NULL).
    if(pair==NULL)//if the key value is alreading present in the map then returning NULL.
    {
        return;
    }
    if(strcmp(pair->value,value)==0)//if the key is same then check whether its value is same as what we want to delete or not.if yes then copying the pair at the end to this pair and then deleting the pair at last position by reallocating.
    {
        temp=&(bucket->pairs[bucket->count-1]);//getting a pointer pointing to last pair of the bucket->pairs.
        if(temp!=pair)//if pair(variable pointer) is itself not equal to the pair in the last then then copying the pair at the end to this pair.
        {
            strcpy(pair->key,temp->key);
            strcpy(pair->value,temp->value);
        }
        bucket->pairs=(struct pair *)realloc(bucket->pairs,(bucket->count-1)*sizeof(struct pair));//then deleting the pair at last position by reallocating.
        bucket->count--;//decreasing bucket->count by 1.
        return;
    }
    
}

unsigned int map_size(const struct map *map)//function to find number of elements in our map.
{
    unsigned int count=map->count;
    unsigned int size=0;//declaring a temporary variable size and initializing it to 0.
    for(int i=0;i<count;i++)//iterating over map->buckets
    {
        size+=(map->buckets[i]).count;//increasing size by the number of elements int map->buckets at the respective position.
    }
    return size;//returning value of our temporary variable size.
}

void map_display(const struct map *map)//function to display all elements in our map.
{
    unsigned int count=map->count;
    unsigned int size=0;//declaring a temporary variable size and initializing it to 0.
    struct bucket *bucket;
    struct pair *pair;
    for(int i=0;i<count;i++)//iterating over map->buckets
    {
        size=(map->buckets[i]).count;
        bucket=&(map->buckets[i]);
        pair=(bucket->pairs);
        for(int j=0;j<size;j++)//iterating over map->buckets at the position i.
        {
            printf("%s -> %s\n",pair->key,pair->value);//printing the respective key and value.
        }
    }
}

static unsigned long hash(const char *str)//hash function which gives unique unsigned integer values for all different strings.
{
    unsigned long hash =1;//declare a temporary variable hash and initialize it to 1.
    int c;
    unsigned long x=strlen(str);
    for (int i = 0; i < x; i++)//iterating over the string.
     {
        c = *str;//making c equal to the ASCII value of letter at position i in the string.
        str++;
        hash = (hash*2) + c;//making our temporary value hash twice of its previous value and adding c in it.
    }
    return hash;//returning value of our temporary variable hash.
}


static struct pair *get_pair(struct bucket *bucket,const char *key)//function to check whether a key is already present in the map or not.
{
    unsigned int n;//declare a temporary variable n.
    struct pair *pair;//declaring a pointer to struct pair.
    n=bucket->count;//making n equal to number of elements in bucket->pairs.
    if(n==0)//if n is equal to 0 then return NULL.
    {
        return NULL;
    }
    pair=bucket->pairs;
    for(unsigned int i=0;i<n;i++)//iterating over bucket->pairs
    {
        if(pair->key!=NULL&&pair->value!=NULL)
        {
            if(strcmp(pair->key, key)==0)//if the key value is found then return the pointer to the respective bucket->pairs in which it is found.
            {
                return pair;
            }
        }
        pair++;
    }
    return NULL;//if the key is not found then return NULL.
}


int main() {
    
    struct map *m;
    m=map_new(10000);
    if(m==NULL)
    {
        printf("not enough memory");
        return 0;
    }
    map_insert(m, "Hello", "World");
    
    map_insert(m, "1", "how");
    map_insert(m, "2", "are");
    map_insert(m, "3", "you");
    if(map_at(m, "Hello")!=NULL)
    printf("%s\n",map_at(m, "Hello"));
    if(map_at(m, "1")!=NULL)
    printf("%s\n",map_at(m, "1"));
    if(map_at(m, "2")!=NULL)
    printf("%s\n",map_at(m, "2"));
    if(map_at(m, "3")!=NULL)
    printf("%u\n",map_size(m));
    printf("%u\n",map_size(m));
    printf("\n\n");
    map_display(m);
    
    printf("\n\n");
    strcpy(map_at(m, "Hello"),"world!!");
    
    map_erase(m, "Hello", "World");
    if(map_at(m, "Hello")!=NULL)
    printf("%s\n",map_at(m, "Hello"));
    if(map_at(m, "1")!=NULL)
    printf("%s\n",map_at(m, "1"));
    if(map_at(m, "2")!=NULL)
    printf("%s\n",map_at(m, "2"));
    if(map_at(m, "3")!=NULL)
    printf("%u\n",map_size(m));
    printf("\n\n");
    map_display(m);
    
    printf("\n\n");
    
    map_erase(m, "1", "how");
    if(map_at(m, "Hello")!=NULL)
    printf("%s\n",map_at(m, "Hello"));
    if(map_at(m, "1")!=NULL)
    printf("%s\n",map_at(m, "1"));
    if(map_at(m, "2")!=NULL)
    printf("%s\n",map_at(m, "2"));
    if(map_at(m, "3")!=NULL)
    printf("%u\n",map_size(m));
    printf("\n\n");
    map_display(m);
    
    printf("\n\n");
    
    map_erase(m, "2", "are");
    if(map_at(m, "Hello")!=NULL)
    printf("%s\n",map_at(m, "Hello"));
    if(map_at(m, "1")!=NULL)
    printf("%s\n",map_at(m, "1"));
    if(map_at(m, "2")!=NULL)
    printf("%s\n",map_at(m, "2"));
    if(map_at(m, "3")!=NULL)
    printf("%u\n",map_size(m));
    printf("\n\n");
    map_display(m);
    
    printf("\n\n");
    
    map_erase(m, "3", "you");
    if(map_at(m, "Hello")!=NULL)
    printf("%s\n",map_at(m, "Hello"));
    if(map_at(m, "1")!=NULL)
    printf("%s\n",map_at(m, "1"));
    if(map_at(m, "2")!=NULL)
    printf("%s\n",map_at(m, "2"));
    if(map_at(m, "3")!=NULL)
    printf("%u\n",map_size(m));
    printf("\n\n");
    map_display(m);
    
    printf("\n\n");
    printf("\n\n");
    
    set a = create(); // Creating set
    set b = create(); //Creating set

    //Inserting values in a
    insert(a, 10);
    insert(a, 20);
    insert(a, 30);
    insert(a, 100);
    insert(a, 90);
    insert(a, 40);
    insert(a, 50);
    insert(a, 60);
    insert(a, 70);
    insert(a, 80);
    insert(a, 150);
    insert(a, 110);
    insert(a, 120);
    insert(a, 78);
    insert(a, 33);
    insert(a, 987);
    insert(a, 66);
    insert(a, 55);
    insert(a, 130);
    insert(a, 155);

    printf("Printing values in Set a : ");
    display(a,a->root);
    printf("\n");
    printf("Size of a : %d\n",size(a));
    node *ptr=begin(a);
    printf("The begin element : %d\n",ptr->data);
    ptr=end(a);
    printf("The end element: %d\n",ptr->data);

    //Inserting values in b
    insert(b, 1);
    insert(b, 2);
    insert(b, 3);
    insert(b, 8);
    insert(b, 5);
    insert(b, 10);
    insert(b, 90);
    insert(b, 100);
    insert(b, 70);
    
    printf("Printing values in Set b : ");
    display(b,b->root);
    printf("\n");
    printf("Size of b : %d\n",size(b));
    ptr=begin(b);
    printf("The begin element : %d\n",ptr->data);
    ptr=end(b);
    printf("The end element: %d\n",ptr->data);

    //FINDING POINTER TO 33 IN ROOT AND PRINTING ITS DATA
    ptr=find(a,33);
    printf("data of ptr given by find function %d\n",ptr->data);

    // Deleting values from set a
    erase(a, 10);
    erase(a, 987);
    erase(a, 33);

    //FINDING POINTER TO 33 IN ROOT AND PRINTING ITS DATA
    printf("After deletion :");
    ptr=find(a,33);

    printf("After deleting values in a --\n");
    printf("Printing values in Set a : ");
    display(a,a->root);
    printf("\n");
    printf("Size of a : %d\n",size(a));
    ptr=begin(a);
    printf("The begin element : %d\n",ptr->data);
    ptr=end(a);
    printf("The end element: %d\n",ptr->data);

    // Deleting values from set b
    erase(b, 10);
    erase(b, 3);

    printf("After deleting values in b --\n");
    printf("Printing values in Set b : ");
    display(b,b->root);
    printf("\n");
    printf("Size of b : %d\n",size(b));
    ptr=begin(b);
    printf("The begin element : %d\n",ptr->data);
    ptr=end(b);
    printf("The end element: %d\n",ptr->data);
    
    printf("\n\n");
    printf("\n\n");
    
    struct vector *v;
    //calling create function just once to initialise the vector
    v=vector_create(0);

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
