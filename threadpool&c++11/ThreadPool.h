#pragma once
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
typedef struct _Pool {
    std::mutex nMutex;                      //线程池临界资源锁
    size_t nSumpthread;                     //线程数
    std::vector<std::thread*> vWorkLink;    //工作线程
    std::atomic<int> nHaveFree;                          //是否有空闲线程
    std::condition_variable cond;           //条件变量
}Pool;//线程信息库


typedef struct _mission {
    void(*missionlink)();
    _mission *next;
    _mission() :missionlink(NULL), next(NULL) {}
}Mission;//任务链



class ThreadPool
{
public:
    //单例接口
    static ThreadPool* GetInstance();
    //清除线程资源
    void Release();
    ~ThreadPool();

    //任务装载器
    void PushMission(void(*mission)());
private:
    ThreadPool();

    //初始线程回调，任务调度器
    void defcallback();


    static ThreadPool *m_owner;             //线程池实体
    
    
    Pool pool;                              //线程信息
    Mission *pFirstMission;                 //第一个任务
    Mission *pLastMission;                  //最后一个任务
};

