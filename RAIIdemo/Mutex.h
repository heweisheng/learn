#pragma once
#include <pthread.h>
#include <iostream>
#include <assert.h>
class Mutex//effective c++ 资源管理使用类=>资源管理类不要使用指针，因为会导致不确定什么时候释放。
{
private:
	std::string str;
	pthread_mutex_t *mutex;
public:
	Mutex(pthread_mutex_t *mutex);
	~Mutex();
	std::string& get() {
		return this->str;
	}
};

