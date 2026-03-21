#include "built_in.h"
#include "global.h"
#include <stdio.h>
#include <stdlib.h>

void builtIn_cd(char *args[], int argc){
    args[1] = args[1] ? args[1] : getenv("HOME");
    if (*(args[1]) == '~') swPthFmt(args[1]);
    if (chdir(args[1]) == -1) perror("cd");
}

void builtIn_pwd(char *args[], int argc){
    char buf[MAX_PATH_LENGTH];
    if (getcwd(buf, MAX_PATH_LENGTH) == NULL) perror("getcwd");
    printf("%s\n", buf);

}

void builtIn_exit(char *args[], int argc){
    exit(0);
}

void builtIn_export(char *args[], int argc){
    // for $export
    // May sort the result by ascii
    if (args[1] == NULL) {
        char **env = environ;
        printf("%s\n", *env);
        while (*(++env) != NULL){
            printf("%s\n", *env);
        }
        return;
    }

    // for $export name=val
    char *name, *val;
    for (int i = 1; i < argc; i++){
        char *p = args[i];
        while(*(p++) != '=' && *p != '\0');
        if (*(p-1) != '=') continue;
        *(p-1) = '\0';
        name = args[i];
        val = p;
        if (*name == '\0'){
            fprintf(stderr, "esh: bad assignment\n");
            continue;
        }

        if (setenv(name, val, 1)){
            perror("setenv");
        }
    }
}

void builtIn_unset(char *args[], int argc){
    unsetenv(args[1]);
}

void builtIn_echo(char *args[], int argc){
    for (int i = 1; i < argc; i++){
        printf( "%s", args[i]);
    }
    printf("\n");
}
