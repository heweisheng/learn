#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdarg.h>
using namespace std;
#define BUFFER_PIECE_SIZE 64
#define CONST_STR_LEN(x) x, x ? sizeof(x) - 1 : 0 
typedef struct
{
	char *ptr;
	size_t size;
	size_t used;
}buffer;
buffer* buffer_init(void) {
	buffer *b;
	b = (buffer *)malloc(sizeof(*b));
	//assert(b);
	b->ptr = NULL;
	b->size = 0;
	b->used = 0;
	return b;
}

//确保用户指定的buffer结构体b的ptr的指向区域至少有size大小的 空闲 内存空间
int buffer_prepare_append(buffer *b, size_t size) {
	if (!b) return -1;
	if (0 == b->size) {
		b->size = size;
		b->size += BUFFER_PIECE_SIZE - (b->size % BUFFER_PIECE_SIZE);
		b->ptr = (char *)malloc(b->size);
		b->used = 0;
		//assert(b->ptr);
	} else if (b->used + size > b->size) {
		b->size += size;
		b->size += BUFFER_PIECE_SIZE - (b->size % BUFFER_PIECE_SIZE);
		b->ptr =(char *) realloc(b->ptr, b->size);
		//assert(b->ptr);
	}
	return 0;
}

int LI_ltostr(char *buf, long val) {
	char swap;
	char *end;
	int len = 1;

	if (val < 0) {
		len++;
		*(buf++) = '-';
//将负数转为正数
		val = -val; 
	}
//使end指针和buf指针所指的位置一致
	end = buf; 
	while (val > 9) {
		*(end++) = '0' + (val % 10);
		val = val / 10;
	}
	*(end) = '0' + val;
	*(end + 1) = '\0'; //这里并没有改变end的值
	len += end - buf; //len=len+end-buf;当指针名没有*符号时，所指的是指针位置

	while (buf < end) { //调换，因为内存先显示低位
		swap = *end;
		*end = *buf;
		*buf = swap;

		buf++;
		end--;
	}

	return len;
}

//将有符号长整型数value转换成对应的十进制字符串，并将其复制到buffer结构体的数据末尾
int buffer_append_long(buffer *b, long val) {
	if (!b) return -1;

	buffer_prepare_append(b, 32);
	if (b->used == 0)
		b->used++;

	b->used += LI_ltostr(b->ptr + (b->used - 1), val);
	return 0;
}

//把指定字符串s复制到用户指定的buffer结构体b的数据空间末尾
int buffer_append_string(buffer *b, const char *s) {
	size_t s_len;

	if (!s || !b) return -1;

	s_len = strlen(s);
	buffer_prepare_append(b, s_len + 1);
	if (b->used == 0)
		b->used++;

	memcpy(b->ptr + b->used - 1, s, s_len + 1);
	b->used += s_len;

	return 0;
}

//把指定字符串s前s_len个字符复制到用户指定的buffer结构体b的数据空间末尾
int buffer_append_string_len(buffer *b, const char *s, size_t s_len) {
	if (!s || !b) return -1;
	if (s_len == 0) return 0;

	buffer_prepare_append(b, s_len + 1);
	if (b->used == 0)
		b->used++;
	memcpy(b->ptr + b->used - 1, s, s_len);
	b->used += s_len;
	b->ptr[b->used - 1] = '\0';
	return 0;
}

int log_error_write(buffer *log,const char *filename, unsigned int line, const char *fmt, ...) {
	va_list ap;
	char *c;
	for(va_start(ap, fmt); *fmt; fmt++) {
		int d;
		char *s;
		buffer *b;
		off_t o;
		switch(*fmt) {
		case 's':           /* string */
			s = va_arg(ap, char *);
			buffer_append_string(log, s); 
			buffer_append_string_len(log, CONST_STR_LEN(" "));
			break;
		case 'i':
			d = va_arg(ap, int);
			buffer_append_long(log, d); 
			buffer_append_string_len(log, CONST_STR_LEN(" "));
			break;
	}
}
	va_end(ap);
	printf("%s\n",log->ptr);
	return 0;
}
int main()
{
	buffer *log;
	log=buffer_init();
	log_error_write(log,__FILE__,__LINE__,"sii","helloworld",112,555);
}
