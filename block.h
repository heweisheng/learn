#ifndef _BLOCK_H_
#define _BLOCK_H_
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
ssize_t Reads(int fd, void *buf, size_t count)
{
	int sum = count;
	while (sum)
	{
		int ret = read(fd, buf+count-sum, sum);
		if (ret == 0)
			return 0;
		sum -= ret;
	}
	return count;
}
ssize_t Writes(int fd, void *buf, size_t count)
{
	int sum = count;
	while (sum)
	{
		int ret = write(fd, buf + count - sum, sum);
		if (ret == -1)
			return -1;
		sum -= ret;
	}
	return count;
}
ssize_t Recvs(int fd, void *buf, size_t count,int flag)
{
	int sum = count;
	while (sum)
	{
		int ret = recv(fd, buf +count - sum, sum, flag);
		if (ret == 0)
			return 0;
		sum -= ret;
	}
	return count;
}
ssize_t Sendfiles(int out, int in, off_t *offset, size_t count)
{
	int sum = count;
	while (sum)
	{
		int ret = sendfile(out, in, offset + count - sum, sum);
		if (ret == -1)
			return -1;
		sum -= ret;
	}
	return count;
}
ssize_t Sends(int fd, void *buf, size_t count, int flag)
{
	int sum = count;
	while (sum)
	{
		int ret = send(fd, buf + count - sum, sum, flag);
		if (ret == -1)
			return -1;
		sum -= ret;
	}
	return count;
}
#endif // !_BLOCK_H_