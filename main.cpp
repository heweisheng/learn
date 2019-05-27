#include"block.h"
#include"Socket_TCP.h"
typedef struct//不考虑主机字节序问题
{
	int size;
	char *buff;
}buff;
int main()
{
	peer client;
	Tcp_server ser("127.0.0.1", 43491,0);
	client = ser.Accept();
	printf("ip:%s,port:%d", inet_ntoa(client.link->sin_addr), ntohs(client.link->sin_port));
	buff read;
	int fd = client.fd;
	while (true)
	{
		try {
			int ret = Reads(fd, &read.size, sizeof(int));
			printf("size:%d\n", read.size);
			if (ret == 0)
			{
				throw "cli close";
			}
			read.buff = new char[read.size];
			ret = Reads(fd, read.buff, read.size);
			printf("%s\n", read.buff);
			if (ret == 0)
			{
				throw "cli close";
			}
			delete[]read.buff;
		}
		catch (char *str)
		{
			printf("%s\n", str);
			break;
		}
	}
}