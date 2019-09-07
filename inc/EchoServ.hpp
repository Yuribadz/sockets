#ifndef ECHOSERV_HPP
#define ECHOSERV_HPP
#include <syslog.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>                   
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>




class EchoServ
{
public:
	static constexpr const int BUF_SIZE = 4096;
	static constexpr const int BACKLOG = 10;
	static constexpr const char* SERVICE = "echoserv";
	static constexpr const char BD_NO_CHDIR =  01;         
	static constexpr const char BD_NO_CLOSE_FILES = 02;    
	static constexpr const char BD_NO_REOPEN_STD_FDS = 04; 
	                                                       
	static constexpr const char BD_NO_UMASK0 = 010;
	static constexpr const short BD_MAX_CLOSE = 8192;
                      

	EchoServ();
	int start();

private:
	int becomeDaemon(int flags);
	int inetListen(const char *service,
				   int backlog,
				   socklen_t *addrlen);
	int inetPassiveSocket(const char *service,
					  int type, socklen_t *addrlen,
					  bool doListen,
					  int backlog);
	static void handleRequest(int clientFd);


	struct sigaction m_sa;
	int m_cfd; // Client file descriptor
	int m_sfd; // Server file descriptor
};
#endif
