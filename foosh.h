/* foosh.h - Utilities of Foo Shell example. */

#ifndef FOOSH_H
#define FOOSH_H

#define MAX_COMMANDS  512  /* Number of commands in a pipeline. */
#define MAX_ARGUMENTS 512  /* Number of arguments per command in a pipeline. */

#define MAX_FILENAME 1024  /* Maximum length of a filename IO redirection).*/

/* Struct to read the command line. */

typedef struct buffer_t
{
  int size;			/* Current buffer size. */
  int length;			/* Current string lenght. */
  char *buffer;			/* String. */
} buffer_t;

/* Return a pointer to a newly allocated and properly initialized 
   buffer_t block or NULL on error. */

buffer_t * new_command_line (void);

/* Release all the memory used by a buffer_t block. */

void release_command_line (buffer_t *);

/* Read a line from STDIN into a buffer_t block. The buffer
   grows as needed in steps of BUFFER_STEP bytes. Buffer does not
   shrinks autmatically.*/

int read_command_line (buffer_t *); 

#define FOREGROUND 0		/* Run in foregroud. */
#define BACKGROUND 1		/* Run in background. */

/* Struct representing a pipeline. */

typedef struct pipeline_t
{
  char ***command;		/* Command line (grows unboundedly). */
  char file_in[MAX_FILENAME];	/* Redirect input from this file. */
  char file_out[MAX_FILENAME];	/* Redirect output to this file. */
  int ground;			/* Either FOREGROUND or BACKGROUND. */
  int ncommands;		/* Number of commands */
  int narguments[MAX_COMMANDS];	/* Number of arguments in each command. */
} pipeline_t;


/* Return a pointer to a newly allocated and properly initialized
   pipeline_t block, or NULL on error.*/

pipeline_t *new_pipeline (void);

/* Release memory used by a pipeline_t block. */

void release_pipeline (pipeline_t *);

/* Parse a command line stored in a buffer_t struct and fill in the
   corresponding fields of the target pipeline_t struct. */

int parse_command_line (buffer_t*, pipeline_t*);

/* Handy macros to check execution mode. */

#define RUN_FOREGROUND(pipeline) (pipeline->ground == FOREGROUND)
#define RUN_BACKGROUND(pipeline) (pipeline->ground == BACKGROUND)
#define REDIRECT_STDIN(pipeline) (pipeline->file_in[0] != '\0')
#define REDIRECT_STDOUT(pipeline) (pipeline->file_out[0] != '\0')

/* Output information of pipeline for debugging purposes. */

void pripeline_info (pipeline_t *pipeline);

/* Struct representing a node in double-linked list. 
   Nodes have a (void*) value which you can point to whatever kind
   of data you want. */

typedef struct list_node_t list_node_t;
struct list_node_t
{
  list_node_t *next;		/* Link to the next node. */
  list_node_t *previous;	/* Link to the previous node. */
  void *value;			/* Value associated to the node. */
};

/* Struct representing a double-linked list. */

typedef struct list_t
{
  int size;			/* Number of nodes in the list. */
  list_node_t *first;		/* First node; NULL if empty. */
  list_node_t *last;		/* Last node; NULL if empty. */
  void (*del)(void *);		/* Function used to free a node->value. */
} list_t;

/* Return a pointer to a newly allocated and properly initialized double-linked
   list. You shuld pass a pointer to the function to be used to free the node
   value. For example, if your nodes point to a (char*) block, you can pass
   a pointer to a free(char *) .*/

list_t * new_list (void (*)(void*));

/* Release the memory used by a list (including all the nodes). */

void release_list (list_t *);

/* Append a node to the end of the list. */

list_node_t * append_node (list_t *);

/* Remove a node from the list (and free the node). */

int del_node (list_t *, list_node_t *);

/* Like strdup(), which is not ISO C. */

char * stringdup (const char *);


#endif	/* FOOSH_H */
