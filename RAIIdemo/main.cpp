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
	Test *test;//生产bug
	while (1)//测试析构结果
	{
		std::cout << "pthread_id:" << pthread_self() << std::endl;
		{//内域，出域即释放资源，即便内容被引用也会被释放！把注释去掉程序瞬间报废，但是debug程序在跑，没报段错误很奇怪。
			Mutex raiimutex(pm);
			test = new Test(raiimutex);
		}
		//std::cout<<test->rm.get()<<std::endl;
		/*可以知道内容被析构了，所以引用bug很多，要注意作用域问题。*/
		/*指针约等于引用，如果是cer初学引用不要上头，指针跟引用不要同时出现，特别是跨域*/
		/*所谓域就是一个{}区间，{}可以防止变量污染*/
		/*谨记，不要使用指针对象+引用对象组合*/
		delete test;
		sleep(5);
	}
}
int main()
{
	pthread_t thread,thread2;//创建2个线程
	pthread_mutex_t pm;//创建互斥锁，必须要是实体对象，否则初始化会炸。
	int ret = pthread_mutex_init(&pm,NULL);
	if (ret != 0) {
		perror("error");
		return 0;
	}
	
	pthread_create(&thread, NULL, pfunction, (void *)&pm);
	pthread_create(&thread2, NULL, pfunction, (void *)&pm);
	while (1) {//不喜欢可以改用join
		sleep(100);
	}
}