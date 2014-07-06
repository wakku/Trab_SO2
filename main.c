/* main-foo.c - linked to other.o, needs libfoo and libbaz */

#include <stdlib.h>
#include <stdio.h>
#include <foosh.h>
#include <debug.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define PROMPT_CHAR " -> "
#define PROMPT prompt()
#define PATH_SIZE 255

char* prompt()
{
    char* str = (char*) malloc(sizeof(char)*PATH_SIZE);
    getcwd(str, PATH_SIZE);
    return strcat(str, PROMPT_CHAR);
}

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
        go_on = 0;
    }
    else if (!strcmp (command, "cd"))
    {
        chdir(args[1]);
    }
    else if (!strcmp (command, "jobs"))
    {
        
    }
    else if (!strcmp (command, "bg"))
    {
        
    }
    else if (!strcmp (command, "fg"))
    {
        
    }
    else
    {
        return 0;
    }
    return 1;
}

int exec_external(const char *cmd, char *args[], const char *infile, const char *outfile, int count)
{
	int flags, status, infd, outfd, append = 1, fd[2];
	pid_t cpid;
    
    pipe (fd);
    
	if ((cpid = fork()) == 0) {
		/* child process */
        
        /* If not the first in the pipeline */
        if(count != 0)
        {
            dup2 (fd[0], 0);
        }
        /* if not the last */
        if (cmd != NULL) {
            dup2 (fd[1], 1);
        }
        
		if (strlen(infile) != 0) {
			infd = open(infile, O_RDONLY);
			if (infd < 0) {
				fprintf(stderr, "%s: ", infile);
				perror(NULL);
				exit(EXIT_FAILURE);
			}
			dup2(infd, 0);
		}
        
		if (strlen(outfile) != 0) {
            flags = O_CREAT | O_WRONLY;
			if (!append)
				flags |= O_TRUNC;
			else
				flags |= O_APPEND;
			outfd = open(outfile, flags, 511);
			if (infd < 0) {
				fprintf(stderr, "%s: ", outfile);
				perror(NULL);
				exit(EXIT_FAILURE);
			}
			dup2(outfd, 1);
		}
        
		if (execvp(cmd, args) < 0) {
			fprintf(stderr, "%s: command not found\n", cmd);
			exit(EXIT_FAILURE);
		}
	} else {
		/* parent process */
        waitpid(cpid, &status, 0);
	}
    
	return 1;
}

int main (int argc, char **argv)
{
  buffer_t *command_line;
  int i, aux;

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

      parse_command_line (command_line, pipeline);

	  for (i=0; pipeline->command[i][0]; i++)
	    {
            if (!exec_builtin(pipeline->command[i][0], pipeline->command[i])){
                exec_external(pipeline->command[i][0], pipeline->command[i], pipeline->file_in, pipeline->file_out, i);
            }
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

  release_command_line (command_line);
  release_pipeline (pipeline);
    
  printf ("\n%s ", PROMPT);

  return EXIT_SUCCESS;
}
