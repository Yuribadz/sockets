#include <iostream>
#include "EchoServ.hpp"


int main(int argc, char* argv[])
{
	
	EchoServ server;
	int a = server.start();
	std::cout << a << "\n";
	return 0;
}
