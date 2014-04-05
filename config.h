/*
 * The Awesome Shell
 * config.h
 *
 * Vanessa Apolinario de Lima - 7239256
 * Fernando Cury Gorodscy - 7152354
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <unistd.h>

/* Prompt */
#define PROMPT_CHAR "-> "
#define PROMPT prompt()
#define PATH_SIZE 255

char* prompt() {
    char* str = (char*) malloc(sizeof(char)*PATH_SIZE);
    getcwd(str, PATH_SIZE);
    return strcat(str, PROMPT_CHAR);
}

void help(void) {
	fprintf(stderr, "To run commands: %sCOMMAND ARG [ ARG ... ] [ < INFILE ] [ > OUTFILE | >> OUTFILE ]\n", PROMPT);
	fputs("In which both INFILE and OUTFILE are the path to a file\n", stderr);
}

#define PROMPT_QUIT "Exiting our awesome prompt..."

/* Maximum command size */
#define COMMAND_MAX 65535
#define ARGS_MAX 255

/* Word delimiters */
#define WORD_DELIMITERS " \t\n"
/* #define DELIMITERS "|&;()<>" WORD_DELIMITERS */

#endif /* CONFIG_H */
