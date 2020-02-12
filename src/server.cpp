#include <iostream>
#include <string>
#include <thread>
#include "ThreadPool.hpp"
#include <deque>
#include <vector>
#include <utility>
class Scoped_thread{
	std::thread t;
public:
	explicit Scoped_thread(std::thread t_): t(std::move(t_)){
		if ( !t.joinable()) throw std::logic_error("No thread");
	}
	~Scoped_thread(){
		t.join();
	}
	Scoped_thread(Scoped_thread&)= delete;
	Scoped_thread& operator=(Scoped_thread const &)= delete;
};
int main()
{
	auto handler = []
				   {
					   ThreadPool tp;
					   int counter = 0;
					   std::deque<int> polledFds(1000000,0);
					   std::vector<std::future<std::string>> results;
					   results.reserve(polledFds.size());
					   for(auto polledFd: polledFds)
					   {
						   ++counter;
						   auto task = [counter]{
										   std::cout << "Thread = " <<
										   std::this_thread::get_id() << "\n";
										   std::string nice = "nice ";
										   nice.append(
											   std::to_string(counter));
										   return nice;
									   };
						   results.push_back(tp.submit(task));
					   }
					   for(auto& res: results)
					   {
						   res.get();
					   }
					   results.clear();
				   };
	Scoped_thread(std::thread(handler));				   
					 
	
    return 0;
}
