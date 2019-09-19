#include "ThreadPool.hpp"
#include <iostream>
thread_local WsQueue* ThreadPool::m_local_work_queue = nullptr;
thread_local unsigned ThreadPool::m_my_index = 0;

ThreadPool::ThreadPool() : m_done(false), m_joiner(m_threads)
{
    unsigned const thread_count = 6;
    try
    {
        for (unsigned i = 0; i < thread_count; ++i)
        {
            m_queues.push_back(std::unique_ptr<WsQueue>(new WsQueue));
        }
        for (unsigned i = 0; i < thread_count; ++i)
        {
            m_threads.push_back(std::thread(&ThreadPool::worker_thread, this, i));
        }
    }
    catch (...)
    {
        m_done = true;
        throw;
    }
}

void ThreadPool::worker_thread(unsigned my_index)
{
    m_my_index = my_index;
    m_local_work_queue = m_queues[m_my_index].get();
    while (!m_done)
    {
        run_pending_task();
    }
}

bool ThreadPool::pop_task_from_local_queue(task_type &task)
{
    return m_local_work_queue && m_local_work_queue->try_pop(task);
}
bool ThreadPool::pop_task_from_pool_queue(task_type &task)
{
    return m_local_work_queue && m_pool_work_queue.try_pop(task);
}
bool ThreadPool::pop_task_from_other_thread_queue(task_type &task)
{
    for (unsigned i = 0; i < m_queues.size(); ++i)
    {
        unsigned const index = (m_my_index + i + 1) % m_queues.size();
        if (m_queues[index]->try_steal(task))
        {
            return true;
        }
    }
    return false;
}
void ThreadPool::run_pending_task()
{
    task_type task;
    if (pop_task_from_local_queue(task) ||
        pop_task_from_pool_queue(task) ||
        pop_task_from_other_thread_queue(task))
    {
        task();
    }
    else
    {
        std::this_thread::yield();
    }
}
