#include <iostream>
#include <string>
#include <thread>
#include "TsQueue.hpp"
int main()
{
	TsQueue<int> que;
	int count = 0;
	auto func1 = [&que]{for(int i = 0; i < 100; ++i)
	{
		que.push(i);
	}};
	auto func2 = [&que]{for(int i = 100; i < 200; ++i)
		{
			que.push(i);
		}};
	auto popperPipe = [&que, &count]
					  {
						  std::this_thread::sleep_for(std::chrono::seconds(2));
						  while(auto val = que.try_pop())
						  {
							  ++count;
							  std::cout << "#"<< count <<
								  "popped value=" << *val.get()
										<< "\n";
						  }
					  };
	std::thread thread3(popperPipe);
	thread3.detach();
	std::thread thread1(func1);
	std::thread thread2(func2);
	thread1.join();
	thread2.join();
	std::this_thread::sleep_for(std::chrono::seconds(3));
    return 0;
}
