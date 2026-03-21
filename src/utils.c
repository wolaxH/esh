#include "utils.h"
#include <fcntl.h>
#include <string.h>

int swPthFmt(char *path){
    char *home = getenv("HOME");
    if (home == NULL) return -1;

    // /home/username -> ~
    int hpl = strlen(home);
    if (strncmp(home, path, hpl) == 0) {
        *path = '~';
        memmove(path + 1, path + hpl, strlen(path + hpl) + 1);
        return 1;
    }
    // ~ -> /honme/username
    if (*path == '~'){
        memmove(path + hpl, path + 1, strlen(path + 1) + 1);
        strncpy(path, home, strlen(home));
        return 0;
    }
    return -1;
}


void split_redir(char *input, options *opt){
    char *p = input;
    while (*p != '>' && *p != '<' && *p != 0) p++;

    if (*p == '\0'){ // no redirect
        opt->isredir = 0;
        opt->redirP = NULL;
        return;
    }

    memmove(p + 1, p, strlen(p));
    *p = 0;
    opt->isredir = 1, opt->redirP = p+1;
}

void redir(char *p){
    int fd;
    char *f;
    int O_flag;

    if (*p == '>'){
        p += (*(p+1) == '>') ? 2 : 1;
        O_flag = (*(p+1) == '>') ?
            O_WRONLY | O_CREAT | O_APPEND :
            O_WRONLY | O_CREAT | O_TRUNC;

        f = strtok(p, " \t\n");
        fd = open(f, O_flag, 0644);

        if (fd == -1){
            fprintf(stderr, "esh: Cannot open the file %s", f);
            return;
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
    else if (*p == '<'){
        f = strtok(p+1, " \t\n");
        if ((fd = open(f, O_RDONLY)) == -1){
            fprintf(stderr, "esh: Cannot open the file %s\n", f);
            return;
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
}

int forkwrapper(options opt, void (*builtInFunc)(char **, int), char *args[], int argc, int pipes[][2], int pipec){
    pid_t pid = fork();
    if (pid < 0) perror("fork");

    if (pid == 0){
        if (opt.isredir)
            redir(opt.redirP);
        if (opt.ispipe){
            if (opt.pipe[0] != -1) dup2(opt.pipe[0], STDIN_FILENO);
            if (opt.pipe[1] != -1) dup2(opt.pipe[1], STDOUT_FILENO);
        }
        for (int i = 0; i < pipec; i++) {
            close(pipes[i][0]);
            close(pipes[i][1]);
        }

        builtInFunc(args, argc);
        exit(0);
    }
    return pid;
}

int redirable(char *cmd){
    if(strcmp(cmd, "pwd") == 0 ||
       strcmp(cmd, "echo") == 0 ||
       strcmp(cmd, "export") == 0) return 1;
    return 0;
}


int split_pipe(char *input, char *cmds[], options *opt){
    int count = 1;
    *cmds++ = input;
    opt->ispipe = 0;
    while(*input++ != '\0'){
        if (*input == '|'){
            count++;
            opt->ispipe = 1;
            *input = 0;
            while(*(++input) == ' ');
            *cmds++ = input;
        }
    }
    *cmds = NULL;
    return count;
}
