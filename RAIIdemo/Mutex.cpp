#include "Mutex.h"
Mutex::Mutex(pthread_mutex_t *mutex)//RAII获取资源就是初始化=>初始化就是获取资源=>构造就是获取资源
{
	str = "create";//创建调用
	this->mutex = mutex;
	pthread_mutex_lock(this->mutex);
	std::cout << "thread_id lock:" << pthread_self() << std::endl;
}


Mutex::~Mutex()//析构即是释放资源
{
	str = "destory";//析构结果，其实没意义，因为对象已经摧毁了，所以本身是个错误的做法。
	std::cout <<"thread_id unlock:"<< pthread_self() << std::endl;
	pthread_mutex_unlock(mutex);
}
