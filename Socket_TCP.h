#ifndef _SOCKET_TCP_H_
#define _SOCKET_TCP_H_
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#define ERR_EXIT(m)\
do\
{\
	perror(m);\
	exit(0);\
}while(0)
typedef struct
{
	int fd;
	sockaddr_in *link;
}peer;
class Tcp_client
{
private:
	int client=-1;
	struct sockaddr_in link;
public:
	Tcp_client(char *ip, int port, int flag);
	int Connect();
	sockaddr_in getlink()
	{
		return link;
	}
};
Tcp_client::Tcp_client(char *ip, int port, int flag)
{
	client = socket(PF_INET, SOCK_STREAM | flag, 0);
	bzero(&link,sizeof(link));
	link.sin_family = PF_INET;
	link.sin_addr.s_addr = inet_addr(ip);
	link.sin_port = htons(port);
}
int Tcp_client::Connect()//0 success -1\-2 error;
{
	if (client == -1)
		return -2;
	socklen_t len = sizeof(sockaddr_in);
	int ret=connect(client, (sockaddr*)&link, len);
	return client;
}
class Tcp_server
{
private:
	int server;
	struct sockaddr_in seraddr;
public:
	Tcp_server(char *ip, int port, int flag);
	int getopt()
	{
		return this->server;
	}
	sockaddr_in getseraddr()
	{
		return this->seraddr;
	}
	Tcp_server();
	peer Accept()//-1error
	{
		sockaddr_in *peermes=new sockaddr_in;
		socklen_t len = sizeof(struct sockaddr);
		int fd = accept(server,(struct sockaddr *)peermes,&len);
		peer ret;
		ret.fd = fd;
		ret.link = peermes;
		return ret;
	}
};
Tcp_server::Tcp_server()
{
	printf("without init\n");
	exit(0);
}
Tcp_server::Tcp_server(char *ip, int port, int flag)
{
	server = socket(PF_INET, SOCK_STREAM|flag, 0);
	seraddr.sin_family = PF_INET;
	//inet_aton(ip, &seraddr.sin_addr);
	seraddr.sin_addr.s_addr = inet_addr(ip);
	seraddr.sin_port = htons(port);
	if (bind(server, (struct sockaddr *)&seraddr, sizeof(struct sockaddr)) != 0)
		ERR_EXIT("bind error");
	if (listen(server, 4) != 0)
		ERR_EXIT("listen error");
}
#endif // !_SOCKET_TCP_H
