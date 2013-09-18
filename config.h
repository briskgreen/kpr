#ifndef _CONFIG_H
#define _CONFIG_H

#include "mysock/mysock.h"

#define KPR_CONFIG_PATH ".config"

typedef struct node
{
	pid_t pid;
	char *name;
	char *arg;
	struct node *next;
}KPR;

KPR *kpr_open_config(void);

int kpr_read_config(KPR *kpr);

void kpr_close_config(KPR *kpr);

#endif
