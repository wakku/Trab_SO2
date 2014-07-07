/* utils. c - some handy functions. */

#include <debug.h>
#include <foosh.h>

/* Allocate and initialize a list. Parameter del is a pointer
   to a function to be used to release the memory used by
   a node's value.*/

list_t *new_list (void (*del)(void*))
{
  list_t *list;
  list = malloc (sizeof (list_t));
  sysfault (!list, NULL);
  list->size = 0;
  list->first = NULL;
  list->last = NULL;
  list->del = del; 
  return list;
}

/* Free memory used by a list. */

void release_list (list_t *list)
{
  list_node_t *p;

  fatal (!list->del, "Don't know how to free nodes.");

  /* Free all nodes. */
  p = list->first;
  while (p)
    {
      list->del (p);
      p = p->next;
    }

  free (list);
  
}

/* Append a node at the end of the list. */

list_node_t * append_node (list_t *list)
{
  list_node_t * node;
  
  node = malloc (sizeof (list_node_t));
  sysfault (!node, NULL);
  node->value = NULL;

  if (list->first == NULL)
    {
      list->first = node;
      node->previous = NULL;
    }
  else
    {
      list->last->next = node;
      node->previous = list->last;
      }
  
  list->last = node;
  node->next = NULL;

  return node;
}

/* Remove a node from the list. */

int del_node (list_t *list, list_node_t *node)
{
  list_node_t *p;
  /* Seek the node. */
  p = list->first;
  while ( (p) && (p!=node))
    p = p->next;
  
  /* No such node. */
  if (!p)
    return EXIT_FAILURE;

  /* Ok node found. Let's remove it. */
  
  if (node == list->first)
    list->first = node->next;
  else
    node->previous->next = node->next;

  if (node == list->last)
    list->last = node->previous;
  else
    node->next->previous = node->previous;

  list->del (node);

  return EXIT_SUCCESS;
}


/* String dup. */

char * stringdup (const char *str)
{
  char *p;
  int n;

  n = strlen (str);
  p = malloc (n*sizeof(char));
  sysfault (!p, NULL);
  strcpy (p, str);
  return p;
}

/* 

Bellow are just test functions used during development. 

*/

/* void delstring (void *str) */
/* { */
/*   free (str); */
/* } */


/* void test (void) */
/* { */
/*   list_t *list; */
/*   list_node_t *p, *p1, *p2, *p3, *p4; */
  
/*   list = new_list (delstring); */

/*   p1 = append_node (list); */
/*   p1->value = stringdup ("Node 1"); */

/*   p2 = append_node (list); */
/*   p2->value = stringdup ("Node 2"); */

/*   p3 = append_node (list); */
/*   p3->value = stringdup ("Node 3"); */

/*   p = list->first; */
/*   while (p) */
/*     { */
/*       printf ("> %s\n", (char *) p->value); */
/*       p = p->next; */
/*     } */


/*   printf ("Del\n"); */
/*   del_node (list, p2); */

/*   p = list->first; */
/*   while (p) */
/*     { */
/*       printf ("> %s\n", (char *) p->value); */
/*       p = p->next; */
/*     } */

/*   printf ("Add\n"); */
/*   del_node (list, p2); */
  
/*   p4 = append_node (list); */
/*   p4->value = stringdup ("Node 4"); */

/*   p = list->first; */
/*   while (p) */
/*     { */
/*       printf ("> %s\n", (char *) p->value); */
/*       p = p->next; */
/*     } */

/* } */

