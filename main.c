#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include <sys/types.h>
#include "sys/wait.h"
// #define EXIT_SUCCESS 0
#define LSH_RL_BUFSIZE 1024
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"



char *lsh_readline(void)
{
    int buf_size = LSH_RL_BUFSIZE;
    int position = 0;
    char *buf = (char *)malloc(sizeof(char) * buf_size);
    int c;
    if (!buf)
    {
        fprintf(stderr, "lsh: Allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        c = getchar();
        if (c == EOF || c == '\n')
        {
            buf[position] = '\0';
            return buf;
        }
        else
        {
            buf[position] = c;
        }
        position++;

        if (position > buf_size)
        {
            buf_size += LSH_RL_BUFSIZE;
            buf = realloc(buf, buf_size * sizeof(char));
            if (!buf)
            {
                fprintf(stderr, "lsh: Allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

char** lsh_split_line(char* line) {
    int buf_size = LSH_TOK_BUFSIZE;
    int pos = 0;
    char** tokens = (char**)malloc(sizeof(char*) * buf_size);
    char* token;
    if(!tokens) {
        fprintf(stderr, "lsh: Allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, LSH_TOK_DELIM);
    while (token != NULL) {
        tokens[pos] = token;
        pos++;
        if (pos > buf_size)
        {
            buf_size += LSH_RL_BUFSIZE;
            tokens = realloc(tokens, buf_size * sizeof(char*));
            if (!tokens)
            {
                fprintf(stderr, "lsh: Allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    tokens[pos] = NULL;
    return tokens;
}

int lsh_launch(char** args) {
    pid_t pid, wpid;
    int status;

    pid = fork();
    if(pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("lsh");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        perror("FUCJKED");
    } else {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    

    return 1;
}

void lsh_loop()
{
    char *line;
    char **args;
    int status;
    do
    {
        printf("> ");
        line = lsh_readline();
        args = lsh_split_line(line);
        status = lsh_launch(args);
        free(line);
        free(args);
    } while (status);
}

int main(int argc, char const *argv[])
{
    lsh_loop();
    return EXIT_SUCCESS;
}
