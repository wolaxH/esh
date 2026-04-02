#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "utils.h"
#include "built_in.h"
#include "global.h"

void parse(char *input, char **args,int *argc, options *opt){
    //if redirection
    split_redir(input, opt);
    int i = 0;
    char *sp;
    args[i] = strtok_r(input, " \t\n", &sp);
    while (args[i] != NULL) {
        if (*sp == '"') { /* for argument is inside double quotes */
            char *l = sp + 1;
            while (*(++sp) != '"'); /* to the character that is "  */
            char *p = sp;
            strtok_r(NULL, " \t\n", &sp);
            *p = 0;
            args[++i] = l;
            continue;
        }
        
        args[++i] = strtok_r(NULL, " \t\n", &sp);

        // env var
        if (args[i] && *(args[i]) == '$'){
            char* val = getenv(args[i] + 1);
            args[i] = val ? val : "";
        }
    }
    *argc = i;
}

pid_t exec(char **args, options opt, int pipes[][2], int pipec){
    pid_t pid = fork();

    if (pid < 0) perror("fork");

    if (pid == 0){
        //sub process
        if (opt.isredir == 1)
            redir(opt.redirP);
        if (opt.ispipe == 1){
            if (opt.pipe[0] != -1) dup2(opt.pipe[0], STDIN_FILENO);
            if (opt.pipe[1] != -1) dup2(opt.pipe[1], STDOUT_FILENO);
        }
        for (int i = 0; i < pipec; i++) {
            close(pipes[i][0]);
            close(pipes[i][1]);
        }

        if (execvp(args[0], args) == -1){
            fprintf(stderr, "esh: command not found: %s\n", args[0]);
            exit(1);
        }
    }
    return pid;
}

int main(void){
    char input[MAX_INPUT];
    char *args[MAX_ARGS];
    int argc = 0;
    char cwd[MAX_PATH_LENGTH];
    char *cmds[MAX_PIPE_COUNT]; /* pointer to command, seprate the input by | */
    int cmdsc;

    options opt;

    FuncTbl builtIn[] = {
        {"cd",     builtIn_cd},
        {"pwd",    builtIn_pwd},
        {"exit",   builtIn_exit},
        {"export", builtIn_export},
        {"unset",  builtIn_unset},
        {"echo",   builtIn_echo},
        {NULL, NULL}
    };

    while(1){
        if (!getcwd(cwd, MAX_PATH_LENGTH)){
            perror("getcwd");
        }
        swPthFmt(cwd);
        printf("[%s] esh$ ", cwd);
        fflush(stdout);
        if (!fgets(input, MAX_INPUT, stdin)) break;

        cmdsc = split_pipe(input, cmds, &opt);
        int pipes[cmdsc-1][2];
        pid_t pids[cmdsc];
        int pidc = 0;

        for (int i = 0; i < cmdsc-1; i++) pipe(pipes[i]);

        for(int i = 0; i < cmdsc; i++){
            parse(cmds[i], args, &argc, &opt);

            
            if (args[0] == NULL) continue;
            /* set the pipe field of option */
            if (opt.ispipe == 1){
                opt.pipe[0] = (i > 0) ? pipes[i-1][0] : -1;
                opt.pipe[1] = (i < cmdsc-1) ? pipes[i][1] : -1;
            }
            for (int i = 0; builtIn[i].name != NULL; i++){
                if (strcmp(args[0], builtIn[i].name) == 0){
                    if ((redirable(args[0])) &&
                        (opt.ispipe || opt.isredir))
                        pids[pidc++] = forkwrapper(opt, builtIn[i].func, args, argc, pipes, cmdsc-1);
                    else
                        builtIn[i].func(args, argc);
                    goto next;
                }
            }

            /* external command  */
            pids[pidc++] = exec(args, opt, pipes, cmdsc-1);
        next:
            i;  /* To avoid the lable before the end of code block, that's a C23 extension */
        }
        for (int i = 0; i < cmdsc-1; i++) {
            close(pipes[i][0]);
            close(pipes[i][1]);
        }
        for (int i = 0; i < pidc; i++)
            waitpid(pids[i], NULL, 0);
    }
    return 0;
}
