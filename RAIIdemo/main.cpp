#include"Mutex.h"
#include<unistd.h>
#include<vector>
#include<assert.h>
class Test{
public:
	Mutex &rm;
public:
	Test(Mutex &mutex):rm(mutex) {};
};
static void *pfunction(void *mutex) {
	pthread_mutex_t *pm = (pthread_mutex_t *)mutex;
	Test *test;//����bug
	while (1)//�����������
	{
		std::cout << "pthread_id:" << pthread_self() << std::endl;
		{//���򣬳����ͷ���Դ���������ݱ�����Ҳ�ᱻ�ͷţ���ע��ȥ������˲�䱨�ϣ�����debug�������ܣ�û���δ������֡�
			Mutex raiimutex(pm);
			test = new Test(raiimutex);
		}
		//std::cout<<test->rm.get()<<std::endl;
		/*����֪�����ݱ������ˣ���������bug�ܶ࣬Ҫע�����������⡣*/
		/*ָ��Լ�������ã������cer��ѧ���ò�Ҫ��ͷ��ָ������ò�Ҫͬʱ���֣��ر��ǿ���*/
		/*��ν�����һ��{}���䣬{}���Է�ֹ������Ⱦ*/
		/*���ǣ���Ҫʹ��ָ�����+���ö������*/
		delete test;
		sleep(5);
	}
}
int main()
{
	pthread_t thread,thread2;//����2���߳�
	pthread_mutex_t pm;//����������������Ҫ��ʵ����󣬷����ʼ����ը��
	int ret = pthread_mutex_init(&pm,NULL);
	if (ret != 0) {
		perror("error");
		return 0;
	}
	
	pthread_create(&thread, NULL, pfunction, (void *)&pm);
	pthread_create(&thread2, NULL, pfunction, (void *)&pm);
	while (1) {//��ϲ�����Ը���join
		sleep(100);
	}
}