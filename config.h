#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

#define KPR_CONFIG_PATH ".config"

typedef struct node
{
	pid_t pid;
	char *name;
	char *arg;
	struct node *next;
}KPR;

KPR *kpr_config_init();

void kpr_open_config(void);

void kpr_read_config(KPR *krp);

void kpr_close_config(KRP *krp);

#endif
