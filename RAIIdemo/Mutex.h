#pragma once
#include <pthread.h>
#include <iostream>
#include <assert.h>
class Mutex//effective c++ ��Դ����ʹ����=>��Դ�����಻Ҫʹ��ָ�룬��Ϊ�ᵼ�²�ȷ��ʲôʱ���ͷš�
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

