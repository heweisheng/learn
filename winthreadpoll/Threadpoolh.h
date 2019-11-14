#pragma once
//Design by heweisheng
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include <Windows.h>
#include <assert.h>
typedef struct _Pool {
    std::mutex nMutex;                      //线程池临界资源锁
    size_t nSumpthread;                     //线程数
    std::vector<std::thread*> vWorkLink;    //工作线程
    std::atomic<int> nHaveFree;                          //是否有空闲线程
    std::condition_variable cond;           //条件变量
}Pool;//线程信息库

class IThreadMission
{
public:
    virtual void RunFunction() = 0;//线程任务
};



typedef struct _mission {
    //void(*missionlink)(void *argv);
    IThreadMission *missionlink;
    _mission *next;
    HANDLE Event;
    _mission() :missionlink(NULL), next(NULL), Event(NULL) {}
}Mission;//任务链



class ThreadPoolH
{
public:
    //单例接口
    static ThreadPoolH* GetInstance();
    //清除线程资源
    bool Release();
    ~ThreadPoolH();
    //任务装载器,如果需要同步需要对joinable进行ture赋值，并且必须Join该任务例子
    //HANDLE res=xxx->PushMission();
    //...
    //...
    //xxx->Join(res);
    HANDLE PushMission(IThreadMission *mission, bool joinable = false);
    //template<typename T>
    //HANDLE PushMission(void(T::*mission)(),T *obj,bool joinable=false);
    void Join(HANDLE &Event);
    bool Anyncjoin(HANDLE &Event);
private:
    ThreadPoolH();


    //初始线程回调，任务调度器
    void defcallback();
    static ThreadPoolH *m_owner;             //线程池实体
    Pool pool;                              //线程信息
    Mission *pFirstMission;                 //第一个任务
    Mission *pLastMission;                  //最后一个任务
    atomic_bool m_bState;                      //线程池状态
};
