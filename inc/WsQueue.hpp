#ifndef WSQUEUE_HPP
#define WSQUEUE_HPP

#include "FunctionWrapper.hpp"
#include <deque>
#include <mutex>
class WsQueue{
private:
    typedef FunctionWrapper  data_type;
    std::deque<data_type> m_queue;
    mutable std::mutex m_mutex;
public:
    WsQueue() {};
    WsQueue(const WsQueue& other) = delete;
    WsQueue& operator=(const WsQueue& other) = delete;
    void push(data_type data)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push_front(std::move(data));
    }
    bool empty() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }
    bool try_pop(data_type& res)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if(m_queue.empty())
        {
            return false;
        }
        res = std::move(m_queue.front());
        m_queue.pop_front();
        return true;
    }
    bool try_steal(data_type& res)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if(m_queue.empty())
        {
            return false;
        }
        res = std::move(m_queue.back());
        m_queue.pop_back();
        return true;
    }
};
#endif