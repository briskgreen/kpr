#ifndef _KPRD_H
#define _KPRD_H

#include <unistd.h>
#include <sys/wait.h>
#include "config.h"

void init_daemon(const char *path,mode_t mask);

void start_process(KPR *kpr);

void kpr_run(KPR *kpr);

void process_restart(KPR *kpr,pid_t pid);

#endif
