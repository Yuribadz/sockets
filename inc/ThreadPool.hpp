#include "FunctionWrapper.hpp"
#include "TsQueue.hpp"
#include "WsQueue.hpp"
#include "Joiner.hpp"
#include <atomic>
#include <vector>
#include <future>
#include <memory>

class ThreadPool
{
private:
    typedef FunctionWrapper task_type;
    std::atomic_bool m_done;
    TsQueue<task_type> m_pool_work_queue;
    std::vector<std::unique_ptr<WsQueue>> m_queues;
    std::vector<std::thread> m_threads;
    Joiner m_joiner;
    static thread_local WsQueue *m_local_work_queue;
    static thread_local unsigned m_my_index;
    void worker_thread(unsigned my_index);
    bool pop_task_from_local_queue(task_type &task);
    bool pop_task_from_pool_queue(task_type &task);
    bool pop_task_from_other_thread_queue(task_type &task);

public:
    ThreadPool();
    ~ThreadPool()
    {
        m_done = true;
    }
    template <typename FunctionType>
    std::future<typename std::result_of<FunctionType()>::type> submit(FunctionType f)
    {
        typedef typename std::result_of<FunctionType()>::type result_type;
        std::packaged_task<result_type()> task(f);
        std::future<result_type> res(task.get_future());
        if (m_local_work_queue)
        {
            m_local_work_queue->push(std::move(task));
        }
        else
        {
            m_pool_work_queue.push(std::move(task));
        }
        return res;
    }
    void run_pending_task();
};