#ifndef CONFIG_H
#define CONFIG_H

#include <unistd.h>

/* The prompt */
#define PROMPT_CHAR "-> "
#define PROMPT prompt()
#define PATH_SIZE 255

char* prompt()
{
    char* str = (char*) malloc(sizeof(char)*PATH_SIZE);
    getcwd(str, PATH_SIZE);
    return strcat(str, PROMPT_CHAR);
}

#define PROMPT_QUIT "Exiting our awesome prompt..."

/* Maximum command size */
#define COMMAND_MAX 65535
#define ARGS_MAX 255

/* Word delimiters */
#define WORD_DELIMITERS " \t\n"
/* #define DELIMITERS "|&;()<>" WORD_DELIMITERS */

#endif /* CONFIG_H */
