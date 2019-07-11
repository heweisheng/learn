# learn
用于学习，期待与各位一起开发服务器专用库
# xml
删除了，思考了下不是太实用，多次指针后不适合处理实际问题，指针创建过多
# log_write利用va宏动态扩充错误日志函数（buffer部分直接拿lighttpd的用），buffer函数的内存重用是lighttpd的精华这里不介绍太多
va_list,va_start,va_arg,va_end.函数  
int log_error_write(buffer *log,const char *filename, unsigned int line, const char *fmt, ...);  
利用第一个参数做参数读取判断，后面想填啥就填啥，如果要补充其他内容自己扩展吧，我把最好用的几个拿出来了(这个函数真方便)
# python
以前做了一个空课表的APP，曾经想通过爬虫把那个APP完善，结果发现爬出来的内容是图片...想了下实现不了就算了，所以编写了个爬成绩单的脚本，这样就可以不用打开浏览器那么麻烦，直接运行就可以得到图片。
# 线程池
内部有bug，留给学习的人解决，pthread_kill才是正确做法。
# 防拷贝
c++中有些对象不应该被拷贝，要把一个函数声明成不拷贝就需要对copy方法进行private化，这样就无法调用copy函数，编译器也不会生成。关于为什么要有防copy的存在，详细看effective c++的70页。
# RAIIdemo
利用构造析构的特点做互斥锁管理demo
