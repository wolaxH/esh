#ifndef BUILT_IN_H
#define BUILT_IN_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "utils.h"
#include "global.h"

/*
 * change directory
 */
void builtIn_cd(char *args[], int argc);

/*
 * print working dirctory
 */
void builtIn_pwd(char *args[], int argc);

/*
 * exit the shell
 */
void builtIn_exit(char *args[], int argc);

/*
 * to assign value to variable and save in envirment
 */
void builtIn_export(char *args[], int argc);

/*
 * to unset the envirment variable
 */
void builtIn_unset(char *args[], int argc);

/*
 * to print something
 */
void builtIn_echo(char* args[], int argc);

#endif // BUILT_IN_H
