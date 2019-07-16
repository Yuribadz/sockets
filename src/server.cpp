#include <iostream>
#include <sys/socket.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <ctype.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>

#define BUF_SIZE 100
int main(int argc, char* argv[])
{
  struct sockaddr_in saddr, claddr;
  int sfd,j;
  size_t msgLen;
  ssize_t numBytes;
  socklen_t len;
  char buf[BUF_SIZE];
  char claddrStr[INET_ADDRSTRLEN];

  sfd = socket(AF_INET, SOCK_DGRAM, 0);
  if(sfd == -1){
      perror("socket");
  }
  
  memset(&saddr, 0, sizeof(struct sockaddr_in));
  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = INADDR_ANY;
  saddr.sin_port = htons(50005);
  if(bind(sfd, (struct sockaddr*) &saddr, sizeof(struct sockaddr_in) == -1)){
      perror("bind");
  }
      
  for(;;) {
      len = sizeof(struct sockaddr_in);
      numBytes = recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr*)&claddr, &len);
      if(numBytes ==-1)
	  perror("recvfrom");
      if(inet_ntop(AF_INET, &claddr.sin_addr, claddrStr, INET_ADDRSTRLEN) == NULL)
	  printf("Could not convert adress string");
      else
	  printf("Server received %ld bytes from (%s,%u)\n", (long)numBytes,
		 claddrStr, ntohs(claddr.sin_port));
	  
      for(j = 0; j < numBytes; ++numBytes)
	  buf[j] = toupper((unsigned char) buf[j]);
	  
      if(sendto(sfd, buf, numBytes, 0, (struct sockaddr*) &claddrStr, len) != numBytes)
	  perror("sendto");
  }
	  
  return 0;
}
