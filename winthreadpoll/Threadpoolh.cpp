#include <thread>
#include"stdafx.h"
#include "Threadpoolh.h"
ThreadPoolH* ThreadPoolH::m_owner = NULL;
ThreadPoolH::ThreadPoolH() :pool()
{
    pool.nHaveFree = 0;
    pFirstMission = NULL;
    pLastMission = NULL;
    m_bState = true;
    //获取线程数
    pool.nSumpthread = std::thread::hardware_concurrency()/2;
    if (pool.nSumpthread <= 2)
        pool.nSumpthread = 2;
    for (size_t i = 0; i < pool.nSumpthread; i++)
    {
        std::thread *pthread = new std::thread(&ThreadPoolH::defcallback, this);
        pool.vWorkLink.push_back(pthread);
    }

}



void ThreadPoolH::defcallback()
{
    std::mutex Mutex;
    std::unique_lock<std::mutex> UniMutex(Mutex);
    while (true) {
        if (pFirstMission != NULL)//还有任务情况
        {
            Mission *pmission = NULL;
            pool.nMutex.lock();//临界资源获取
            {
                if (pFirstMission != NULL&& m_bState) {
                    pmission = pFirstMission;
                    pFirstMission = pFirstMission->next;
                }
            }
            pool.nMutex.unlock();
            if (pmission) {
                pmission->missionlink->RunFunction();//任务执行
                if (pmission->Event != 0) {
                    SetEvent(pmission->Event);
                }
                delete pmission;
            }
        }
        else//没有任务，进入休眠
        {
            pool.nHaveFree++;
            pool.cond.wait(UniMutex);
            if (!m_bState)
                return;
        }
    }
}





ThreadPoolH * ThreadPoolH::GetInstance()
{
    if (m_owner == NULL)
        m_owner = new ThreadPoolH();
    return m_owner;
}



bool ThreadPoolH::Release()
{
    m_bState = false;
    Mission *pMission;
    //清除任务链
    pool.nMutex.lock();
    while (pFirstMission != NULL) {
        pMission = pFirstMission;
        pFirstMission = pFirstMission->next;
        delete pMission;
    }
    pool.nMutex.unlock();
    if (pool.nHaveFree != pool.vWorkLink.size())
        return false;
    //释放线程
    pool.cond.notify_all();
    for (int i = 0; i < pool.vWorkLink.size(); i++) {
        //TerminateThread(pool.vWorkLink.at(i)->native_handle(), 0);        
        pool.vWorkLink.at(i)->join();
        delete pool.vWorkLink.at(i);
    }

    delete this;
    m_owner = NULL;
    return true;
}



ThreadPoolH::~ThreadPoolH()
{
}

HANDLE ThreadPoolH::PushMission(IThreadMission *mission, bool joinable)
{
    HANDLE ret = NULL;
    if (mission == NULL|| !m_bState)//如果线程池已经进入清理状态了，则不进行任务堆入
        return ret;
    if (joinable)//同步需求
    {
        ret = CreateEventA(NULL, FALSE, FALSE, NULL);
        assert(ret != NULL);
    }
    pool.nMutex.lock();
    if (pFirstMission == NULL)//任务堆入
    {
        pFirstMission = new Mission();
        pLastMission = pFirstMission;
        pFirstMission->missionlink = mission;
        pFirstMission->Event = ret;
    }
    else
    {
        pLastMission->next = new Mission();
        pLastMission = pLastMission->next;
        pLastMission->missionlink = mission;
        pLastMission->Event = ret;
    }
    pool.nMutex.unlock();
    if (pool.nHaveFree)//有未工作的线程，尝试唤醒抢夺
    {
        pool.nHaveFree--;
        pool.cond.notify_one();
    }
    return ret;
}

void ThreadPoolH::Join(HANDLE &Event)
{
    if (Event != NULL) {
        WaitForSingleObject(Event, INFINITE);
        CloseHandle(Event);
    }
    Event = NULL;
}

bool ThreadPoolH::Anyncjoin(HANDLE &Event)
{
    if (Event == NULL)
        return true;
    DWORD nRet = WaitForSingleObject(Event, 0);
    if (nRet == WAIT_OBJECT_0)
    {
        CloseHandle(Event);
        Event = NULL;
        return true;
    }
    return false;
}