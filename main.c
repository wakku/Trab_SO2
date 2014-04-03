/*
 * main.c
 *
 * The alpha and omega.
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "config.h"

static char command[COMMAND_MAX+1];
static char* args[ARGS_MAX+1];

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
        chdir(args[1]);
    }
    else
    {
        return 0;
    }
    return 1;
}

int exec_external(const char *cmd, char *args[], const char *infile, const char *outfile, int append)
{
	int status, infd, outfd;
	pid_t cpid;

	if ((cpid = fork()) == 0) {
		/* child process */
		if (infile != NULL) {
			infd = open(infile, O_RDONLY);
			if (infd < 0) {
				fprintf(stderr, "ffsh: %s: ", infile);
				perror(NULL);
				exit(EXIT_FAILURE);
			}
			dup2(infd, 0);
		}

		if (outfile != NULL) {
			int flags = O_CREAT | O_WRONLY;
			if (!append)
				flags |= O_TRUNC;
			else
				flags |= O_APPEND;
			outfd = open(outfile, flags);
			if (infd < 0) {
				fprintf(stderr, "ffsh: %s: ", outfile);
				perror(NULL);
				exit(EXIT_FAILURE);
			}
			dup2(outfd, 1);
		}

		if (execvp(cmd, args) < 0) {
			fprintf(stderr, "ffsh: %s: command not found\n", cmd);
			exit(EXIT_FAILURE);
		}
	} else {
		/* parent process */
		waitpid(cpid, &status, 0);
	}

	return 1;
}

int main(int argc, char *argv[])
{
	int i, append = 0;
	char *str, *input;
	const char *cmd, *infile, *outfile;

	/* prompt loop */
	for (;;) {
		printf("%s", PROMPT);
		input = fgets(command, COMMAND_MAX, stdin);

		if (input == NULL)
			break;

		cmd = args[0] = strtok(command, WORD_DELIMITERS);
		if (cmd == NULL)
			continue;

		infile = outfile = NULL;
		for (i = 1; (str = strtok(NULL, WORD_DELIMITERS)) && i < ARGS_MAX;) {
			switch (str[0]) {
			case '<':
				/* change input file */
				if (str[1] == '\0')
					infile = strtok(NULL, WORD_DELIMITERS);
				else
					infile = &str[1];
				break;
			case '>':
				/* change output file */
				if (str[1] != '\0' && str[1] != '>')
					outfile = &str[1];
				else if (str[1] == '\0' || str[2] == '\0')
					outfile = strtok(NULL, WORD_DELIMITERS);
				else if (str[1] == '>') {
					append = 1;
					outfile = &str[2];
				}
				break;
			default:
				args[i++] = str;
			}
		}
		args[i] = NULL;

		if (!exec_builtin(cmd, args))
			exec_external(cmd, args, infile, outfile, append);
	}

	printf("%s\n", PROMPT_QUIT);

	return EXIT_SUCCESS;
}
