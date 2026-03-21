#ifndef UTIL_H_
#define UTIL_H_

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

/*========== structure ==========*/

typedef struct{
    char* name;
    void (*func)(char *agrs[], int argc);
} FuncTbl;

typedef struct{
    int isredir;
    char *redirP;
    int ispipe;
    int pipe[2];
} options;

/*========== function ==========*/
/*
 * function ChangePathFormat
 * this function switch the path format between /home/username/ and ~
 *
 * if change /home/username to ~ return 1
 * if change ~ to /home/username return 0
 * if fail return -1
 */
int swPthFmt(char *path);


/**
 * @bref a parse function for redirection symbol
 * this function will separate the cmd and the redirection symbol by \0.
 * then set opt.isredir to 1, and opt.redirP to the pointer to
 * the first character of the redirection part
 * otherwise opt.isredir = 0, redirP = NULL
 * @note this function should be call at thefirst of parse
 *
 * @param input the pointer to the user command
 */
void split_redir(char *input, options *opt);

/**
 * @bref redirection to the target file
 * parse the redirection part to get the target file name
 * set the dup2 in sub process
 *
 * @param p the pointer to the first character of the redirection part
 */
void redir(char *p);

/**
 * @bref a wrapper function to execute the builtIn function in child process
 *
 * @param opt to provide the info and options
 * @param builtInFunc the action to be performed (builtIn Function)
 * @param args the arguments for `builtInFunc`
 * @param argc the arguments for `builtInFunc`
 * @param pipes the other pipes that should be close
 * @param pipec pipe count
 */
int forkwrapper(options opt, void (*builtInFunc)(char **, int), char *args[], int argc, int pipes[][2], int pipec);

/**
 * @bref check if the command can be redirection
 * if so return 1 ,otherwise return 0
 *
 * @param cmd the command to be checked
 */
int redirable(char *cmd);

/**
 * @bref split the pipe command
 * split the command by `|` and put each pointer to command to `cmds`
 * and then return the commands count
 *
 * @param input the user input
 * @param cmds a pointer array, Each element corresponds to a command (end with a NULL)
 * @param opt the command options @see utils.h
 */
int split_pipe(char *input, char *cmds[], options *opt);


#endif // UTIL_H_
