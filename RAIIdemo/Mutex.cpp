#include "Mutex.h"
Mutex::Mutex(pthread_mutex_t *mutex)//RAII��ȡ��Դ���ǳ�ʼ��=>��ʼ�����ǻ�ȡ��Դ=>������ǻ�ȡ��Դ
{
	str = "create";//��������
	this->mutex = mutex;
	pthread_mutex_lock(this->mutex);
	std::cout << "thread_id lock:" << pthread_self() << std::endl;
}


Mutex::~Mutex()//���������ͷ���Դ
{
	str = "destory";//�����������ʵû���壬��Ϊ�����Ѿ��ݻ��ˣ����Ա����Ǹ������������
	std::cout <<"thread_id unlock:"<< pthread_self() << std::endl;
	pthread_mutex_unlock(mutex);
}
