#include "stdafx.h"
#include "ThreadPool.h"
#include <thread>

ThreadPool* ThreadPool::m_owner = NULL;
ThreadPool::ThreadPool():pool()
{
    pool.nHaveFree = 0;
    pFirstMission = NULL;
    pLastMission = NULL;
    //获取线程数
    pool.nSumpthread = std::thread::hardware_concurrency();
    for (size_t i = 0; i < pool.nSumpthread; i++)
    {
        std::thread *pthread = new std::thread(&ThreadPool::defcallback, this);
        pool.vWorkLink.push_back(pthread);
    }
}

void ThreadPool::defcallback()
{
    std::mutex Mutex;
    std::unique_lock<std::mutex> UniMutex(Mutex);
    while (true) {
        if (pFirstMission != NULL)//还有任务情况
        {
            Mission *pmission = NULL;
            pool.nMutex.lock();//临界资源获取
            
            {
                if (pFirstMission != NULL) {
                    pmission = pFirstMission;
                    pFirstMission = pFirstMission->next;
                }
            }
            pool.nMutex.unlock();

            if (pmission) {
                pmission->missionlink();//任务执行
                delete pmission;
            }
        }


        else//没有任务，进入休眠
        {
            pool.nHaveFree++;        
            pool.cond.wait(UniMutex);
        }
    }
}


ThreadPool * ThreadPool::GetInstance()
{
    if (m_owner == NULL)
        m_owner = new ThreadPool();
    return m_owner;
}

void ThreadPool::Release()
{
    Mission *pMission;
    //清除任务链
    while (pFirstMission != NULL) {
        pMission = pFirstMission;
        pFirstMission = pFirstMission->next;
        delete pMission;
    }
    //释放线程
    for (int i = 0; i < pool.vWorkLink.size(); i++) {
        pool.vWorkLink.at(i)->~thread();
    }
    this->~ThreadPool();
}

ThreadPool::~ThreadPool()
{
}

void ThreadPool::PushMission(void(*mission)())
{
    pool.nMutex.lock();
    if (pFirstMission == NULL)//任务堆入
    {
        pFirstMission = new Mission();
        pLastMission = pFirstMission;
        pFirstMission->missionlink = mission;
    }
    else
    {
        pLastMission->next = new Mission();
        pLastMission = pLastMission->next;
        pLastMission->missionlink = mission;
    }
    pool.nMutex.unlock();
    if (pool.nHaveFree)//有未工作的线程，尝试唤醒抢夺
    {
        pool.nHaveFree--;
        pool.cond.notify_one();
    }
    
}
