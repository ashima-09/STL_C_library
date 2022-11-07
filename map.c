#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
    printf("%s\n",map_at(m, "3"));
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
        printf("%s\n",map_at(m, "3"));
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
        printf("%s\n",map_at(m, "3"));
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
        printf("%s\n",map_at(m, "3"));
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
        printf("%s\n",map_at(m, "3"));
    printf("%u\n",map_size(m));
    printf("\n\n");
    map_display(m);
    
    return 0;
}
