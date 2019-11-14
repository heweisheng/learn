#pragma once
//Design by heweisheng
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include <Windows.h>
#include <assert.h>
typedef struct _Pool {
    std::mutex nMutex;                      //�̳߳��ٽ���Դ��
    size_t nSumpthread;                     //�߳���
    std::vector<std::thread*> vWorkLink;    //�����߳�
    std::atomic<int> nHaveFree;                          //�Ƿ��п����߳�
    std::condition_variable cond;           //��������
}Pool;//�߳���Ϣ��

class IThreadMission
{
public:
    virtual void RunFunction() = 0;//�߳�����
};



typedef struct _mission {
    //void(*missionlink)(void *argv);
    IThreadMission *missionlink;
    _mission *next;
    HANDLE Event;
    _mission() :missionlink(NULL), next(NULL), Event(NULL) {}
}Mission;//������



class ThreadPoolH
{
public:
    //�����ӿ�
    static ThreadPoolH* GetInstance();
    //����߳���Դ
    bool Release();
    ~ThreadPoolH();
    //����װ����,�����Ҫͬ����Ҫ��joinable����ture��ֵ�����ұ���Join����������
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


    //��ʼ�̻߳ص������������
    void defcallback();
    static ThreadPoolH *m_owner;             //�̳߳�ʵ��
    Pool pool;                              //�߳���Ϣ
    Mission *pFirstMission;                 //��һ������
    Mission *pLastMission;                  //���һ������
    atomic_bool m_bState;                      //�̳߳�״̬
};
