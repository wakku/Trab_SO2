/* main-foo.c - linked to other.o, needs libfoo and libbaz */

#include <stdlib.h>
#include <stdio.h>
#include <foosh.h>
#include <debug.h>

#define PROMPT "@:"

/* void test(void); */

int go_on = 1;			/* This variable controls the main loop. */

void help(void)
{
	fprintf(stderr, "To run commands: %sCOMMAND ARG [ ARG ... ] [ <INFILE ] [ >OUTFILE | >>OUTFILE ]\n", PROMPT);
	fputs("Where both INFILE and OUTFILE are the path to a file\n", stderr);
}

int exec_builtin(const char *command, char *args[])
{
	if (!strcmp (command, "help"))
    {
        help();
    }
    else if (!strcmp (command, "exit"))
    {
        exit(EXIT_SUCCESS);
    }
    else if (!strcmp (command, "cd"))
    {
        /*chdir(args[1]);*/
    }
    else
    {
        return 0;
    }
    return 1;
}

int main (int argc, char **argv)
{
  buffer_t *command_line;
  int i, j, aux;

  pipeline_t *pipeline;

  command_line = new_command_line ();
  /* test(); */
  
  pipeline = new_pipeline ();

  while (go_on)
    {
      printf ("%s ", PROMPT);
      fflush (stdout);
      aux = read_command_line (command_line);
      fatal (aux<0, NULL);

      /* Parse command line. */

      if (!parse_command_line (command_line, pipeline) || 1)
	{

	  
	  /* This is an example, of how to use pipeline_t. 
	     See foosh.h for detailed information. */

	  printf ("  Pipeline has %d command(s)\n", pipeline->ncommands);

	  for (i=0; pipeline->command[i][0]; i++)
	    {
	      printf ("  Command %d has %d argument(s): ", i, pipeline->narguments[i]);
          for (j=0; pipeline->command[i][j]; j++){
              printf ("%s ", pipeline->command[i][j]);
              exec_builtin(pipeline->command[i][0], pipeline->command[i]);
          }
	      printf ("\n");
	    }
	  

	  if ( RUN_FOREGROUND(pipeline))
	    printf ("  Run pipeline in foreground\n");
	  else
	    printf ("  Run pipeline in background\n");

	  
	  
	  if ( REDIRECT_STDIN(pipeline))
	    printf ("  Redirect input from %s\n", pipeline->file_in);
	  if ( REDIRECT_STDOUT(pipeline))
	    printf ("  Redirect output to  %s\n", pipeline->file_out);


	}
    }

  release_command_line (command_line);
  release_pipeline (pipeline);

  return EXIT_SUCCESS;
}
