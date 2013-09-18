#include "config.h"

void to_replace(char *res,int len);
void null_free(char *p);

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

	return kpr;
}

int kpr_read_config(KPR *kpr)
{
	FILE *kpr_fp;
	char name[1024]={0};
	char path[1024]={0};
	char arg[1024]={0};
	KPR *node;

	kpr_fp=fopen(KPR_CONFIG_PATH,"rb");
	if(kpr_fp == NULL)
		return -1;

	while(!feof(kpr_fp))
	{
		fscanf(kpr_fp,"%s",name);
		fscanf(kpr_fp,"%s",path);
		fgets(arg,sizeof(arg)-1,kpr_fp);

		to_replace(name,sizeof(name));
		to_replace(path,sizeof(path));
		to_replace(arg,sizeof(arg));

		if(name[0] == '\0' || path[0] == '\0' || arg[0] == '\0')
			continue;

		node=malloc(sizeof(KPR));
		node->pid=-1;
		node->name=string_add("%s",name);
		node->path=string_add("%s",path);
		if(strcmp(arg,"NULL") == 0)
			node->arg=NULL;
		else
			node->arg=string_add("%s",arg);
		node->next=NULL;
		kpr->next=node;
		kpr=kpr->next;

		bzero(name,sizeof(name));
		bzero(path,sizeof(path));
		bzero(arg,sizeof(arg));
	}

	fclose(kpr_fp);

	return 0;
}

void kpr_close_config(KPR *kpr)
{
	while(kpr->next != NULL)
	{
		null_free(kpr->name);
		null_free(kpr->path);
		null_free(kpr->arg);
		free(kpr);
		kpr=kpr->next;
	}
}

void to_replace(char *res,int len)
{
	char *temp;

	if(strstr(res,"\t"))
	{
		temp=string_add("%s",res);
		bzero(res,len);
		strreplace(temp,"\t","",res,len);
		free(temp);
	}

	if(strstr(res,"\n"))
	{
		temp=string_add("%s",res);
		bzero(res,len);
		strreplace(temp,"\n","",res,len);
		free(temp);
	}
}

void null_free(char *p)
{
	if(p == NULL)
		return;
	free(p);
}
