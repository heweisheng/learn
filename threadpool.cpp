#include <pthread.h>
#include <signal.h>
#include <iostream>
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
typedef struct Node
{
	void *(*mission)(void* stat);//任务队列中任务项 
	void *value;				//任务队列对应参数 
	Node *nextnode;				//下一个任务 
}thread_mission;
//C++create第三个参数必须是静态函数，如果要内传参数，必须用静态内容。 
class threadpool
{
	int thread_limit;					//线程大小 
	static pthread_t *thread;			//线程id 
	static pthread_mutex_t *pmutex;		//线程锁，主线程激励工作 
	static void *work(void *arg);		//工作线程定义 
	static thread_mission *list;		//任务队列 头 
	static thread_mission *bottom;		//任务队列尾 
	public:threadpool(int num)			//初始化创建线程池 
	{
		thread_limit=num;
		thread=new pthread_t[num];
		pmutex=new pthread_mutex_t;
		pthread_mutex_init(pmutex,NULL);
		pthread_mutex_lock(&mutex);
		for(int i=0;i<num;i++)
		{
			if((pthread_create(thread+i,NULL,work,this))!=0)
				throw std::exception();
		}
	}
	public:void thread_input(void *(*mission)(void *stat),void *stat);		//线程堆入 
	public:void join()		
	{
		while(1)
		{
			pthread_mutex_lock(pmutex);
			if(list==NULL)
				return;
			pthread_mutex_unlock(pmutex);
		}
	}
	public:~threadpool()	//析构，关闭线程 
	{
		pthread_mutex_destroy(pmutex);
		for(int i=0;i<thread_limit;i++)
			kill(thread[i],SIGKILL);
		delete []thread;
		delete pmutex;		
	}
};
pthread_t *threadpool::thread=NULL;
pthread_mutex_t *threadpool::pmutex=NULL;
thread_mission *threadpool::list=NULL;
thread_mission *threadpool::bottom=NULL;

void threadpool::thread_input(void *(mission)(void *stat),void *stat)
{
	pthread_mutex_lock(pmutex);//任务堆入前先确保其他线程没有修改 
	thread_mission *point=new thread_mission;
	point->mission=mission;
	point->value=stat;
	point->nextnode=NULL;
	if(list==NULL)
		bottom=list=point;
	else
	{
		bottom->nextnode=point;
		bottom=point;
	}
	pthread_mutex_unlock(pmutex);
	pthread_mutex_unlock(&mutex);//这里其实是通知空闲进程开始工作 
}

void *threadpool::work(void *arg)//工作线程 
{
	while(1)
	{
		//std::cout<<"test"<<std::endl;
		if(list==NULL)
		{
			pthread_mutex_lock(&mutex);//如果所有任务处理完，等待主线程派发新任务 
		}
		pthread_mutex_lock(pmutex);//确认其他线程没有抢占资源 
		thread_mission *point=list;
		list=list->nextnode;
		pthread_mutex_unlock(pmutex);
		(point->mission)(point->value);
		delete point;
	}
}
void *demo(void *arg)
{
	int *point=(int *)arg;
	std::cout<<*point<<std::endl;
	delete point;
	return NULL;
}
int main()
{
	threadpool t(4);
	sleep(1);//线程创建好了并不是立马开始工作，bug原因想不出来，内部只有锁会冲突，很迷。去掉会概率发生段错误
	for(int i=0;i<100;i++)
	{
		int *point=new int;
		*point=i;
		t.thread_input(demo,(void *)point);
	}
	t.join();
	return 0;
}

