#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct LinkedList
{
  int data;
  struct LinkedList *next;
  struct LinkedList *prev;
};

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

struct LinkedList *head;

typedef struct Info_
{
  char A[100];
} Info;

typedef struct Element_
{
  Info info;
  struct Element_ *next;
} Element;


struct LinkedList *
LinkedList_new (int x)
{
  struct LinkedList *newNode =
    (struct LinkedList *) malloc (sizeof (struct LinkedList));

  newNode->data = x;
  newNode->prev = NULL;
  newNode->next = NULL;
  return newNode;
}

//Inserts a Node at tail of bi-directional linked list
void *
LinkedList_addToEnd (char x)
{
  struct LinkedList *temp = head;
  struct LinkedList *newNode = LinkedList_new (x);
  if (head == NULL)
    {
      head = newNode;
      return;
    }
  while (temp->next != NULL)
    temp = temp->next;		// Go To last Node
  temp->next = newNode;
  newNode->prev = temp;
}

//linked list in forward traversal order
void *
Forward ()
{
  struct LinkedList *temp = head;
  pthread_mutex_lock (&lock);
  sleep (2);
  printf ("Forward: ");
  while (temp != NULL)
    {
      printf ("%c ", temp->data);
      temp = temp->next;
    }
  printf ("\n");
  pthread_mutex_unlock (&lock);
}

//linked list in reverse traversal order. 
void *
Reverse ()
{
  struct LinkedList *temp = head;
  pthread_mutex_lock (&lock);
  sleep (2);
  if (temp == NULL)
    return;		// empty list, exit
  // Going to last Node
  while (temp->next != NULL)
    {
      temp = temp->next;
    }
  // Traversing backward using prev pointer
  printf ("Reverse: ");
  while (temp != NULL)
    {
      printf ("%c ", temp->data);
      temp = temp->prev;
    }
  printf ("\n");
  pthread_mutex_unlock (&lock);
}

void
print_comparisons (Element * elm)
{

  assert (elm);

  Element *cur = elm;
  Element *next = cur->next;
  pthread_mutex_lock (&lock);
  sleep (2);
  for (; next; cur = next, next = next->next)
    {
      if (strcmp (cur->info.A, next->info.A) == 0)
	printf ("ptrs found are same\n");
      else
	printf ("ptrs found are different\n");
        pthread_mutex_unlock (&lock);
    }
}

void main ()
{
  pthread_t t1, t2, t3;
  int res;
  head = NULL;
  
  pthread_create (&t1, PTHREAD_CREATE_JOINABLE, LinkedList_addToEnd, NULL);
  pthread_join (t1, head);
  pthread_create (&t2, PTHREAD_CREATE_JOINABLE, Forward, NULL);
  pthread_join (t2, head);
  pthread_create (&t3, PTHREAD_CREATE_JOINABLE, Reverse, NULL);
  pthread_join (t3, head);
  
  Info a;a.A[0] = 'A';LinkedList_addToEnd ('A');a.A[1] = '\0';
  Element na;na.info = a;
  Forward ();
  Reverse ();

  Info b;b.A[0] = 'B';LinkedList_addToEnd ('B');b.A[1] = '\0';
  Element nb;nb.info = b;
  Forward ();
  Reverse ();

  Info c;c.A[0] = 'B';LinkedList_addToEnd ('B');c.A[1] = '\0';
  Element nc;nc.info = c;
  Forward ();
  Reverse ();

  Info d;d.A[0] = 'D';LinkedList_addToEnd ('D');d.A[1] = '\0';
  Element nd;nd.info = d;
  Forward ();
  Reverse ();

  na.next = &nb;
  nb.next = &nc;
  nc.next = &nd;
  nd.next = NULL;
  print_comparisons (&na);
  return 0;
}
