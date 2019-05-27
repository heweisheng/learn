#include"block.h"
#include"Socket_TCP.h"
typedef struct//不考虑主机字节序问题
{
	int size;
	char *buff;
}buff;
int main()
{
	Tcp_client cli("127.0.0.1", 43491,0);
	int fd = cli.Connect();
	if (fd == -1)
		ERR_EXIT("ser");
	buff send;
	while (true)
	{
		char *str = new char[1000];
		scanf("%s", str);
		send.size = strlen(str);
		send.buff = str;
		try
		{
			int ret = Writes(fd, &send.size, sizeof(int));
			if(ret==-1)
			{ 
				throw"ser close";
			}
			ret = Writes(fd, send.buff, send.size);
			if(ret==-1)
			{
				throw "ser close";
			}
			delete[]send.buff;
		}
		catch (char *str)
		{
			printf("%s\n", str);
			break;
		}
	}
}