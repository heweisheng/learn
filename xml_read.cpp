#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define mallocif(m)\
do\
{\
	if((m)==NULL)\
		exit(0);\
}while(0);
typedef struct
{
	char *ptr;
	size_t use;
	size_t size;
} buffer;
buffer* buffer_init(int size)
{	
	buffer *buff;
	mallocif(buff=(buffer *)malloc(sizeof(buffer)));
	if(buff==NULL)
	{
		return NULL;
	}
	if(size!=0)
		mallocif(buff->ptr=(char *)malloc(sizeof(char)*(size+1)));
	buff->use=0;
	buff->size=size;
	return buff;
}
void buffer_del(buffer *buff)
{
	if(buff==NULL)
		return;
	free(buff->ptr);
	free(buff);

}
void buffer_reset(buffer *buff)
{
	buff->use=0;
}
typedef struct
{
	buffer **type;
	buffer **value;
	size_t size;
	size_t use;
}xml_pack;
xml_pack* xml_init()
{
	xml_pack *pack;
	pack=(xml_pack *)malloc(sizeof(xml_pack));
	mallocif(pack->type=(buffer **)malloc(sizeof(buffer *)*4));
	mallocif(pack->value=(buffer **)malloc(sizeof(buffer *)*4));
	int i=0;
	for(;i<4;i++)
	{
		pack->type[i]=buffer_init(0);
		pack->value[i]=buffer_init(0);
	}
	pack->size=4;
	pack->use=0;
	return pack;
}
void xml_copy2(xml_pack *pack)
{
	buffer **p;
	p=pack->type;
	int i=pack->size;
	pack->size*=2;
	printf("111");
	mallocif(pack->type=(buffer **)malloc(sizeof(buffer *)*pack->size));
	memcpy(pack->type,p,pack->size*sizeof(buffer*)/2);
	p=pack->value;
	mallocif(pack->value=(buffer **)malloc(sizeof(buffer *)*pack->size));
	memcpy(pack->value,p,pack->size*sizeof(buffer*)/2);
	for(;i<pack->size;i++)
	{
		pack->type[i]=buffer_init(0);
		pack->value[i]=buffer_init(0);
	}
	printf("222\n");
}
void xml_del(xml_pack *pack)/*无实体*/
{
	int i=0;
	free(pack->type);
	free(pack->value);
	free(pack);
}
void xml_reset(xml_pack *pack)
{
	pack->use=0;
}
xml_pack* xml_read(buffer *buff)
{
	char *ptr=buff->ptr;
	xml_pack *pack;
	buffer *mes;
	pack=xml_init();
	size_t use=0;
	int read=0;
	int i=0;
	int entity=0;
	while(1)
	{
		printf("use %d\n",use);
		while(i<buff->use&&ptr[i++]!='<');
		if(buff->use==i)
			break;
		mes=pack->type[use];
		mes->ptr=ptr+i;
		if(ptr[i++]!='/')
		{
			i--;
			while(i<buff->use&&ptr[i++]!='>')
			{
				mes->use++;
			}
			mes=pack->value[use];
			mes->ptr=ptr;
			use++;
			read++;
		}
		else
		{
			for(entity=read-1;entity!=-1;entity--)
			{
				buffer *c=pack->type[entity];
				buffer *p=pack->value[entity];
				if(strncmp(c->ptr,ptr+i,c->use)==0)
				{
					p->ptr=c->ptr+c->use+1;
					p->use=ptr+i-(c->ptr)-c->use-3;						
					break;					
				}
			}
		}
		if(use>=pack->size)
		{
			//printf("22\n");
			xml_copy2(pack);
		}		
	}
	pack->use=use;
	return pack;
}
int main()
{
	xml_pack *sec;
	char xml_mes[]="<name>我是徐茅山</name>\
<age>今年20岁</age>\
<sex>男</sex>\
<name>我是徐茅山</name>\
<name>我是徐茅山</name>\
<name>我是徐茅山</name>\
<name>我是徐茅山</name>\
   ";
	buffer *xml=buffer_init(sizeof(xml_mes));
	strcpy(xml->ptr,xml_mes); 
	xml->use=sizeof(xml_mes);
	//printf("%s\n %d\n",xml->ptr,xml->size);
	xml_pack *getmes=xml_read(xml);
	int i=0;
	int j=0;
	for(;i<getmes->use;i++)
		{
			for(j=0;j<(getmes->type[i])->use;j++)
				putchar(*((getmes->type[i])->ptr+j));
			putchar('\n');
			for(j=0;j<(getmes->value[i])->use;j++)
				putchar(*((getmes->value[i])->ptr+j));
			putchar('\n');
		}
}
