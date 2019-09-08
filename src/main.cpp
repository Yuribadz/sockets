#include <iostream>
#include <string>
#include <thread>
#include "TsQueue.hpp"
int main()
{
	TsQueue<int> que;
	int count = 0;
	auto func1 = [&que]{for(int i = 0; i < 100000; ++i)
	{
		que.push(i);
	}};
	auto func2 = [&que]{for(int i = 100000; i < 200000; ++i)
		{
			que.push(i);
		}};
	auto func3 = [&que]{for(int i = 200000; i < 300000; ++i)
		{
			que.push(i);
		}};
	auto func4 = [&que]{for(int i = 300000; i < 400000; ++i)
		{
			que.push(i);
		}};
	
	auto popperPipe = [&que, &count]
					  {
						  while(auto val = que.wait_and_pop())
						  {
							  ++count;
							  std::cout << "popped= " << *val.get() << "\n";
							  if(count == 400000)
							  {
								  std::cout << "400k values collected";
								  break;
							  }
							  
						  };
						  std::cout << "count= " << count << "\n";
					  };
	std::thread thread3(popperPipe);
	thread3.detach();
	std::thread thread1(func1);
	std::thread thread2(func3);
	std::thread thread4(func4);
	std::thread thread5(func2);
	thread1.join();
	thread2.join();
	thread4.join();
	thread5.join();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    return 0;
}
