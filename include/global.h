#ifndef GLOBAL_H_
#define GLOBAL_H_

#define MAX_INPUT 1024
#define MAX_ARGS 64
#define MAX_PATH_LENGTH 512
#define MAX_ENV_VAR 256
#define MAX_PIPE_COUNT 64

typedef struct{
    char *name;
    char *val;
} SSmap;

extern char **environ;


#endif // GLOBAL_H_
