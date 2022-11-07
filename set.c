/*-------------------------------------------------------------------------------------------------

Can create multiple set at once

set set_name =create()             //creates a new set
insert(set_name,key)               //insert element into that set
erase(set_name,key)                //delete element from that set
find(set_name,key)                 // find element in that set
begin(set_name)                    // gives the first element of set
end(set_name)                      // gives the last element of set
size(set_name)                     // gives the size of set
display(set_name,set_name->root)   //display the entire set

-------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

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
      x->p->p->c = 0; //color grandparent red
      x->p->p->l->c = x->p->p->r->c = 1; // color both parent and uncle black
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

    if (x == x->p->l) //if x is the left child
    {
      node *sibling = x->p->r;
      if (sibling->c == 0)
      {                        // Case when sibling colour is red
        swap_c(sibling, x->p); // swap the color of sibling and parent
        LEFT_ROTATE(t, x->p);  // Left rotate with root as parent
        sibling = x->p->r;
      }

      // case when both child of sibling are black...
      // this is also the continuing case of above case i.e when parent is red and both the child of sibling are black
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
          sibling->l->c = 1; //change the color of sibling left child to black
          sibling->c = 0; // change the sibling color to red
          RIGHT_ROTATE(t, sibling);
          sibling = x->p->r;
        }

        // also the continiuing case of above case , when farther child of sibling is red
        sibling->r->c = 1; // change the sibling right child color to black
        LEFT_ROTATE(t, x->p);

        // complete and covers the case when x->p is red and sibling and both its children are black
        sibling->c = x->p->c; 
        x->p->c = 1; // change the parent color to black

        x = t->root; //  breaks the loop and to color the root black
      }
    }

    // mirror image of above cases
    else // when x is the right child
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

int main()
{
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
  

  return 0;
}