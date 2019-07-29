#include "EchoServ.hpp"

namespace{
	static void killChild(int sig)
	{
		int savedErrno;
		
		savedErrno = errno;
		while(waitpid(-1, NULL, WNOHANG) > 0)
		{
			continue;
		}
		errno = savedErrno;
	};
}

EchoServ::EchoServ(int clientFd, int socketFd):
	m_sa(),
	m_cfd(clientFd),
	m_sfd(socketFd)
{
	printf("Echoserv started");
}


/* Create an Internet domain socket and bind it to the address
   { wildcard-IP-address + 'service'/'type' }.
   If 'doListen' is TRUE, then make this a listening socket (by
   calling listen() with 'backlog'), with the SO_REUSEADDR option set.
   If 'addrLen' is not NULL, then use it to return the size of the
   address structure for the address family for this socket.
   Return the socket descriptor on success, or -1 on error. */
static int inetPassiveSocket(const char *service,
							 int type, socklen_t *addrlen,
							 bool doListen,
							 int backlog)
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, optval, s;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_socktype = type;
    hints.ai_family = AF_UNSPEC;        /* Allows IPv4 or IPv6 */
    hints.ai_flags = AI_PASSIVE;        /* Use wildcard IP address */

    s = getaddrinfo(NULL, service, &hints, &result);
    if (s != 0)
        return -1;

    /* Walk through returned list until we find an address structure
       that can be used to successfully create and bind a socket */

    optval = 1;
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
            continue;                 /* On error, try next address */

        if (doListen) {
            if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optval,
                    sizeof(optval)) == -1) {
                close(sfd);
                freeaddrinfo(result);
                return -1;
            }
        }

        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;                      /* Success */

        /* bind() failed: close this socket and try next address */

        close(sfd);
    }

    if (rp != NULL && doListen) {
        if (listen(sfd, backlog) == -1) {
            freeaddrinfo(result);
            return -1;
        }
    }

    if (rp != NULL && addrlen != NULL)
		/* Return address structure size */
        *addrlen = rp->ai_addrlen;

    freeaddrinfo(result);

    return (rp == NULL) ? -1 : sfd;
}

/* Create stream socket, bound to wildcard IP address + port given in
  'service'. Make the socket a listening socket, with the specified
  'backlog'. Return socket descriptor on success, or -1 on error. */

int EchoServ::inetListen(const char *service,
						 int backlog,
						 socklen_t *addrlen)
{
    return inetPassiveSocket(service, SOCK_STREAM,
							 addrlen, true, backlog);
}

int EchoServ::becomeDaemon(int flags)
{
    int maxfd, fd;

    switch (fork()) {           /* Become background process */
    case -1: return -1;
    case 0:  break;             /* Child falls through... */
    default: _exit(EXIT_SUCCESS);     /* while parent terminates */
    }

    if (setsid() == -1)         /* Become leader of new session */
        return -1;

    switch (fork()) {           /* Ensure we are not session leader */
    case -1: return -1;
    case 0:  break;
    default: _exit(EXIT_SUCCESS);
    }

    if (!(flags & BD_NO_UMASK0))
        umask(0);               /* Clear file mode creation mask */

    if (!(flags & BD_NO_CHDIR))
        chdir("/");             /* Change to root directory */

    if (!(flags & BD_NO_CLOSE_FILES)) { /* Close all open files */
        maxfd = sysconf(_SC_OPEN_MAX);
        if (maxfd == -1)               /* Limit is indeterminate... */
            maxfd = BD_MAX_CLOSE;      /* so take a guess */

        for (fd = 0; fd < maxfd; fd++)
            close(fd);
    }

    if (!(flags & BD_NO_REOPEN_STD_FDS)) {
        close(STDIN_FILENO);   /* Reopen standard fd's to /dev/null */

        fd = open("/dev/null", O_RDWR);

        if (fd != STDIN_FILENO)         /* 'fd' should be 0 */
            return -1;
        if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
            return -1;
        if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
            return -1;
    }

    return 0;
}


int EchoServ::start()
{
	if(becomeDaemon(0) == -1)
    {
		perror("becomedaemon");
    }
	sigemptyset(&m_sa.sa_mask);
	m_sa.sa_flags = SA_RESTART;
	m_sa.sa_handler = killChild;
	if(sigaction(SIGCHLD, &m_sa, NULL) == -1)
    {
		printf("Error from sigaction(): %s", strerror(errno));
		exit(EXIT_FAILURE);
    }
	
	m_sfd = inetListen(SERVICE, 10, NULL);

	if(m_sfd == -1)
    {
		printf("Could not create server socket (%s)",
			   strerror(errno));
		exit(EXIT_FAILURE);
    }
	for(;;)
    {
		m_cfd = accept(m_sfd, NULL, NULL);
		if(m_cfd == -1)
		{
			printf("Failure in accept(): %s", strerror(errno));
			exit(EXIT_FAILURE);
		}
    }
	switch(fork()) {
    case -1:
		printf("Can't create child(%s)", strerror(errno));
		close(m_cfd);
		exit(EXIT_FAILURE);
		break;
    case 0:
		close(m_sfd);
		handleRequest(m_cfd);
		_exit(EXIT_SUCCESS);
	default:
		close(m_cfd);
		break;
    }    
	return 0;
}



