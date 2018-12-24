# learn
用于学习，期待与各位一起开发服务器专用库
# 利用va宏动态扩充错误日志函数（buffer部分直接拿lighttpd的用），buffer函数的内存重用是lighttpd的精华这里不介绍太多
va_list,va_start,va_arg,va_end.函数  
int log_error_write(buffer *log,const char *filename, unsigned int line, const char *fmt, ...);  
利用第一个参数做参数读取判断，后面想填啥就填啥，如果要补充其他内容自己扩展吧，我把最好用的几个拿出来了(这个函数真方便)
