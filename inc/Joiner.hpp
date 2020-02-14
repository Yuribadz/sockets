#ifndef JOINER_HPP
#define JOINER_HPP
#include <vector>
#include <thread>
class Joiner {
 private:
  std::vector<std::thread> &m_threads;
 public:
  explicit Joiner(std::vector<std::thread> &threads)
      :
      m_threads(threads) {
  }
  ~Joiner() {
    for (unsigned long i = 0; i < m_threads.size(); ++i) {
      if (m_threads[i].joinable())
        m_threads[i].join();
    }
  }
};
#endif
