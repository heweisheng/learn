#ifndef XML_H
#define XML_H
#include<iostream>
#include<stddef.h>
#include<cstring>
#include<stdio.h>
#include<stdlib.h>
using namespace std; 
/*version 1.0.1 超简单的xml封包，单层结构*/
/*计划多层结构架构但暂时没必要*/
/*父子信息都要考虑结束符！某些时候可能会分配一块连续的内存空间， 
而字符串的读取时根据结束符识别的。多申请一个字符给结束符用。*/
static const char hex_chars[] = "0123456789abcdef";
static const char xml_check[] = {
	/*
	0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
	*/
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  /*  00 -  0F control chars */
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  /*  10 -  1F */
	0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /*  20 -  2F & */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0,  /*  30 -  3F < > */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /*  40 -  4F */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /*  50 -  5F */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /*  60 -  6F */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,  /*  70 -  7F DEL */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /*  80 -  8F */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /*  90 -  9F */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /*  A0 -  AF */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /*  B0 -  BF */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /*  C0 -  CF */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /*  D0 -  DF */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /*  E0 -  EF */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /*  F0 -  FF */
};
class XML_PACK
{
protected:
	char *info;					//父层信息 
	char **C_info;				//子层信息 
	size_t use;					//父层信息使用字节 
	size_t size;				//父层信息总共的字节-1 
	size_t C_arrary_use;		//子信息队列长 
	size_t *C_use;				//子信息内容使用大小 
	size_t *C_size;				//子信息内容总共大小 
	size_t C_arrary_size;		//子信息队列总大小 
public:
	XML_PACK();					//初始化 
	~XML_PACK();
	char* info_init();								//可整合暂时无思路 
	char* XML_check(char *mes);
	int info_copy(int multiple);					//扩大父层信息空间 
	int XML_C_info(char *type,char *value);			//子信息生成函数 
	void XML_C_recovery();							//重置子信息，取消反复申请，减少碎片 
	char* XML_info(char *type,int start,int end);	//生成整体信息 
};
char* XML_PACK::XML_check(char *mes)//xml格式检测，根据国际标准lighttpd的函数移植 
{
	char *p;
	int len=strlen(mes);
	int newlen=0;
	for(int i=0;i<len;i++)
	{
		if(xml_check[*(mes+i)]==1)
			newlen+=6;
		else
			newlen++;
	}
	newlen++;
	p=new char[newlen];
	//cout<<newlen<<endl;
	newlen=0;
	for(int i=0;i<len;i++)
	{
		if(xml_check[*(mes+i)]==1)
		{
				p[newlen++] = '&';
				p[newlen++] = '#';
				p[newlen++] = 'x';
				p[newlen++] = hex_chars[(*(mes+i) >> 4) & 0x0F];
				p[newlen++] = hex_chars[*(mes+i) & 0x0F];
				p[newlen++] = ';';
		}
		else
			p[newlen++] = *(mes+i);
	}
		p[newlen] = '\0';
	return p;
	
}
char* XML_PACK::info_init()		//初始化父层空间 
{
	char *mes;
	mes=new char[101];
	if(mes==NULL)
	{
		cout<<"memory create error3"<<endl;
		exit(0);
	}
	mes[0]='\0';
	return mes;
}
XML_PACK::~XML_PACK()
{
	for(int i=0;i<this->C_arrary_size;i++)
		delete []this->C_info[i];
	delete []this->C_info;
	delete []this->C_size;
}
XML_PACK::XML_PACK()			//初步申请10个子信息队列跟100个字符的父信息 
{
	this->info=info_init();
	this->size=100;
	this->use=0;
	this->C_info=new char*[10];
	this->C_use=new size_t[10];
	this->C_size=new size_t[10];
	this->C_arrary_use=0;
	this->C_arrary_size=10;
	for(int i=0;i<10;i++)
		C_size[i]=0;
}
void XML_PACK::XML_C_recovery()					//重置工作 
{
	this->C_arrary_use=0;
	for(int i=1;i<this->C_arrary_use;i++)
	{
		this->C_info[i][0]='\0';
		this->C_use[i]=0;
	}
}
int XML_PACK::XML_C_info(char *type,char *value)
{
	value=this->XML_check(value);
	size_t check=strlen(value);
	size_t len=strlen(type)*2+strlen("<></>\n")+check;			//子信息需使用长 
	
	
	if(this->C_size[this->C_arrary_use]!=0&&this->C_size[this->C_arrary_use]<(len+1)) 
	/*若不为新的子信息并且可用长度不够则重新申请*/
	{
		delete []this->C_info[this->C_arrary_use];
		this->C_info[this->C_arrary_use]=new char[len+1];
		this->C_size[this->C_arrary_use]=len;
	}


	else if(this->C_size[this->C_arrary_use]==0)/*新信息申请空间*/ 
	{
		this->C_info[this->C_arrary_use]=new char[len+1];
		this->C_size[this->C_arrary_use]=len;
	}
	if(this->C_info[this->C_arrary_use]==NULL)
		exit(0);
	sprintf(this->C_info[C_arrary_use],"<%s>%s</%s>\n",type,value,type);
	//cout<<C_info[C_arrary_use];
	this->C_use[C_arrary_use]=len;/*更新使用队列的信息*/ 
	
	this->C_arrary_use++;
	if(this->C_arrary_use==this->C_arrary_size)/*如果空间已经满了开辟新空间*/
	{
		size_t lens=this->C_arrary_size*2;/*倍数扩充并复制原来的内容*/
		char **newC_arrary;
		newC_arrary=new char*[lens];
		if(newC_arrary==NULL)
		{
			cout<<"memory create error1";
			exit(0);
		}
		char **del=this->C_info;
		memcpy(newC_arrary,this->C_info,sizeof(char *)*(this->C_arrary_size+1));
		this->C_info=newC_arrary;
		delete []del;
		
		
		size_t *dels=this->C_use;
		this->C_use=new size_t[lens];
		if(this->C_use==NULL)
		{
			cout<<"memory create error2";
			exit(0);
		}
		memcpy(this->C_use,dels,sizeof(size_t)*this->C_arrary_size);
		delete []dels;
		dels=this->C_size;
		this->C_size=new size_t[lens];
		if(this->C_size==NULL)
		{
			cout<<"memory create error";
			exit(0);
		}
		for(int i=0;i<lens;i++)/*new的空间不初始化，所以要自己赋值，如果用alloc则会自己处理*/
		{
			this->C_size[i]=0;
		}
		delete []dels;
		this->C_arrary_size=lens;
	}
	delete []value;
	return this->C_arrary_use-1;/*让用户生成总体信息的关键key*/
}
int XML_PACK::info_copy(int multiple)		/*扩大父信息空间*/
{
	char *del=this->info;
	this->info=new char[(this->size*multiple)+1];
	strcpy(this->info,del);
	delete []del;
	return 1;
}
char* XML_PACK::XML_info(char *type,int start,int end)
/*储备父信息start跟end是子信息队列的编号，装入连续队列进行生成*/
/*由于以后可能要扩展暂时保留start参数，默认0*/
{
	size_t len=this->use;
	for(int i=start;i<=end;i++)
	{
		len=len+this->C_use[i];
	}
	if(len>=this->size)
		info_copy(len/this->size+1);
	/*if(end-start>=1)
	{*/
		sprintf(this->info+this->use,"<%s>\n",type);
		this->use=this->use+strlen(type)+strlen("<>\n");
		for(int i=start;i<=end;i++)
		{
			strcpy(this->info+this->use,this->C_info[i]);
			this->use+=this->C_use[i];
		}
		sprintf(this->info+this->use,"</%s>\n",type);
		this->use=use+strlen(type)+strlen("</>\n");
	/*}*/
	this->use=0;
	this->XML_C_recovery(); 
	char *ret=this->info;
	this->info=this->info_init();
	return ret;
}
#endif
