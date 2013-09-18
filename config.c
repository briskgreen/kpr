#include "config.h"

FILE *kpr_fp=NULL;

KPR *kpr_open_config(void)
{
	KPR *kpr;

	kpr=malloc(sizeof(KPR));
	if(kpr == NULL)
		return NULL;

	kpr->pid=-1;
	kpr->name=NULL;
	kpr->arg=NULL;
	kpr->next=NULL;

	kpr_fp=fopen(KPR_CONFIG_PATH,"rb");

	return kpr;
}

int kpr_read_config(KPR *kpr)
{
	char name[1024]={0};
	char path[1024]={0};
	char arg[1024]={0};
	KPR *node;

	if(kpr_fp == NULL)
		return -1;

	while(!feof(fp))
	{
		fscanf(fp,"%s",name);
		fscanf(fp,"%s",path);
		fscanf(arg,sizeof(arg)-1,fp);

		to_replace(name,sizeof(name));
		to_replace(path,sizeof(path));
		to_replace(arg,sizeof(arg));

		if(name[0] == '\0' || path[0] == '\0' || arg[0] == '\0')
			continue;

		node=malloc(sizeof(KPR));
		node->pid=-1;
		node->name=name;
		node->path=path;
		if(strcmp(arg,"NULL"))
			node->arg=NULL;
		else
			node->arg=arg;
		node->next=NULL;
		kpr->next=node;
		kpr=kpr->next;
	}

	return 0;
}

void kpr_close_config(KPR *kpr)
{
	if(kpr_fp != NULL)
		fclose(kpr_fp);

	while(kpr->next != NULL)
	{
		free(kpr);
		kpr=kpr->next;
	}
}
