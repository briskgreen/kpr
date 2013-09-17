#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
	pid_t pid;
	char *name;
	char *arg;
	struct node *next;
}KPR;

#endif
